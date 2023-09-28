//
// Created by Rodrigo Llanes on 25/09/2023.
//

#ifndef GRAPHIC_ENGINE_ENGINE_HPP
#define GRAPHIC_ENGINE_ENGINE_HPP

#include <algorithm>
#include <initializer_list>
#include <cstdio>
#include <iostream>
#include <queue>

#include <Eigen/Dense>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>


using Eigen::Vector4i;
using Eigen::Vector3i;
using Eigen::Vector2i;

struct Job {
    int buffer, n, start, stride;
    int colorStart = -1, colorStride = -1;
};

struct Transparency {
    int r, g, b, a;
    float z;
};

struct Vec3 {
    float x, y, z;
};


class Engine {
private:
    std::vector<float*> _buffers;
    std::queue<Job*> _jobs;
    uint32_t *_screenBuffer;
    float *_depthBuffer;
    Transparency *_alphaBuffer;
    int *_alphaLengthBuffer;
    int *_maxBuffer, *_minBuffer;
    int _width, _height;

    SDL_Window* _window;
    SDL_Renderer *_renderer;
    SDL_Texture* _screenTexture;
    SDL_Rect _screenRect;

    void _buildBuffers();
    Vec3 _toScreen(float x, float y, float z);
    void _brasenham(int x0, int y0, int x1, int y1);

    void _clearDepthBuffer();
    void _clearScreenBuffer();
    void _clearAlphaBuffer();

public:
    Engine(int width, int height);
    ~Engine();

    // Buffers
    int createBuffer();
    void bindBuffer(int buffer, float* value);

    bool init();
    void draw(bool depth=false);

    // 2D drawing
    void setPixel(int x, int y, int r, int g, int b, int a=255);
    void setPixel(Vector2i pos, Vector3i color);
    void setPixel(Vector2i pos, Vector4i color);
    void drawLine(int x0, int y0, int x1, int y1);
    void drawPoint(int x, int y);

    // 3D drawing
    void drawTriangle(int buffer, int n, int start, int stride);
    void setVertexColor(int start, int stride);

    void switchBuffers();
};

#endif //GRAPHIC_ENGINE_ENGINE_HPP
