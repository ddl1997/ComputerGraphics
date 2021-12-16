#ifndef RENDER_H
#define RENDER_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <utils/buffer.h>
#include <Eigen/Dense>

void renderToImage(std::string filename, buffer2D<Eigen::Vector3f> buf)
{
	cv::Mat image(buf.getHeight(), buf.getWidth(), CV_32FC3, buf.data());
	image.convertTo(image, CV_8UC3, 1.0f);
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

	cv::imwrite(filename, image);
}

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
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

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    Eigen::Matrix4f projection;

    return projection;
}

#endif // !RENDER_H
