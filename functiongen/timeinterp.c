#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "ex4.h"


double table_accel(int timeidx);

double faccel(double time);

double interpretAccel(double t);

#define STEPS_PER_SEC (10)

void main(int argc, char *argv[])
{
    int idx;
    int steps_per_sec=STEPS_PER_SEC;
    double time, dt;


    printf("argc=%d, argv[0]=%s\n", argc, argv[0]);

    if(argc == 2)
    {
        sscanf(argv[1], "%d", &steps_per_sec);
        printf("Will use %d steps per sec\n", steps_per_sec);
    }
    else
    {
        printf("Use: trainprofile [steps_per_sec]\n");
        printf("Will use default time profile\n");
    }

    dt = 1.0 / steps_per_sec;

    printf("Step size of %lf\n", dt);


    printf("Number of values in profile = %lu for 1801 expected\n", sizeof(DefaultProfile)/sizeof(double));

    for(idx=0; idx <= (steps_per_sec*1800); idx++)
    {
        // time you would use in your integrator and faccel(time) is the fuction to integrate
        time = 0.0 + (dt*(double)idx);
        printf("%015.14lf, %015.14lf\n", time, interpretAccel(time));
    }
}

// indexes into excel sheet, checks bounds, then returns acceleration value
double table_accel(int timeidx)
{
    long unsigned int tsize = sizeof(DefaultProfile) / sizeof(double);

    // Check array bounds for look-up table
    if(timeidx > tsize)
    {
        printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize-1);
        exit(-1);
    }

    return DefaultProfile[timeidx];
}



double interpretAccel(double t) {
    // Using typecast to floor the passed time, giving x1 / x2
    int x1 = (int)t;
    int x2 = (int)t + 1;


    // subtract out whole numbers, leaving the decimal place we want to find
    double dT = t - (double)((int)(t));

    // y = y1 + ( (x - x1)(y2-y1) / (x2 - x1) )
    double accel = table_accel(x1) + ( (table_accel(x2) - table_accel(x1))  * dT );
    return accel;
}
