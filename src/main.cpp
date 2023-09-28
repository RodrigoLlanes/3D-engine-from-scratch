#include <engine.hpp>
#include <image.hpp>

#include <iostream>
#include <fstream>


using namespace std;


void plotImage(Image img) {
    Engine engine(img.width, img.height);

    engine.init();

    bool close = false;
    while (!close) {
        SDL_Event event;
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    cout << "click at " << x << " " << y << endl;
                    break;
            }
        }

        for (int x = 0; x < img.width; x++) {
            for (int y = 0; y < img.height; y++) {
                Vector4i color = img.getRGBA(x, y);
                engine.setPixel(x, y, color.x(), color.y(), color.z());
            }
        }

        engine.switchBuffers();
        SDL_Delay(1000 / 60);
    }
}


int main(int argc, char *argv[])
{
    Engine engine(1000, 1000);

    engine.init();

    bool depth = false;

    /*float *triangle = new float[42] {
        // Points        // Colors (RGBA)
        -0.5,  0.5, 0.0, 255,   0,   0, 255,
         0.5,  0.5, 0.5, 255,   0,   0, 255,
         0.0, -0.5, 0.5, 255,   0,   0, 255,

        -0.5, -0.5, 0.5, 255,   0,   0, 255,
         0.5, -0.5, 0.0,   0, 255,   0, 255,
         0.0,  0.5, 0.0,   0,   0, 255, 255
    };*/

    float *triangle = new float[7 * 3 * 4] {
            // Points        // Colors (RGBA)
            -0.5,  0.5, 0.1, 255,   0,   0, 125,
            0.5,  1.5, 0.1, 255,   0,   0, 125,
            0.0, -0.5, 0.1, 255,   0,   0, 125,

            -0.3, -0.3, 0.2,   255, 255,   0, 125,
            1.3, -0.3, 0.2,   255, 255,   0, 125,
            0.0,  0.3, 0.2,   255, 255,   0, 125,

            -1.5, -0.5, 0.5,   0, 255,   0, 255,
            0.5, -0.5, 0.5,   0, 255,   0, 255,
            0.0,  0.5, 0.5,   0, 255,   0, 255,

            -0.5, -1.7, 0.0,   0, 0,   255, 125,
            0.5, -0.7, 0.0,   0, 0,   255, 125,
            0.0,  0.7, 0.0,   0, 0,   255, 125,
    };

    int buffer = engine.createBuffer();
    engine.bindBuffer(buffer, triangle);

    bool close = false;
    while (!close) {
        SDL_Event event;
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                close = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_d) { depth = !depth; } 
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                std::cout << "click at " << x << " " << y << std::endl;
                break;
            }
        }

        engine.drawTriangle(buffer, 3 * 4, 0, 7);
        engine.setVertexColor(3, 7);

        engine.draw(depth);
        SDL_Delay(1000 / 60);
    }
    return 0;
}