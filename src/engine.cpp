//
// Created by Rodrigo Llanes on 25/09/2023.
//

#include <engine.hpp>

#define TH 2
#define OIT_LAYERS 3


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
    _screenTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);
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
            this->_screenBuffer[y * _width + x] = (255<<24) | (255<<16) | (255<<8) | 255;
        }
    }
}

int Engine::createBuffer() {
    this->_buffers.push_back(nullptr);
    return this->_buffers.size()-1;
}

void Engine::bindBuffer(int buffer, float *value) {
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
    this->_screenBuffer[index] = (255<<24) | (r<<16) | (g<<8) | b;
}

Vec3 Engine::_toScreen(float x, float y, float z) {
    return Vec3 {_width*(x+1.0f)/2.0f, _height*(y+1.0f)/2.0f, z};
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

void Engine::drawTriangle(int buffer, int n, int start, int stride) {
    Job *job = new Job{buffer, n, start, stride};
    _jobs.push(job);
}

void Engine::setVertexColor(int start, int stride) {
    _jobs.back()->colorStart = start;
    _jobs.back()->colorStride = stride;
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
        if (dy<0) dy2 = -1; else if (dy>0) dy2 = 1;
        dx2 = 0;
    }

    int x = x1;
    int y = y1;

    int numerator = longest >> 1;
    for (int i=0;i<=longest;i++) {
        setPixel(x,y,255,255,255);
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

void Engine::_brasenham(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

    if (dx < 0) { dx1 = -1; } else if (dx > 0) { dx1 = 1; }
    if (dy < 0) { dy1 = -1; } else if (dy > 0) { dy1 = 1; }
    if (dx < 0) { dx2 = -1; } else if (dx > 0) { dx2 = 1; }

    int longest = std::abs(dx);
    int shortest = std::abs(dy);
    if (longest <= shortest) {
        longest = std::abs(dy);
        shortest = std::abs(dx);
        if (dy<0) dy2 = -1; else if (dy>0) dy2 = 1;
        dx2 = 0;
    }

    int x = x0;
    int y = y0;

    int numerator = longest >> 1;
    for (int i=0;i<=longest;i++) {
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
    for (int i = std::max(0, x-5); i < std::min(_width, x+5); i++) {
        for (int j = std::max(0, y-5); j < std::min(_height, y+5); j++) {
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

    float x1, x2, x3, y1, y2, y3, z1, z2, z3;

    while (!_jobs.empty()) {
        Job *job = _jobs.front();
        _jobs.pop();

        float *buffer = this->_buffers[job->buffer];

        for (int i = 0; i < job->n; i+=3) {
            int index = job->start + i * job->stride;
            x1 = buffer[index];
            y1 = 0.0f - buffer[index + 1];
            z1 = buffer[index + 2];

            x2 = buffer[index + job->stride];
            y2 = 0.0f - buffer[index + job->stride + 1];
            z2 = buffer[index + job->stride + 2];

            x3 = buffer[index + job->stride * 2];
            y3 = 0.0f - buffer[index + job->stride * 2 + 1];
            z3 = buffer[index + job->stride * 2 + 2];

            float r1, r2, r3, g1, g2, g3, b1, b2, b3, a1, a2, a3;
            r1 = r2 = r3 = g1 = g2 = g3 = b1 = b2 = b3 = a1 = a2 = a3 = 255;
            if (job->colorStart != -1) {
                index = job->colorStart + i * job->colorStride;
                r1 = buffer[index];
                g1 = buffer[index + 1];
                b1 = buffer[index + 2];
                a1 = buffer[index + 3];

                r2 = buffer[index + job->stride];
                g2 = buffer[index + job->stride + 1];
                b2 = buffer[index + job->stride + 2];
                a2 = buffer[index + job->stride + 3];

                r3 = buffer[index + job->stride * 2];
                g3 = buffer[index + job->stride * 2 + 1];
                b3 = buffer[index + job->stride * 2 + 2];
                a3 = buffer[index + job->stride * 2 + 3];
            }

            float maxY = std::max({y1, y2, y3});
            float minY = std::min({y1, y2, y3});

            minY = 1000.0f * (minY + 1.0f) / 2.0f;
            maxY = 1000.0f * (maxY + 1.0f) / 2.0f;

            int minYg = std::max(0, (int) minY);
            int maxYg = std::min((int) ceil(maxY), _height);
            for (int i = minYg; i < maxYg; i++) {
                _maxBuffer[i] = -1;
                _minBuffer[i] = _width +1;
            }

            Vec3 p1 = _toScreen(x1, y1, 0);
            Vec3 p2 = _toScreen(x2, y2, 0);
            Vec3 p3 = _toScreen(x3, y3, 0);

            _brasenham(p1.x, p1.y, p2.x, p2.y);
            _brasenham(p3.x, p3.y, p2.x, p2.y);
            _brasenham(p1.x, p1.y, p3.x, p3.y);

            for (int y = minYg; y < ceil(maxYg); y++) {
                int start = _minBuffer[y];
                int end = _maxBuffer[y];
                for(int x = start; x <= end; x++) {
                    float px = (x/500.0f)-1.0f;
                    float py = (y/500.0f)-1.0f;

                    // https://codeplea.com/triangular-interpolation
                    float w1 = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3))/
                               ((y2 - y3) * (x1 - x3) + (x3 - x2) * (py - y3));


                    float w2 = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3))/
                               ((y2 - y3) * (x1 - x3) + (x3 - x2) * (py - y3));

                    float w3 = 1.0f - w1 - w2;

                    int r = r1*w1 + r2*w2 + r3*w3;
                    int g = g1*w1 + g2*w2 + g3*w3;
                    int b = b1*w1 + b2*w2 + b3*w3;
                    int a = a1*w1 + a2*w2 + a3*w3;

                    float z = z1*w1 + z2*w2 + z3*w3;

                    if (z < -1.0 || z > _depthBuffer[x + y * _width]) { continue; }

                    if (a < 255 - TH) {
                        int aIndex = (x + y * _width);
                        int d = _alphaLengthBuffer[aIndex];
                        if (d == OIT_LAYERS) {
                            setPixel(x, y, r, g, b, a);
                        } else {
                            _alphaLengthBuffer[aIndex]++;
                            int i;
                            for (i = aIndex * OIT_LAYERS; i < aIndex * OIT_LAYERS + d; i++) {
                                if (_alphaBuffer[i].z < z) {
                                    for (int j = aIndex * OIT_LAYERS + d; j > i; j--) {
                                        _alphaBuffer[j] = _alphaBuffer[j-1];
                                    }
                                    _alphaBuffer[i] = {r, g, b, a, z};
                                    break;
                                }
                            }
                            if (i == aIndex * OIT_LAYERS + d) {
                                _alphaBuffer[aIndex * OIT_LAYERS + d] = {r, g, b, a, z};
                            }
                        }

                        continue;
                    }
                    _depthBuffer[x + y * _width] = z;

                    if (depth) {
                        int color = 255 * ((_depthBuffer[x + y * _width]+1.0f)/2.0f);
                        setPixel(x, y, color, color, color);
                    } else {
                        setPixel(x, y, r, g, b);
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
                setPixel(x, y, _alphaBuffer[i].r, _alphaBuffer[i].g, _alphaBuffer[i].b, _alphaBuffer[i].a);
            }
        }
    }

    switchBuffers();
}

void Engine::switchBuffers() {
    SDL_UpdateTexture(_screenTexture , nullptr, _screenBuffer, _width * sizeof (uint32_t));
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _screenTexture, nullptr, &_screenRect);
    SDL_RenderPresent(_renderer);
}
