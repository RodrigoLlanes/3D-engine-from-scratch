//
// Created by Rodrigo Llanes on 25/09/2023.
//

#ifndef GRAPHIC_ENGINE_IMAGE_HPP
#define GRAPHIC_ENGINE_IMAGE_HPP


#include <Eigen/Dense>

using Eigen::Vector4f;
using Eigen::Vector4i;
using Eigen::Vector2i;


class Image {
private:
    uint8_t* image;

public:
    int width, height;

    Vector4i getRGBA(int x, int y);
    Vector4i getRGBA(Vector2i pos);

    Vector4f get(int x, int y);
    Vector4f get(Vector2i pos);

    explicit Image(char *path);
    ~Image();
};

#endif //GRAPHIC_ENGINE_IMAGE_HPP
