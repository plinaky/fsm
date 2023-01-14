#ifndef _SCAN_H
#define _SCAN_H

#define MAX_STR_LEN     (1 << 15)
#define SCAN_NOP               0
#define SCAN_FOUND       (1 << 0)
#define SCAN_EOL         (1 << 1)
#define SCAN_EOF         (1 << 2)
#define SCAN_BUF_FULL    (1 << 3)
#define SCAN_NO_FILE     (1 << 4)

unsigned char str_scan(FILE *f, char *str, unsigned int max_len);

#define ARRAY_SIZE(_ARRAY_) (sizeof(_ARRAY_) / sizeof((_ARRAY_)[0]))

#define MIN(_X_, _Y_) (_X_ > _Y_ ? _Y_ : _X_)

#define STR_SCAN(_F_, _STR_, _LEN_) \
	str_scan(_F_,_STR_, MIN(ARRAY_SIZE(_STR_), _LEN_))

#endif
