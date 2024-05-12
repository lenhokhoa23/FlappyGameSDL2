#include "TextObject.h"


TextObject::TextObject(void)
{
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
}


TextObject::~TextObject(void)
{
}


bool TextObject::loadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen)
{
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, str_val_.c_str(), text_color_);
    if (textSurface != NULL)
    {
        //Create texture from surface pixels
        texture_ = SDL_CreateTextureFromSurface(screen, textSurface);
        if (texture_ != NULL)
        {
            //Get image dimensions
            width_ = textSurface->w;
            height_ = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return texture_ != NULL;
}

void TextObject::Free()
{
    if (texture_ != NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}

void TextObject::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void TextObject::setColor(int type)
{
    if (type == RED_TEXT)
    {
        SDL_Color color = { 255, 0, 0 };
        text_color_ = color;
    }
    else if (type == WHITE_TEXT)
    {
        SDL_Color color = { 255, 255, 255 };
        text_color_ = color;
    }
    else
    {
        SDL_Color color = { 0, 0, 0 };
        text_color_ = color;
    }
}
void TextObject::RenderText(SDL_Renderer* screen, int x, int y, SDL_Rect* clip /* = NULL */, double angle /* = 0.0 */, SDL_Point* center /* = NULL */, SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width_, height_ };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip);
}
