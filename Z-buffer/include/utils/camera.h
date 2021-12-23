#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <utils/global.h>

class Camera {
public:
    Camera()
    {
        position = Eigen::Vector3f(0.0, 0.0, 0.0);
        rotation = Eigen::Vector3f(0.0, 0.0, -1.0);
        eyeFov = 45.0;
        aspectRatio = 1.0;
        zNear = 0.1;
        zFar = 2000.0;
    }

    Eigen::Matrix4f getTransMatrix()
    {
        return getProjectionMatrix() * getViewMatrix() * getModelMatrix(0.0);
    }

private:
    Eigen::Vector3f position;
    Eigen::Vector3f rotation;

    float eyeFov;
    float aspectRatio;
    float zNear;
    float zFar;

    Eigen::Matrix4f getModelMatrix(float rotation_angle)
    {
        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
        return model;
    }

    Eigen::Matrix4f getViewMatrix()
    {
        Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

        Eigen::Matrix4f translate;
        translate << 1, 0, 0, -position[0],
            0, 1, 0, -position[1],
            0, 0, 1, -position[2],
            0, 0, 0, 1;

        view = translate * view;

        return view;
    }

    Eigen::Matrix4f getProjectionMatrix()
    {
        Eigen::Matrix4f projection;

        float ratio = tan(eyeFov * PI / 360.0);
        float yNear = zNear * ratio, xNear = yNear * aspectRatio;

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
};
#endif // !CAMERA_H
