/*****************************************************************************/
/*
https://www.programmingalgorithms.com/algorithm/smooth-sort/c/
*/
#include "stdio.h"

/*****************************************************************************/
#define UP(IA,IB) temp = IA; IA += IB + 1; IB = temp;
#define DOWN(IA,IB) temp = IB; IB = IA - IB - 1; IA = temp;

struct smoothie {
	int q;
	int r;
	int p;
	int b;
	int c;
	int r1;
	int b1;
	int c1;
} s = {1, 0, 1, 1, 1, 0, 0, 0};

int data[] = {654, 456, 456541, 54, 6541};

/*****************************************************************************/
void sift()
{
	int r0, r2, temp;
	int t;
	r0 = s.r1;
	t = data[r0];

	while (s.b1 >= 3) {
		r2 = s.r1 - s.b1 + s.c1;

		if (data[s.r1 - 1] > data[r2]) {
			r2 = s.r1 - 1;
			DOWN(s.b1, s.c1);
		}

		if (data[r2] > t) {
			data[s.r1] = data[r2];
			s.r1 = r2;
			DOWN(s.b1, s.c1);
		} else {

			s.b1 = 1;
		}
	}

	if (s.r1 - r0)
		data[s.r1] = t;
}

/*****************************************************************************/
void trinkle()
{
	int p1, r2, r3, r0, temp;
	int t;
	p1 = s.p;
	s.b1 = s.b;
	s.c1 = s.c;
	r0 = s.r1;
	t = data[r0];

	while (p1 > 0) {
		while ((p1 & 1) == 0) {
			p1 >>= 1;
			UP(s.b1, s.c1)
		}

		r3 = s.r1 - s.b1;

		if ((p1 == 1) || (data[r3] <= t)) {
			p1 = 0;
		} else {
			--p1;
			if (s.b1 == 1) {
				data[s.r1] = data[r3];
				s.r1 = r3;
			} else {
				if (s.b1 >= 3) {
					r2 = s.r1 - s.b1 + s.c1;

					if (data[s.r1 - 1] > data[r2]) {
						r2 = s.r1 - 1;
						DOWN(s.b1, s.c1);
						p1 <<= 1;
					}
					if (data[r2] > data[r3]) {
						data[s.r1] = data[r2];
						s.r1 = r2;
						DOWN(s.b1, s.c1);
						p1 = 0;
					} else {
						data[s.r1] = data[r3]; s.r1 = r3;
					}
				}
			}
		}
	}

	if (r0 - s.r1)
		data[s.r1] = t;

	sift();
}

/*****************************************************************************/
void semi_trinkle()
{
	int t;
	s.r1 = s.r - s.c;

	if (data[s.r1] > data[s.r]) {
		t = data[s.r];
		data[s.r] = data[s.r1];
		data[s.r1] = t;
		trinkle();
	}
}

void smoothsort(int N)
{
	int temp;

	while (s.q < N) {
		s.r1 = s.r;
		if ((s.p & 7) == 3) {
			s.b1 = s.b;
			s.c1 = s.c;
			sift();
			s.p = (s.p + 1) >> 2;
			UP(s.b, s.c);
			UP(s.b, s.c);
		} else if ((s.p & 3) == 1) {
			if (s.q + s.c < N) {
				s.b1 = s.b;
				s.c1 = s.c;
				sift();
			} else {
				trinkle();
			}

			DOWN(s.b, s.c);
			s.p <<= 1;

			while (s.b > 1) {
				DOWN(s.b, s.c);
				s.p <<= 1;
			}
			s.p++;
		}
		s.q++;
		s.r++;
	}

	s.r1 = s.r;
	trinkle();

	while (s.q > 1) {
		--s.q;

		if (s.b == 1) {
			s.r--;
			s.p--;

			while ((s.p & 1) == 0) {
				s.p >>= 1;
				UP(s.b, s.c);
			}
		} else {
			if (s.b >= 3) {
				s.p--;
				s.r = s.r - s.b + s.c;
				if (s.p > 0)
					semi_trinkle();

				DOWN(s.b, s.c);
				s.p = (s.p << 1) + 1;
				s.r = s.r + s.c;
				semi_trinkle();
				DOWN(s.b, s.c);
				s.p = (s.p << 1) + 1;
			}
		}
	}
}


/*****************************************************************************/
int main(int argc, char *argv[])
{
	smoothsort(5);

	for (int i = 0 ; i < 5 ; i++)
		printf("%d\n", data[i]);
}
