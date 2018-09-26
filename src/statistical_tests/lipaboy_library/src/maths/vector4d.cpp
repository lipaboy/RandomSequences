#include "vector4d.h"

namespace LipaboyLib {

Vector4D::Vector4D(double vect1[4])
{
    container[0] = vect1[0];
    container[1] = vect1[1];
    container[2] = vect1[2];
    container[3] = vect1[3];
}

Vector4D::Vector4D(double v1, double v2, double v3, double v4)
{
    container[0] = v1;
    container[1] = v2;
    container[2] = v3;
    container[3] = v4;
}

Vector4D Vector4D::operator*(const double coef) const
{
    Vector4D newVec({0.0, 0.0, 0.0, 0.0});

    for (int i = 0; i < 4; i++) {
        newVec.container[i] = this -> container[i] * coef;
    }

    return newVec;
}

//Vector4D Vector4D::operator*(const Vector4D &obj) const
//{
//    Vector4D newVec({0.0, 0.0, 0.0, 0.0});

//    for (int i = 0; i < 4; i++)
//        newVec[i] = this -> container[i] * obj.container[i];

//    return newVec;
//}

double operator*(const VectorRow4D &v1, const VectorColumn4D &v2)
{
    double res = 0.0;
    for (int i = 0; i < 4; i++) {
        res += v1[i] * v2[i];
    }

    return res;
}


}
