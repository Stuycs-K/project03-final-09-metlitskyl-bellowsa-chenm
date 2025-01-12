#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int x;
	int y;
	int t;
} Point;

void diff(const char *a, const char *b) {
	const int N = strlen(a);
	const int M = strlen(b);
	const int MAX = N+M;
	int v[2*MAX+1];
	Point trace[MAX+1][2*MAX+1];

	for (int d = 0; d <= MAX; d++) {
		for (int k = -d; k <= d; k += 2) {
			int x, y, type;
		
			if (k == -d || (k != d && v[k-1+MAX] < v[k+1+MAX])) {
				x = v[k+1+MAX];
				type = 0; //insertion
			}
			else {
				x = v[k-1+MAX]+1;
				type = 1; //deletion
			}

			y = x-k;
			trace[d][k+MAX] = (Point){x, y, type};

			while (x < N && y < M && a[x] == b[y]) {
				x++;
				y++;
			}

			v[k+MAX] = x;
			
			if (x >= N && y >= M) {
				//this part doesnt quite work
				int c = k+MAX;
				for (int r = d; r > 0; r--) {
					printf("(%d,%c,%d) ", trace[r][c].x, b[trace[r][c].y-1], trace[r][c].t);
					if (trace[r][c].t == 0) c++;
					else c--;
				}
				printf("min edit dist is %d\n", d);
				return;
			}
		}
	}
}

int main() {
	//gcc -Wall diff.c && ./a.out > out.txt && sed 's/50529027/_/g' out.txt
	const char *a = "matthew";
	const char *b = "little";

	diff(a, b);
	return 0;
}
