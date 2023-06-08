#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fsm.h"
#include "list.h"
#include "comment.h"


#define TEST_COMMENT
//#define DEBUG_COMMENT

struct parser {
	char *buf;
	unsigned int pos;
	unsigned int len;
	unsigned int off;
	FSM_DECLARE(operator);
};

FSM_STATE(in_normal_mode,          myfsm);
FSM_STATE(entering_slash_detected, myfsm);
FSM_STATE(in_line_comment,         myfsm);
FSM_STATE(in_block_comment,        myfsm);
FSM_STATE(exit_star_detected,      myfsm);

#ifdef DEBUG_COMMENT

static inline char symb(char c)
{
	switch (c) {
	case '\n' :
	case '\r' :
	case '\t' :
	case EOF  :
		return ' ';
	}
	return c;
}

#define LOG_STATE(_PAR_) do {                             \
	printf("char: %c pos: %d len: %d off: %d (%s)\n", \
	       symb(_PAR_->buf[_PAR_->pos + _PAR_->off]), \
	       _PAR_->pos,                                \
	       _PAR_->len,                                \
	       _PAR_->off,                                \
	       __func__);                                 \
} while(0)

#else

#define LOG_STATE

#endif

static inline void skip(struct parser *par)
{
	for (int i = 0 ; i < par->off ; i++)
		printf("%c", par->buf[par->pos + i]);
	printf("\n");

	memmove(par->buf + par->pos,
		par->buf + par->pos + par->off,
		par->len - par->pos - par->off);

	par->len -= par->off;
	par->off  = 0;
}

FSM_STATE(in_normal_mode, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF : 
			FSM_STOP(myfsm);
		case '/' :
			par->off += 1;
			FSM_NEXT(myfsm, entering_slash_detected);
		default :
			par->pos += 1;
			//FSM_NEXT(myfsm, in_normal_mode);
	}
}

FSM_STATE(entering_slash_detected, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF :
			par->off -= 1;
			FSM_STOP(myfsm);
		case '/' :
			par->off += 1;
			FSM_NEXT(myfsm, in_line_comment);
		case '*' :
			par->off += 1;
			FSM_NEXT(myfsm, in_block_comment);
		default  :
			par->off -= 1;
			par->pos += 2;
			FSM_NEXT(myfsm, in_normal_mode);
	}
}

FSM_STATE(in_line_comment, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF :
			skip(par);
			FSM_STOP(myfsm);
		case '\r' :
		case '\n' :
			skip(par);
			par->pos += 1;
			FSM_NEXT(myfsm, in_normal_mode);
		default   :
			par->off += 1;
			//FSM_NEXT(myfsm, in_line_comment);
	}
}

FSM_STATE(in_block_comment, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF :
			FSM_STOP(myfsm);
		case '*' :
			par->off += 1;
			FSM_NEXT(myfsm, exit_star_detected);
		default  :
			par->off += 1;
			//FSM_NEXT(myfsm, in_block_comment);
	}
}


FSM_STATE(exit_star_detected, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF :
			FSM_STOP(myfsm);
		case '*' :
			par->off += 1;
			FSM_NEXT(myfsm, exit_star_detected);
		case '/' :
			par->off += 1;
			skip(par);
			par->pos += 1;
			FSM_NEXT(myfsm, in_normal_mode);
		default  :
			par->off += 1;
			FSM_NEXT(myfsm, in_block_comment);
	}
}


unsigned int comment(char *buf, unsigned int len)
{
	struct parser par = {buf, 0, len, 0};

	FSM_RUN(&par.operator, in_normal_mode);

	return par.pos;
}


#ifdef TEST_COMMENT

int main(int argc, char *argv[])
{
	long int fd;
	int i, ret;
	struct stat stat_buf;
	char *mem;

	if (argc < 2) {
		printf("not enough arguments\n");
		goto err;
	}

	fd = open(argv[1], O_RDWR);
	if (0 == fd) {
		printf("can't open %s\n", argv[1]);
		goto err;
	}

	ret = fstat(fd, &stat_buf);
	if (ret != 0) {
		printf("stat failed with %d\n", ret);
		goto err;
	} else {
		printf("stat found length : %ld\n", stat_buf.st_size);
	}

	mem = mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (mem == MAP_FAILED) {
		printf("Mapping Failed\n");
		goto err;
	}

	ret = comment(mem, stat_buf.st_size);

	if (munmap(mem, stat_buf.st_size) != 0) {
		printf("UnMapping Failed\n");
		close(fd);
		goto err;
	}

	if (ftruncate(fd, ret) != 0) {
		printf("trunc failed\n");
		close(fd);
		goto err;
	}

	close(fd);

	fd = open(argv[1], O_RDWR);
	if (0 == fd) {
		printf("can't open %s\n", argv[1]);
		goto err;
	}

	ret = fstat(fd, &stat_buf);
	if (ret != 0) {
		printf("stat failed with %d\n", ret);
		goto err;
	} else {
		printf("stat found length : %ld\n", stat_buf.st_size);
	}

	return 0;
err :
	return 1;
}

#endif
