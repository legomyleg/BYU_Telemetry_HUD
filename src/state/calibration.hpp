#pragma once
#include "rocket_state.hpp"

struct Biases {
    Vector3 accel = {0,0,0};
    Vector3 gyro = {0,0,0};
};

/*
HOW TO CALIBRATE:

For the HUD to represent the rockets state correctly, the sensor's frame needs to 
line up with the rocket's frame. Here, R1, R2, and R3 represent the rows of the 
calibration matrix that we will use to make the conversion. I am assuming that the 
x, y, and z axis will match up pretty close to one-to-one for each axis, i.e. the 
sensors won't be tilted. This means that the sensors x-axis might be the rocket's  
negative z-axis, or something similar. Here is how to generate the matrix:

                          Sensor's frame
                                |
                                V
                            x   y   z
                         x [ 0, -1,  0 ]
    Rocket's frame -->   y [ 0,  0, -1 ]
                         z [ 1,  0,  0 ]


And the multiplication works like this:

                     Sensor's measured vals
                Sensor's frame  |
                [ 0, -1,  0 ] [ x ]           [-y ] 
Rocket's frame  [ 0,  0, -1 ] [ y ]     =     [-z ]
                [ 1,  0,  0 ] [ z ]           [ x ]
                                                |
                                            Rocket's vals

*/

inline constexpr float R1[3] = {-1, 0, 0};
inline constexpr float R2[3] = {0, 1, 0};
inline constexpr float R3[3] = {-1, 0, 0};

inline Vec3 calibrate_sensor_vector(Vec3 vals) {
    float x1 = vals.x*R1[0] + vals.y*R1[1] + vals.z*R1[2];
    float y1 = vals.x*R2[0] + vals.y*R2[1] + vals.z*R2[2];
    float z1 = vals.x*R3[0] + vals.y*R3[1] + vals.z*R3[2];

    return {x1, y1, z1};
}

