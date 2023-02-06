#ifndef _SCAN_H
#define _SCAN_H

#define MAX_STR_LEN     (1 << 12)
#define SCAN_NOP               0
#define SCAN_FOUND       (1 << 0)
#define SCAN_EOL         (1 << 1)
#define SCAN_EOF         (1 << 2)
#define SCAN_BUF_FULL    (1 << 3)
#define SCAN_NO_FILE     (1 << 4)

unsigned char str_scan(FILE *f, char *str, unsigned int max_len);

#endif
