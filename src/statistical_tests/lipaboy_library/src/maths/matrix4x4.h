#ifndef MATRIX4x4_H
#define MATRIX4x4_H

#include "vector4d.h"

namespace LipaboyLib {

//TODO: rename namespace to LipaboyLib namespace

class Matrix4x4
{
private:
    std::vector<double> matrix = std::vector<double>(4 * 4);

public:
    //Matrix4x4(const QVector<double>& matr);
    Matrix4x4() {}
    //Matrix4x4(float matr[16]);
    Matrix4x4(double matr[16]);
    Matrix4x4(const Matrix4x4& obj);
    Matrix4x4(const VectorColumn4D& v1, const VectorColumn4D& v2, const VectorColumn4D& v3,
              const VectorColumn4D& v4);
    Matrix4x4(const VectorRow4D& v1, const VectorRow4D& v2, const VectorRow4D& v3,
              const VectorRow4D& v4);

    VectorColumn4D operator* (const VectorColumn4D& container) const;
    Matrix4x4 operator* (const Matrix4x4& container) const;
    const Matrix4x4& operator*= (const Matrix4x4& container) { return ((*this) = (*this) * container); }
    Matrix4x4 operator- (const Matrix4x4& container) const;
    Matrix4x4 operator+ (const Matrix4x4& container) const;
    const Matrix4x4& operator*= (const double coef);
    Matrix4x4 operator* (const double coef) { return (Matrix4x4(*this) *= coef); }

    double get(uint32_t row, uint32_t column) const { return matrix[row * 4 + column]; }
    double& get(uint32_t row, uint32_t column) { return matrix[row * 4 + column]; }
    void set(uint32_t row, uint32_t column, double value) { matrix[row * 4 + column] = value; }
    void add(uint32_t row, uint32_t column, double value) { matrix[row * 4 + column] += value; }
    void transpose();

    static Matrix4x4 getEye();
    static Matrix4x4 getEye3D();
    static Matrix4x4 getDualMatrix(Vector3D container);
    static Matrix4x4 getDiagonal(Vector4D container);
};

Matrix4x4 operator* (const VectorColumn4D& v1, const VectorRow4D& v2);

}

#endif // MATRIX4D_H
