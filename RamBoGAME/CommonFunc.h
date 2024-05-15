

#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_

#include <windows.h>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "TextObject.h"
#include "BaseObject.h"
#include <fstream>

static  SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;

static SDL_Event g_event;

#define GROUND_MAP 600

const int COLOR_KEY_R = 180;
const int COLOR_KEY_G = 180;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0XFF;

//Screen
const int FRAMES_PER_SECOND = 55;
const int SCREEN_WIDTH = 1280; //Fix location
const int SCREEN_HEIGHT = 640;

typedef struct GeometricFormat
{
public:
    GeometricFormat(int left, int top, int width, int height)
    {
        left_ = left;
        top_ = top;
        width_ = width;
        height_ = height;
    }

    int left_;
    int top_;
    int width_;
    int height_;
};

typedef struct ColorData
{
public:
    ColorData(Uint8 r, Uint8 g, Uint8 b)
    {
        red_ = r, green_ = g, blue_ = b;
    }
    Uint8 red_;
    Uint8 green_;
    Uint8 blue_;
};

namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
        const std::string& menu1,
        const std::string& menu2,
        const std::string& img_name);
    int MyRandom(int min, int max);
}

#endif
