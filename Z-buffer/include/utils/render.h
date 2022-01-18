#ifndef RENDER_H
#define RENDER_H

//#include <opencv2\core\core.hpp>
//#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <utils/buffer.h>
#include <Eigen/Dense>

// ��color buffer�е�������Ⱦ��ͼ����
void renderToImage(std::string filename, buffer2D<Eigen::Vector3f> buf)
{
	cv::Mat image(buf.getHeight(), buf.getWidth(), CV_32FC3, buf.data());
	image.convertTo(image, CV_8UC3, 1.0f);
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

	cv::imwrite(filename, image);
}



#endif // !RENDER_H
