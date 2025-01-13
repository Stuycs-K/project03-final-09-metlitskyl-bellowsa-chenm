#include "utils.h"

//NOTE NEVER USE STRING CMDS HERE, WE HAVE A BYTE ARRAY
Patch *diff(const char *a, const char *b, size_t a_length, size_t b_length) {
	const int N = a_length;
	const int M = b_length;
	const int MAX = N+M;
	int v[2*MAX+1];
	memset(v, 0, (2*MAX+1)*sizeof(int));
	int trace[MAX+1][2*MAX+1][3];//x, y, t

	for (int d = 0; d <= MAX; d++) {
		for (int k = -d; k <= d; k += 2) {
			int x, y, type;
		
			if (k == -d || (k != d && v[k-1+MAX] < v[k+1+MAX])) {
				x = v[k+1+MAX];
				type = INSERT_TYPE; //insertion
			}
			else {
				x = v[k-1+MAX]+1;
				type = DELETE_TYPE; //deletion
			}

			y = x-k;
			trace[d][k+MAX][0] = x;
			trace[d][k+MAX][1] = y;
			trace[d][k+MAX][2] = type;

			while (x < N && y < M && a[x] == b[y]) {
				x++;
				y++;
			}

			v[k+MAX] = x;
			if (x >= N && y >= M) {
				Patch *res = malloc(sizeof(Patch) + d*sizeof(Point));
				res->pts = malloc(d*sizeof(Point));
				res->memory_size = d;
				
				int c = k+MAX;
				int ind = 0;
				for (int r = d; r > 0; r--) {
					//type, pos, char
					res->pts[ind] = (Point){trace[r][c][2], trace[r][c][0], (trace[r][c][2] == INSERT_TYPE)?b[trace[r][c][1]-1]:'\0'};
					ind++;
					
					if (trace[r][c][2] == INSERT_TYPE) c++;
					else c--;
				}
				
				//printf("min edit dist is %d\n", d);
				return res;
			}
		}
	}
	return 0;
}

//assumes patch is a MODIFY
//returns new length of str
int apply_patch(char *str, size_t str_length, Patch *p) {
	int length = str_length;
	for (int i = 0; i < p->memory_size; i++) {
		if (p->pts[i].type == INSERT_TYPE) str_length++;
		if (p->pts[i].type == DELETE_TYPE) str_length--;
	}
	char *result = malloc(length*sizeof(char));
	
	for (int i = length-1; i >= 0; i--) {
		
	}
}

int main() {
	//gcc -Wall diff.c && ./a.out > out.txt && sed 's/50529027/_/g' out.txt
	const char *a = "matthew";
	const char *b = "little";

	Patch *p = diff(a, b, strlen(a), strlen(b));
	printf("%ld\n", p->memory_size);
	
	for (int i = 0; i < p->memory_size; i++) {
		printf("%d ", p->pts[i].pos);
		if (p->pts[i].type == INSERT_TYPE) printf("%c", p->pts[i].ch);
		printf("\n");
	}
	
	free(p);
	return 0;
}
