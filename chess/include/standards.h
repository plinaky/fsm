#ifndef STANDARDS_H
#define STANDARDS_H

/* ISO C */
#include <assert.h>			//	Conditionally compiled macro that compares its argument to zero
#include <complex.h>		// (since C99)	Complex number arithmetic
#include <ctype.h>			//	Functions to determine the type contained in character data
#include <errno.h>			//	Macros reporting error conditions
#include <fenv.h>			// (since C99)	Floating-point environment
#include <float.h>			//	Limits of floating-point types
#include <inttypes.h>		// (since C99)	Format conversion of integer types
#include <iso646.h>			// (since C95)	Alternative operator spellings
#include <limits.h>			//	Ranges of integer types
#include <locale.h>			//	Localization utilities
#include <math.h>			//	Common mathematics functions
#include <setjmp.h>			//	Nonlocal jumps
#include <signal.h>			//	Signal handling
#include <stdalign.h>		// (since C11)	alignas and alignof convenience macros
#include <stdarg.h>			//	Variable arguments
#include <stdatomic.h>		// (since C11)	Atomic operations
//#include <stdbit.h>			// (since C23)	Macros to work with the byte and bit representations of types
#include <stdbool.h>		// (since C99)	Macros for boolean type
//#include <stdckdint.h>		// (since C23)	macros for performing checked integer arithmetic
#include <stddef.h>			//	Common macro definitions
#include <stdint.h>			// (since C99)	Fixed-width integer types
#include <stdio.h>			//	Input/output
#include <stdlib.h>			//	General utilities: memory management, program utilities, string conversions, random numbers, algorithms
#include <stdnoreturn.h>	// (since C11)	noreturn convenience macro
#include <string.h>			//	String handling
#include <tgmath.h>			// (since C99)	Type-generic math (macros wrapping math.h and complex.h)
//#include <threads.h>		// (since C11)	Thread library
#include <time.h>			//	Time/date utilities
#include <uchar.h>			// (since C11)	UTF-16 and UTF-32 character utilities
#include <wchar.h>			// (since C95)	Extended multibyte and wide character utilities
#include <wctype.h>			// (since C95)

/* spécifiques à POSIX */
#include <unistd.h>  /* Fonctions d'accès aux API POSIX */

/* gestion de la mémoire */
#include <memory.h>  /* Opérations de mémoire (copie, comparaison, etc.) */

/* additions*/
#include <sys/time.h> /* Accçs aux primitives de temps système */
#include <sys/resource.h> /* pour changer la stack size */

/* memory mapping */
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#endif
