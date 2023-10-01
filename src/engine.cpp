//
// Created by Rodrigo Llanes on 25/09/2023.
//

#include <engine.hpp>
#include "shaders/colorShader.hpp"

#define TH 2
#define OIT_LAYERS 8


namespace Engine {

    Engine::Engine(int width, int height) : _width(width), _height(height) {}

    Engine::~Engine() {
        while (_jobs.size()) {
            Job *job = _jobs.front();
            _jobs.pop();
            delete job;    // TODO: Destructor de job debe destruir su contenido?
        }

        SDL_DestroyTexture(_screenTexture);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);

        // close SDL
        SDL_Quit();
    }

    void Engine::_buildBuffers() {
        this->_screenBuffer = new uint32_t[_width * _height];
        this->_depthBuffer = new float[_width * _height];
        this->_alphaBuffer = new Transparency[_width * _height * OIT_LAYERS];
        this->_alphaLengthBuffer = new int[_width * _height];
        this->_maxBuffer = new int[_height];
        this->_minBuffer = new int[_height];
        _clearDepthBuffer();
        _screenTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, _width,
                                           _height);
        _screenRect.x = 0;
        _screenRect.y = 0;
        _screenRect.w = _width;
        _screenRect.h = _height;
    }

    void Engine::_clearDepthBuffer() {
        for (int x = 0; x < _width; x++) {
            for (int y = 0; y < _height; y++) {
                this->_depthBuffer[y * _width + x] = 1;
            }
        }
    }

    void Engine::_clearAlphaBuffer() {
        for (int i = 0; i < _width * _height; i++) {
            _alphaLengthBuffer[i] = 0;
        }
    }

    void Engine::_clearScreenBuffer() {
        for (int x = 0; x < _width; x++) {
            for (int y = 0; y < _height; y++) {
                this->_screenBuffer[y * _width + x] = (255 << 24) | (255 << 16) | (255 << 8) | 255;
            }
        }
    }

    int Engine::createBuffer() {
        this->_buffers.push_back(nullptr);
        return this->_buffers.size() - 1;
    }

    void Engine::bindBuffer(int buffer, std::vector<Shaders::Shader::VertexData*> *value) {
        this->_buffers[buffer] = value;
    }

    void Engine::setPixel(int x, int y, int r, int g, int b, int a) {
        int index = (y * this->_width) + x;
        if (a < 255) {
            uint32_t color = _screenBuffer[index];
            int oldR = (((color >> 16) & 255) * (255 - a)) / 255;
            int oldG = (((color >> 8) & 255) * (255 - a)) / 255;
            int oldB = ((color & 255) * (255 - a)) / 255;

            r = oldR + (r * a) / 255;
            g = oldG + (g * a) / 255;
            b = oldB + (b * a) / 255;
        }
        this->_screenBuffer[index] = (255 << 24) | (r << 16) | (g << 8) | b;
    }

    Vector3i Engine::_toScreen(Vector4f pos) {
        return {
                (int) (_width * (pos.x + 1.0f) / 2.0f),
                (int) (_height * (pos.y + 1.0f) / 2.0f),
                (int) pos.z
        };
    }

    bool Engine::init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            printf("error initializing SDL: %s\n", SDL_GetError());
            return false;
        }

        _window = SDL_CreateWindow("GAME",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   _width, _height, 0);

        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

        this->_buildBuffers();
        return true;
    }

    void Engine::drawTriangle(int buffer, int n) {
        Job *job = new Job{buffer, n};
        _jobs.push(job);
    }

// https://stackoverflow.com/a/11683720
    void Engine::drawLine(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

        if (dx < 0) { dx1 = -1; } else if (dx > 0) { dx1 = 1; }
        if (dy < 0) { dy1 = -1; } else if (dy > 0) { dy1 = 1; }
        if (dx < 0) { dx2 = -1; } else if (dx > 0) { dx2 = 1; }

        int longest = std::abs(dx);
        int shortest = std::abs(dy);
        if (longest <= shortest) {
            longest = std::abs(dy);
            shortest = std::abs(dx);
            if (dy < 0) dy2 = -1; else if (dy > 0) dy2 = 1;
            dx2 = 0;
        }

        int x = x1;
        int y = y1;

        int numerator = longest >> 1;
        for (int i = 0; i <= longest; i++) {
            setPixel(x, y, 255, 255, 255);
            numerator += shortest;
            if (numerator >= longest) {
                numerator -= longest;
                x += dx1;
                y += dy1;
            } else {
                x += dx2;
                y += dy2;
            }
        }
    }

    void Engine::_brasenham(Vector3i p0, Vector3i p1) {
        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

        if (dx < 0) { dx1 = -1; } else if (dx > 0) { dx1 = 1; }
        if (dy < 0) { dy1 = -1; } else if (dy > 0) { dy1 = 1; }
        if (dx < 0) { dx2 = -1; } else if (dx > 0) { dx2 = 1; }

        int longest = std::abs(dx);
        int shortest = std::abs(dy);
        if (longest <= shortest) {
            longest = std::abs(dy);
            shortest = std::abs(dx);
            if (dy < 0) dy2 = -1; else if (dy > 0) dy2 = 1;
            dx2 = 0;
        }

        int x = p0.x;
        int y = p0.y;

        int numerator = longest >> 1;
        for (int i = 0; i <= longest; i++) {
            if (y >= 0 && y < _height) {
                _maxBuffer[y] = std::max(_maxBuffer[y], std::min(x, _width));
                _minBuffer[y] = std::min(_minBuffer[y], std::max(x, 0));
            }

            numerator += shortest;
            if (numerator >= longest) {
                numerator -= longest;
                x += dx1;
                y += dy1;
            } else {
                x += dx2;
                y += dy2;
            }
        }
    }

    void Engine::drawPoint(int x, int y) {
        for (int i = std::max(0, x - 5); i < std::min(_width, x + 5); i++) {
            for (int j = std::max(0, y - 5); j < std::min(_height, y + 5); j++) {
                setPixel(i, j, 255, 0, 0);
            }
        }
    }

/**
  @brief Draws all the queued jobs in the screen

  @param depth Enables the depth buffer visualization (Unexpected behabiour with transparency)
  */
    void Engine::draw(bool depth) {
        _clearDepthBuffer();
        _clearScreenBuffer();
        _clearAlphaBuffer();

        while (!_jobs.empty()) {
            Job *job = _jobs.front();
            _jobs.pop();

            std::vector<Shaders::Shader::VertexData*> *buffer = this->_buffers[job->buffer];

            for (int i = 0; i < job->n; i += 3) {
                Shaders::Shader::VertexData* v1 = buffer->at(i);
                Shaders::Shader::VertexData* v2 = buffer->at(i + 1);
                Shaders::Shader::VertexData* v3 = buffer->at(i + 2);

                shader->vertex(v1, v2, v3);

                Vector3i s1 = _toScreen(shader->getF1()->fragCoord);
                Vector3i s2 = _toScreen(shader->getF2()->fragCoord);
                Vector3i s3 = _toScreen(shader->getF3()->fragCoord);

                int maxY = std::max({s1.y, s2.y, s3.y});
                int minY = std::min({s1.y, s2.y, s3.y});

                int maxYg = std::min((int) ceil(maxY), _height);
                int minYg = std::max(0, (int) minY);

                for (int i = minYg; i < maxYg; i++) {
                    _maxBuffer[i] = -1;
                    _minBuffer[i] = _width + 1;
                }

                _brasenham(s1, s2);
                _brasenham(s3, s2);
                _brasenham(s1, s3);

                for (int y = minYg; y < ceil(maxYg); y++) {
                    int start = _minBuffer[y];
                    int end = _maxBuffer[y];
                    for (int x = start; x <= end; x++) {
                        float px = (x / (_width / 2.0f)) - 1.0f;
                        float py = (y / (_height / 2.0f)) - 1.0f;

                        Vector4i color = shader->fragment(Vector2f(px, py));
                        float pz = shader->interpolateFloat(shader->getF1()->fragCoord.z, shader->getF2()->fragCoord.z, shader->getF3()->fragCoord.z);

                        if (pz < -1.0 || pz > _depthBuffer[x + y * _width]) { continue; }

                        if (color.w < 255 - TH) {
                            int aIndex = (x + y * _width);
                            int d = _alphaLengthBuffer[aIndex];
                            if (d == OIT_LAYERS) {
                                setPixel(x, y, color);
                            } else {
                                _alphaLengthBuffer[aIndex]++;
                                int i;
                                for (i = aIndex * OIT_LAYERS; i < aIndex * OIT_LAYERS + d; i++) {
                                    if (_alphaBuffer[i].z < pz) {
                                        for (int j = aIndex * OIT_LAYERS + d; j > i; j--) {
                                            _alphaBuffer[j] = _alphaBuffer[j - 1];
                                        }
                                        _alphaBuffer[i] = {color, pz};
                                        break;
                                    }
                                }
                                if (i == aIndex * OIT_LAYERS + d) {
                                    _alphaBuffer[aIndex * OIT_LAYERS + d] = {color, pz};
                                }
                            }

                            continue;
                        }
                        _depthBuffer[x + y * _width] = pz;

                        if (depth) {
                            int color = 255 * ((_depthBuffer[x + y * _width] + 1.0f) / 2.0f);
                            setPixel(x, y, color, color, color);
                        } else {
                            setPixel(x, y, color.x, color.y, color.z);
                        }
                    }
                }
            }
            delete job;
        }

        for (int y = 0; y < _height; y++) {
            for (int x = 0; x < _width; x++) {
                int index = (x + y * _width);
                int d = _alphaLengthBuffer[index];
                index *= OIT_LAYERS;

                for (int i = index; i < index + d; i++) {
                    setPixel(x, y, _alphaBuffer[i].color.x, _alphaBuffer[i].color.y, _alphaBuffer[i].color.z, _alphaBuffer[i].color.w);
                }
            }
        }

        switchBuffers();
    }

    void Engine::switchBuffers() {
        SDL_UpdateTexture(_screenTexture, nullptr, _screenBuffer, _width * sizeof(uint32_t));
        SDL_RenderClear(_renderer);
        SDL_RenderCopy(_renderer, _screenTexture, nullptr, &_screenRect);
        SDL_RenderPresent(_renderer);
    }

    void Engine::setPixel(int x, int y, Vector3i color) {
        setPixel(x, y, color.x, color.y, color.z);
    }

    void Engine::setPixel(int x, int y, Vector4i color) {
        setPixel(x, y, color.x, color.y, color.z, color.w);
    }

}