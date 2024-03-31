#include "mathlib.h"

#include <math.h>
/*
 * Compute the absolute value of x
*/
double Abs(double x) {
    if (x < 0) {
        x *= -1;
    }
    return x;
}
/*
 * Compute the square root of x >= 0.
 */
double Sqrt(double x) {
    // Check domain.
    if (x < 0) {
        return nan("nan");
    }
    double old = 0.0;
    double new = 1.0;
    while (Abs(old - new) > EPSILON) {
        // Specifically, this is the Babylonian method--a simplification of
        // Newton's method possible only for Sqrt(x).
        old = new;
        new = 0.5 * (old + (x / old));
    }
    return new;
}
/*
 * Compute the sine of x.
 */
double Sin(double x) {
    double i = 3;
    double neg_pos = -1;
    double fact = 3;
    double j = i;
    double numerator = x;
    double result = x;
    while (i <= 7) {
        for (j = i; j > 1; j--) {
            fact = fact * (j - 1);
            numerator = numerator * numerator;
        }
        result = result + neg_pos * numerator / fact;
        i += 2;
        neg_pos *= -1;
        fact = i;
        numerator = x;
    }
    return result;
}
/*
 * Compute the cosine of x.
 */
double Cos(double x) {
    double i = 2;
    double neg_pos = -1;
    double fact = 2;
    double j = i;
    double numerator = x;
    double result = 1;
    while (i <= 8) {
        for (j = i; j > 1; j--) {
            fact = fact * (j - 1);
            numerator = numerator * numerator;
        }
        result = result + neg_pos * numerator / fact;
        i += 2;
        neg_pos *= -1;
        fact = i;
        numerator = x;
    }
    return result;
}
/*
 * Compute the tangent of x.
 *
 * NOTE:
 * Tan(M_PI / 2) will not be undefined for two reasons:
 * (1) the IEEE 754 double precision standard cannot perfectly represent
 *     pi in binary; and
 * (2) the Taylor-Maclaurin series used to approximate Cos(pi/2) will not
 *     converge to 0.
 *  Therefore, it will instead to converge to a very, very small
 *  number resulting in a very, very large value of Tan(pi/2).
 */
double Tan(double x) {
    x = Sin(x) / Cos(x);
    return x;
}
