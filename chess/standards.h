#ifndef STANDARDS_H
#define STANDARDS_H

/* standards */
#include <assert.h>  /* Assertions */
#include <ctype.h>   /* Fonctions pour tester et manipuler des caractères */
#include <errno.h>   /* Gestion des erreurs */
#include <float.h>   /* Limites des types à virgule flottante */
#include <limits.h>  /* Limites des types entiers */
#include <locale.h>  /* Gestion des paramètres régionaux */
#include <math.h>    /* Fonctions mathématiques */
#include <setjmp.h>  /* Gestion des sauts */
#include <signal.h>  /* Gestion des signaux */
#include <stdarg.h>  /* Gestion des arguments variables */
#include <stddef.h>  /* Types et constantes de base */
#include <stdio.h>   /* Entrées/sorties standard */
#include <stdlib.h>  /* Fonctions utilitaires standard */
#include <string.h>  /* Fonctions de manipulation de chaînes */
#include <time.h>    /* Fonctions de gestion du temps */

/* spécifiques à POSIX */
#include <unistd.h>  /* Fonctions d'accès aux API POSIX */

/* gestion de la mémoire */
#include <stdlib.h>  /* Allocation dynamique de mémoire */
#include <memory.h>  /* Opérations de mémoire (copie, comparaison, etc.) */

/* types spéciaux */
#include <stdint.h>  /* Types entiers de taille fixe */
#include <stdbool.h> /* Types booléens */



/* additions*/
#include <sys/time.h> /* Accçs aux primitives de temps système */
#include <sys/resource.h> /* pour changer la stack size */

/* memory mapping */
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

#endif
