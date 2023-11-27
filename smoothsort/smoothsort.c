/*****************************************************************************/
/*
https://www.programmingalgorithms.com/algorithm/smooth-sort/c/
*/
#include "stdio.h"

#define SWAP(_A_,_B_) swap(&_A_, &_B_)
#define UP(_X_,_Y_) {SWAP(_X_, _Y_); _X_ += _Y_ + 1;}
#define DOWN(_X_,_Y_) {SWAP(_X_, _Y_); _X_ -= _Y_ + 1;}

struct indexes {
	unsigned long int q;
	unsigned long int r;
	unsigned long int p;
	unsigned long int b;
	unsigned long int c;
	unsigned long int r1;
	unsigned long int b1;
	unsigned long int c1;
};

struct smoothie {
	struct indexes  *ind;
	unsigned long int *data;
	unsigned long int nitems;
};

/*****************************************************************************/
static inline void swap(unsigned long int *a, unsigned long int *b)
{
	unsigned long int s = *b;
	*b = *a;
	*a = s;
}

/*****************************************************************************/
void sift(struct smoothie *s)
{
	int r0, r2;
	int t;
	r0 = s->ind->r1;
	t = s->data[r0];

	while (s->ind->b1 >= 3) {
		r2 = s->ind->r1 - s->ind->b1 + s->ind->c1;

		if (s->data[s->ind->r1 - 1] > s->data[r2]) {
			r2 = s->ind->r1 - 1;
			DOWN(s->ind->b1, s->ind->c1);
		}

		if (s->data[r2] > t) {
			s->data[s->ind->r1] = s->data[r2];
			s->ind->r1 = r2;
			DOWN(s->ind->b1, s->ind->c1);
		} else {

			s->ind->b1 = 1;
		}
	}

	if (s->ind->r1 - r0)
		s->data[s->ind->r1] = t;
}

/*****************************************************************************/
void trinkle(struct smoothie *s)
{
	int p1, r2, r3, r0;
	int t;
	p1 = s->ind->p;
	s->ind->b1 = s->ind->b;
	s->ind->c1 = s->ind->c;
	r0 = s->ind->r1;
	t = s->data[r0];

	while (p1 > 0) {
		while ((p1 & 1) == 0) {
			p1 >>= 1;
			UP(s->ind->b1, s->ind->c1);
		}

		r3 = s->ind->r1 - s->ind->b1;

		if ((p1 == 1) || (s->data[r3] <= t)) {
			p1 = 0;
		} else {
			--p1;
			if (s->ind->b1 == 1) {
				s->data[s->ind->r1] = s->data[r3];
				s->ind->r1 = r3;
			} else {
				if (s->ind->b1 >= 3) {
					r2 = s->ind->r1 - s->ind->b1 + s->ind->c1;

					if (s->data[s->ind->r1 - 1] > s->data[r2]) {
						r2 = s->ind->r1 - 1;
						DOWN(s->ind->b1, s->ind->c1);
						p1 <<= 1;
					}
					if (s->data[r2] > s->data[r3]) {
						s->data[s->ind->r1] = s->data[r2];
						s->ind->r1 = r2;
						DOWN(s->ind->b1, s->ind->c1);
						p1 = 0;
					} else {
						s->data[s->ind->r1] = s->data[r3]; s->ind->r1 = r3;
					}
				}
			}
		}
	}

	if (r0 - s->ind->r1)
		s->data[s->ind->r1] = t;

	sift(s);
}

/*****************************************************************************/
void semi_trinkle(struct smoothie *s)
{
	int t;
	s->ind->r1 = s->ind->r - s->ind->c;

	if (s->data[s->ind->r1] > s->data[s->ind->r]) {
		t = s->data[s->ind->r];
		s->data[s->ind->r] = s->data[s->ind->r1];
		s->data[s->ind->r1] = t;
		trinkle(s);
	}
}

/*****************************************************************************/
void smoothsort(unsigned long int *data, unsigned long int nitems)
{
	struct indexes ind = {1, 0, 1, 1, 1, 0, 0, 0};
	struct smoothie s = {&ind, data, nitems};

	while (s.ind->q < nitems) {
		s.ind->r1 = s.ind->r;
		if ((s.ind->p & 7) == 3) {
			s.ind->b1 = s.ind->b;
			s.ind->c1 = s.ind->c;
			sift(&s);
			s.ind->p = (s.ind->p + 1) >> 2;
			UP(s.ind->b, s.ind->c);
			UP(s.ind->b, s.ind->c);
		} else if ((s.ind->p & 3) == 1) {
			if (s.ind->q + s.ind->c < nitems) {
				s.ind->b1 = s.ind->b;
				s.ind->c1 = s.ind->c;
				sift(&s);
			} else {
				trinkle(&s);
			}

			DOWN(s.ind->b, s.ind->c);
			s.ind->p <<= 1;

			while (s.ind->b > 1) {
				DOWN(s.ind->b, s.ind->c);
				s.ind->p <<= 1;
			}
			s.ind->p++;
		}
		s.ind->q++;
		s.ind->r++;
	}

	s.ind->r1 = s.ind->r;
	trinkle(&s);

	while (s.ind->q > 1) {
		--s.ind->q;

		if (s.ind->b == 1) {
			s.ind->r--;
			s.ind->p--;

			while ((s.ind->p & 1) == 0) {
				s.ind->p >>= 1;
				UP(s.ind->b, s.ind->c);
			}
		} else {
			if (s.ind->b >= 3) {
				s.ind->p--;
				s.ind->r = s.ind->r - s.ind->b + s.ind->c;
				if (s.ind->p > 0)
					semi_trinkle(&s);

				DOWN(s.ind->b, s.ind->c);
				s.ind->p = (s.ind->p << 1) + 1;
				s.ind->r = s.ind->r + s.ind->c;
				semi_trinkle(&s);
				DOWN(s.ind->b, s.ind->c);
				s.ind->p = (s.ind->p << 1) + 1;
			}
		}
	}
}


/*****************************************************************************/
int main(int argc, char *argv[])
{
	unsigned long int data[] = {654, 456, 456541, 54, 6541};
	smoothsort(data, 5);

	for (int i = 0 ; i < 5 ; i++)
		printf("%lu\n", data[i]);
}
