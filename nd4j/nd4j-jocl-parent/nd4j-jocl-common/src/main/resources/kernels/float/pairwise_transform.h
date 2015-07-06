#include <math.h>

//x[i] and y[i]
__global  float op(float d1,float d2,float *params);
__global  float op(float d1,float *params);

__global  void transform(int n,int xOffset,int yOffset, float *dx, float *dy,int incx,int incy,float *params,float *result) {

    int totalThreads = get_num_groups(0) * get_local_size(0);
    int tid = get_local_id(0);
    int i = get_group_id(0) * get_local_size(0) + tid;

    if (incy == 0) {
        if ((get_group_id(0) == 0) && (tid == 0)) {
            int ix = (incx < 0) ? ((1 - n) * incx) : 0;
            for (; i < n; i++) {
                result[i * incx] = op(dx[i * incx],params);
            }

        }
    } else if ((incx == incy) && (incx > 0)) {
        /* equal, positive, increments */
        if (incx == 1) {
            /* both increments equal to 1 */
            for (; i < n; i += totalThreads) {
                  result[i] = op(dx[i],dy[i],params);
              }
        } else {
            /* equal, positive, non-unit increments. */
            for (; i < n; i += totalThreads) {
                result[i * incy] = op(dx[i * incx],dy[i * incy],params);
            }
        }
    } else {
        /* unequal or nonpositive increments */
        for (; i < n; i += totalThreads) {
            result[i] = op(dx[i * incx],dy[i * incy],params);
        }
    }
}


