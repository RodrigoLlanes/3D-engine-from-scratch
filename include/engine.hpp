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

#include "vectors.hpp"

#include "shaders/shader.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>


namespace Engine {
    struct Job {
        int buffer, n;
    };

    struct Transparency {
        Vector4i color;
        float z;
    };


    class Engine {
    private:
        std::vector<std::vector<Shaders::Shader::VertexData*>*> _buffers;
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
        Vector3i _toScreen(Vector4f pos);
        void _brasenham(Vector3i p0, Vector3i p1);

        void _clearDepthBuffer();
        void _clearScreenBuffer();
        void _clearAlphaBuffer();

    public:
        Shaders::Shader* shader;

        Engine(int width, int height);
        ~Engine();

        // Buffers
        int createBuffer();
        void bindBuffer(int buffer, std::vector<Shaders::Shader::VertexData*>* value);

        bool init();
        void draw(bool depth=false);

        // 2D drawing
        void setPixel(int x, int y, int r, int g, int b, int a=255);
        void setPixel(int x, int y, Vector3i color);
        void setPixel(int x, int y, Vector4i color);
        void drawLine(int x0, int y0, int x1, int y1);
        void drawPoint(int x, int y);

        // 3D drawing
        void drawTriangle(int buffer, int n);

        void switchBuffers();
    };
}

#endif //GRAPHIC_ENGINE_ENGINE_HPP
