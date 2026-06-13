#pragma once
#include <raylib.h>
#include "raymath.h"

inline Matrix PhysicsToRenderMatrix = {
    0, 1, 0, 0,
    0, 0, 1, 0,
    1, 0, 0, 0,
    0, 0, 0, 1
};      

inline Quaternion apply_body_transformation_to_render(Quaternion q) {
    auto qm = QuaternionToMatrix(q);
    auto l = MatrixMultiply(qm, PhysicsToRenderMatrix);
    auto r = MatrixInvert(PhysicsToRenderMatrix);
    return QuaternionFromMatrix(MatrixMultiply(r, l));
}
