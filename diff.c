#include "utils.h"
#include <assert.h>

/*
NOTE NEVER USE STRING CMDS HERE, WE HAVE A BYTE ARRAY
returns the patch that converts one byte array to another
-remember to always free this patch
-also note that the filename isnt filled, u have to do that manually
*/
Patch *diff(const char *a, const char *b, size_t a_length, size_t b_length) {
    const int N = a_length;
    const int M = b_length;
    const int MAX = N + M;
    int v[2 * MAX + 1];
    memset(v, 0, (2 * MAX + 1) * sizeof(int));
    int trace[MAX + 1][2 * MAX + 1][3]; // x, y, t

    for (int d = 0; d <= MAX; d++) {
        for (int k = -d; k <= d; k += 2) {
            int x, y, type;

            if (k == -d || (k != d && v[k - 1 + MAX] < v[k + 1 + MAX])) {
                x = v[k + 1 + MAX];
                type = INSERT_TYPE; // insertion
            } else {
                x = v[k - 1 + MAX] + 1;
                type = DELETE_TYPE; // deletion
            }

            y = x - k;
            trace[d][k + MAX][0] = x;
            trace[d][k + MAX][1] = y;
            trace[d][k + MAX][2] = type;

            while (x < N && y < M && a[x] == b[y]) {
                x++;
                y++;
            }

            v[k + MAX] = x;
            if (x >= N && y >= M) {
                Patch *res = malloc(sizeof(Patch) + d * sizeof(Point));
                res->mode = MODE_MODIFY;
                // res->pts = malloc(d * sizeof(Point));
                // memset(res->pts, 0, d * sizeof(Point));
                res->memory_size = d;

                int c = k + MAX;
                int ind = 0;
                for (int r = d; r > 0; r--) {
                    // type, pos, char
                    res->pts[ind] = (Point){trace[r][c][2], trace[r][c][0], (trace[r][c][2] == INSERT_TYPE) ? b[trace[r][c][1] - 1] : '\0'};
                    ind++;

                    if (trace[r][c][2] == INSERT_TYPE)
                        c++;
                    else
                        c--;
                }

                return res;
            }
        }
    }
    return 0;
}

// assumes patch is a MODIFY
// returns new byte array
// assigns new byte array size to new_size
char *apply_patch(char *arr, size_t arr_length, Patch *p, size_t *new_size) {
    assert(p->mode = MODE_MODIFY);

    // calculate the resultant byte array length
    int length = arr_length;
    for (int i = 0; i < p->memory_size; i++) {
        if (p->pts[i].type == INSERT_TYPE)
            length++;
        if (p->pts[i].type == DELETE_TYPE)
            length--;
    }

    char *result = malloc(length * sizeof(char));
    int res_pt = length - 1;
    int cur_pt = 0;

    // for each byte in arr, determine whether to include it or not
    for (int i = arr_length - 1; i >= 0; i--) {

        // repeatedly add insertion updates
        while (cur_pt < p->memory_size && p->pts[cur_pt].pos == i + 1 && p->pts[cur_pt].type == INSERT_TYPE) {
            result[res_pt--] = p->pts[cur_pt].ch;
            cur_pt++;
        }

        // check if point update is a deletion, if so, just dont include arr[i] to result
        if (cur_pt < p->memory_size && p->pts[cur_pt].pos == i + 1 && p->pts[cur_pt].type == DELETE_TYPE) {
            cur_pt++;
            continue;
        }

        result[res_pt--] = arr[i];
    }

    *new_size = length;
    return result;
}

// int main() {
//     char *a = "a";
//     char *b = "little";

//     Patch *p = diff(a, b, strlen(a), strlen(b));

//     for (int i = 0; i < p->memory_size; i++) {
//         printf("%d ", p->pts[i].pos);
//         if (p->pts[i].type == INSERT_TYPE)
//             printf("%c", p->pts[i].ch);
//         printf("\n");
//     }

//     size_t new_size;
//     char *applied_patch = apply_patch(a, strlen(a), p, &new_size);

//     printf("after applying patch: |%s|\n", applied_patch);

//     free(p);
//     free(applied_patch);
//     return 0;
// }
