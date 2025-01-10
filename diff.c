#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} Point;

void diff(const char *a, const char *b) {
    const int N = strlen(a);
    const int M = strlen(b);
    const int MAX = N+M;
    int *v = calloc(2*MAX+1, sizeof(int));
    Point **trace = malloc((MAX+1)*sizeof(Point *));

    for (int d = 0; d <= MAX; d++) {
        trace[d] = malloc((2*MAX+1)*sizeof(Point));

        for (int k = -d; k <= d; k += 2) {
            int x, y;

            if (k == -d || (k != d && v[k-1+MAX] < v[k+1+MAX])) {
                x = v[k+1+MAX];
            }
            else {
                x = v[k-1+MAX]+1;
            }

            y = x-k;
            trace[d][k+MAX] = (Point){x, y};

            while (x < N && y < M && a[x] == b[y]) {
                x++;
                y++;
            }

            v[k+MAX] = x;

            if (x >= N && y >= M) {
                //this part doesnt quite work
                /*for (int d_back = d; d_back >= 0; d_back--) {
                    Point p = trace[d_back][k+MAX];
                    printf("(%d, %d)\n", p.x, p.y);
                }*/
                printf("min edit dist is %d\n", d);
                free(v);
                return;
            }
        }
    }
}

int main() {
    const char *a = "ABCABBA";
    const char *b = "CBABAC";

    diff(a, b);
    return 0;
}
