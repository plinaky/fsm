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


#define TEST_EMPTYLINE
//#define DEBUG_EMPTYLINE

struct parser {
	char *buf;
	unsigned int pos;
	unsigned int len;
	unsigned int off;
	FSM_DECLARE(operator);
};

FSM_STATE(normal_mode,   myfsm);
FSM_STATE(newline_detected, myfsm);
FSM_STATE(newnewline_detected, myfsm);

#ifdef DEBUG_EMPTYLINE

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

FSM_STATE(normal_mode, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF :
			FSM_STOP(myfsm);
		case '\r' :
		case '\n' :
			par->off += 1;
			FSM_NEXT(myfsm, newline_detected);
		default   :
			par->pos += 1;
			//FSM_NEXT(myfsm, normal_mode);
	}
}

FSM_STATE(newline_detected, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF  :
			FSM_STOP(myfsm);
		case ' '  :
		case '\t' :
			par->pos += 1;
			FSM_NEXT(myfsm, newline_detected);
		case '\r' :
		case '\n' :
			par->pos += 1;
			par->off  = 0;
			FSM_NEXT(myfsm, newnewline_detected);
		default   :
			par->pos += 1;
			par->off  = 0;
			FSM_NEXT(myfsm, normal_mode);
	}
}

FSM_STATE(newnewline_detected, myfsm)
{
	struct parser *par = container_of(myfsm, struct parser, operator);

	LOG_STATE(par);

	if (par->pos >= par->len)
		FSM_STOP(myfsm);

	switch (par->buf[par->pos + par->off]) {
		case EOF  :
			FSM_STOP(myfsm);
		case ' '  :
		case '\t' :
			par->off += 1;
			FSM_NEXT(myfsm, newnewline_detected);
		case '\r' :
		case '\n' :
			skip(par);
			par->off = 1; /* no += !*/
			FSM_NEXT(myfsm, newnewline_detected);
		default   :
			par->pos += par->off;
			par->off = 0; /* no += !*/
			FSM_NEXT(myfsm, normal_mode);
	}
}


unsigned int emptyline(char *buf, unsigned int len)
{
	struct parser par = {buf, 0, len, 0};

	FSM_RUN(&par.operator, normal_mode);

	return par.pos;
}


#ifdef TEST_EMPTYLINE

int main(int argc, char *argv[])
{
	long int fd;
	int ret;
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

	ret = emptyline(mem, stat_buf.st_size);

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
