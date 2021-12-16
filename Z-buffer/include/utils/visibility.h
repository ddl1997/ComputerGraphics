#ifndef VISIBILITY_H
#define VISIBILITY_H

#include <Eigen/Dense>
#include <utils/model.h>

constexpr auto PI = 3.14159265358979323846;

Eigen::Matrix4f getModelMatrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    return model;
}

Eigen::Matrix4f getViewMatrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
        0, 1, 0, -eye_pos[1],
        0, 0, 1, -eye_pos[2],
        0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f getProjectionMatrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    Eigen::Matrix4f projection;

    float ratio = tan(eye_fov * PI / 360.0);
    float yNear = zNear * ratio, xNear = yNear * aspect_ratio;

    Eigen::Matrix4f p2o = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f o_t = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f o_s = Eigen::Matrix4f::Identity();
    p2o << -zNear, 0, 0, 0,
        0, -zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1, 0;
    o_t << 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0, 1;
    o_s << 1 / xNear, 0, 0, 0,
        0, 1 / yNear, 0, 0,
        0, 0, -2 / (zFar - zNear), 0,
        0, 0, 0, 1;

    projection = o_s * o_t * p2o;

    return projection;
}



#endif // !VISIBILITY_H
