#ifndef _PREPROC_H
#define _PREPROC_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct preproc {
	FILE *f;	
};

/* return a file*/
unsigned int pre_process(struct preproc *pp);

#endif
