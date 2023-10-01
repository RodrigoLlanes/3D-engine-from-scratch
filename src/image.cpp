//
// Created by Rodrigo Llanes on 25/09/2023.
//

#include<iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "image.hpp"


// Namespace nullifies the use of cv::function();
using namespace std;


Image::Image(char *path) {
    int comp;
    image = stbi_load(path, &width, &height, &comp, 4);
}

Image::~Image() {
    stbi_image_free(image);
}

Vector4i Image::getRGBA(int x, int y) {
    int index = (y * width + x) * 4;
    return {
        image[index],
        image[index + 1],
        image[index + 2],
        image[index + 3]
    };
}

Vector4i Image::getRGBA(Vector2i pos) {
    return getRGBA(pos.x, pos.y);
}

Vector4f Image::get(int x, int y) {
    int index = (y * width + x) * 4;
    return {
        (float) image[index] / 255.0f,
        (float) image[index + 1] / 255.0f,
        (float) image[index + 2] / 255.0f,
        (float) image[index + 3] / 255.0f
    };
}

Vector4f Image::get(Vector2i pos) {
    return get(pos.x, pos.y);
}
