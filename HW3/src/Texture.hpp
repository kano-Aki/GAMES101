//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        u = u * width;
        v = (1 - v) * height;
        int u_img = (int)u;
        int v_img = (int)v;
        float delta_u = u - u_img;
        float delta_v = v - v_img;
        auto color1 = image_data.at<cv::Vec3b>(v_img, u_img);
        auto color2 = image_data.at<cv::Vec3b>(v_img, u_img + 1);
        auto color3 = image_data.at<cv::Vec3b>(v_img + 1, u_img);
        auto color4 = image_data.at<cv::Vec3b>(v_img + 1, u_img + 1);
        Eigen::Vector3f color = (1 - delta_u) * (1 - delta_v) * Eigen::Vector3f(color1[0], color1[1], color1[2]) +
                                delta_u * (1 - delta_v) * Eigen::Vector3f(color2[0], color2[1], color2[2]) +
                                (1 - delta_u) * delta_v * Eigen::Vector3f(color3[0], color3[1], color3[2]) +
                                delta_u * delta_v * Eigen::Vector3f(color4[0], color4[1], color4[2]);
        return color;
    }

};
#endif //RASTERIZER_TEXTURE_H
