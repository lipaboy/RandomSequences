
#ifndef STAT_TESTS_CEPHES_H_
#define STAT_TESTS_CEPHES_H_

namespace statistical_tests_space {

double cephes_igamc(double a, double x);
double cephes_igam(double a, double x);
double cephes_lgam(double x);
double cephes_p1evl(double x, double *coef, int N);
double cephes_polevl(double x, double *coef, int N);
double cephes_erf(double x);
double cephes_erfc(double x);
double cephes_normal(double x);

}

#endif /*  STAT_TESTS_CEPHES_H_  */
