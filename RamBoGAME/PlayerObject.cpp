
#include "PlayerObject.h"

PlayerObject::PlayerObject()
{
    is_falling_ = false;
    y_val_ = 1;
}

PlayerObject::~PlayerObject()
{
    Free();
}

void PlayerObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_UP:
        {
            if (is_falling_ == false)
            {
                y_val_ = -15;
            }
        }
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_UP:
        {
            y_val_ = 5;
        }
        break;
        }
    }
}


bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImageFile(path, screen);
    return ret;
}

void PlayerObject::Show(SDL_Renderer* des)
{
    BaseObject::Render(des);
}

void PlayerObject::DoFalling(SDL_Renderer* des)
{
    rect_.y += y_val_;
    if ((rect_.y + rect_.h) >= GROUND_MAP)
    {
        LoadImg("img//fl_ver3end.png", des);
        is_falling_ = true;
        DoGround(des);
    }
}

void PlayerObject::DoGround(SDL_Renderer* screen)
{
    y_val_ = 0;
    rect_.y = GROUND_MAP - rect_.h;
    is_die_ = true;
}