#include "matrix4x4.h"

namespace LipaboyLib {

//Matrix4x4::Matrix4x4(float matr[16])
//{
//    for (int i = 0; i < 4; i++) {
//       for (int j = 0; j < 4; j++) {
//           set(i, j, matr[i * 4 + j]);
//       }
//    }
//}

Matrix4x4::Matrix4x4(double matr[16])
{
    for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 4; j++) {
           set(i, j, (double)matr[i * 4 + j]);
       }
    }
}

Matrix4x4::Matrix4x4(const Matrix4x4 &obj)
{
    for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 4; j++) {
           set(i, j, obj.get(i, j));
       }
    }
}

Matrix4x4::Matrix4x4(const VectorColumn4D &v1, const VectorColumn4D &v2, const VectorColumn4D &v3,
                     const VectorColumn4D &v4)
{
    for (int j = 0; j < 4; j++)
        set(j, 0, v1[j]);
    for (int j = 0; j < 4; j++)
        set(j, 1, v2[j]);
    for (int j = 0; j < 4; j++)
        set(j, 2, v3[j]);
    for (int j = 0; j < 4; j++)
        set(j, 3, v4[j]);
}

Matrix4x4::Matrix4x4(const VectorRow4D &v1, const VectorRow4D &v2, const VectorRow4D &v3, const VectorRow4D &v4) :
    Matrix4x4(transposeVec(v1), transposeVec(v2), transposeVec(v3), transposeVec(v4))
{

}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &obj) const
{
    Matrix4x4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            res.set(i, j, get(i, j) - obj.get(i, j));
        }
    }
    return res;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &obj) const
{
    Matrix4x4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            res.set(i, j, get(i, j) + obj.get(i, j));
        }
    }
    return res;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &obj) const
{
    Matrix4x4 res;
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++){
            auto temp = get(i, k);
            for (int j = 0; j < 4; j++){
                res.add(i, j, temp * obj.get(k, j));
            }
        }
    }
    return res;
}

VectorColumn4D Matrix4x4::operator*(const VectorColumn4D &container) const
{
    VectorColumn4D newVec(0, 0, 0, 0);

    for (int i = 0; i < 4; i++) {
        //double temp = container[i];
        for (int j = 0; j < 4; j++) {
            newVec[i] += get(i, j) * container[j];
        }
    }

    return newVec;
}

const Matrix4x4& Matrix4x4::operator*=(const double coef)
{
     for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            set(i, j, get(i, j) * coef);
        }
     }

     return *this;
}

void Matrix4x4::transpose()
{
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            auto temp = get(i, j);
            set(i, j, get(j, i));
            set(j, i, temp);
        }
    }
}

Matrix4x4 Matrix4x4::getDualMatrix(Vector3D container)
{
    double arr[] = {0, -container.z(), container.y(), 0,
                   container.z(), 0, -container.x(), 0,
                   -container.y(), container.x(), 0, 0,
                   0, 0, 0, 0};
    return Matrix4x4(arr);
}

Matrix4x4 Matrix4x4::getDiagonal(Vector4D container)
{
    Matrix4x4 matr;
    for (int i = 0; i < 4; i++) {
        matr.set(i, i, container.get(i));
    }
    return matr;
}

Matrix4x4 Matrix4x4::getEye()
{
    double arr[] = {1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1};
    return Matrix4x4(arr);
}

Matrix4x4 Matrix4x4::getEye3D()
{
    double arr[] = {1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 0};
    return Matrix4x4(arr);
}

Matrix4x4 operator*(const VectorColumn4D &v1, const VectorRow4D &v2)
{
    Matrix4x4 matrix;

    for (int i = 0; i < 4; i++) {
        double temp = v1[i];
        for (int j = 0; j < 4; j++) {
            matrix.set(i, j, temp * v2[j]);
        }
    }

    return matrix;
}

}

