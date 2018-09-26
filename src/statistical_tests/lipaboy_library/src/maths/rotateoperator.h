#ifndef ROTATEOPERATOR_H
#define ROTATEOPERATOR_H

#include "vector3d.h"
#include "matrix4x4.h"

namespace LipaboyLib {

class RotateOperator
{
public:
    RotateOperator(Vector3D asixRotation1, double _radianAngle);

    Vector3D rotate(Vector3D container) const { return (rotateMat * VectorColumn4D(container, 0)).toVector3D(); }
    const Matrix4x4& getMatrix() const { return rotateMat; }

private:
    Vector3D asixRotation;
    double angle;
    Matrix4x4 rotateMat;
};

}

#endif // ROTATEOPERATOR_H
