#include <algorithm>
#include <initializer_list>
#include <stdio.h>
#include <iostream>
#include <queue>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

using namespace std;


struct Job {
    int length;
    float *points;
    int *color;
};

queue<Job*> jobs;

uint32_t *textureBuffer;
int width = 1000, height = 1000;

void drawTriangle(float points[9], int color[9]) {
    Job *job = new Job{9, points = points, color = color};
    jobs.push(job);
}


void setPixel(int x, int y, int a, int r, int g, int b) {
    textureBuffer[(y*width) + x] = (a<<24) | (r<<16) | (g<<8) | b;
}

float sign(float v1x, float v1y, float v2x, float v2y, float v3x, float v3y)
{
    return (v1x - v3x) * (v2y - v3y) - (v2x - v3x) * (v1y - v3y);
}

bool PointInTriangle(float px, float py, float v1x, float v1y, float v2x, float v2y, float v3x, float v3y)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(px, py, v1x, v1y, v2x, v2y);
    d2 = sign(px, py, v2x, v2y, v3x, v3y);
    d3 = sign(px, py, v3x, v3y, v1x, v1y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void draw() {
    while (jobs.size()) {
        Job *job = jobs.front();
        jobs.pop();

        float x1, x2, x3, y1, y2, y3;
        x1 = job->points[0];
        x2 = job->points[3];
        x3 = job->points[6];
        y1 = 0.0 - job->points[1];
        y2 = 0.0 - job->points[4];
        y3 = 0.0 - job->points[7];
        
        int r1, r2, r3, g1, g2, g3, b1, b2, b3;
        r1 = job->color[0];
        g1 = job->color[1];
        b1 = job->color[2];
        r2 = job->color[3];
        g2 = job->color[4];
        b2 = job->color[5];
        r3 = job->color[6];
        g3 = job->color[7];
        b3 = job->color[8];

        float maxX = max({x1, x2, x3});
        float maxY = max({y1, y2, y3});
        float minX = min({x1, x2, x3});
        float minY = min({y1, y2, y3});

        for(int x = 1000.0*(minX+1.0)/2.0; x < ceil(1000.0*(maxX+1.0)/2.0); x++) {
            for(int y = 1000.0*(minY+1.0)/2.0; y < ceil(1000.0*(maxY+1.0)/2.0); y++) {
                float px = (x/500.0f)-1.0f;
                float py = (y/500.0f)-1.0f;
                if (PointInTriangle(px, py, x1, y1, x2, y2, x3, y3)) {
                    float d1 = sqrt((px-x1) * (px-x1) + (py-y1)*(py-y1));
                    float d2 = sqrt((px-x2) * (px-x2) + (py-y2)*(py-y2));
                    float d3 = sqrt((px-x3) * (px-x3) + (py-y3)*(py-y3));
                    float s = d1+d2+d3;
                    d1/=s;
                    d2/=s;
                    d3/=s;
                    d1 = 1.0-d1;
                    d2 = 1.0-d2;
                    d3 = 1.0-d3;

                    int r = r1*d1 + r2*d2 + r3*d3;
                    int g = g1*d1 + g2*d2 + g3*d3;
                    int b = b1*d1 + b2*d2 + b3*d3;

                    setPixel(x, y, 255, r, g, b);
                }
            }
        }
        delete job;
    }
}


int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height, 0);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    textureBuffer = new uint32_t[width * height];
    SDL_Rect texture_rect;
    texture_rect.x = 0;
    texture_rect.y = 0;
    texture_rect.w = width;
    texture_rect.h = height;

    bool close = false;
    while (!close) {
        SDL_Event event;
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                close = true;
                break;
            }
        }

        int colors[] = {
            255, 0, 0,
            0, 255, 0,
            0, 0, 255
        };

        float points[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5, 0.0
        };

        int colors2[] = {
            255, 0, 0,
            255, 0, 0,
            255, 0, 0
        };
        float points2[] = {
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.0f, -0.5, 0.5
        };
        drawTriangle(points, colors);
        drawTriangle(points2, colors2);

        draw();
        SDL_UpdateTexture(texture , NULL, textureBuffer, width * sizeof (uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
 
    // destroy texture
    SDL_DestroyTexture(texture);
 
    // destroy renderer
    SDL_DestroyRenderer(renderer);
 
    // destroy window
    SDL_DestroyWindow(window);
    
    // close SDL
    SDL_Quit();
 
    return 0;
}