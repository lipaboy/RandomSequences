#include "rotateoperator.h"

namespace LipaboyLib {

RotateOperator::RotateOperator(Vector3D asixRotation1, double angle1) :
    asixRotation(asixRotation1),
    angle(angle1)
{
    asixRotation1.normalize();
    VectorColumn4D vAsix(asixRotation1, 0);

    Matrix4x4 temp = vAsix * transposeVec(vAsix);
    Matrix4x4 I = Matrix4x4::getEye3D();
    Matrix4x4 dual = Matrix4x4::getDualMatrix(asixRotation1);

    rotateMat = temp + (I - temp) * std::cos(angle1) + dual * std::sin(angle1);
    rotateMat.set(3, 3, 1);
}


}
