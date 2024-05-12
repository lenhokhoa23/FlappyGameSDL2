

#include "Geometric.h"

void Gemometric::RenderRectange(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen)
{
    SDL_Rect fillRect = { geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_ };
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF);
    SDL_RenderFillRect(screen, &fillRect);
}

void Gemometric::RenderOutline(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen)
{
    SDL_Rect outlineRect = { geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_ };
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF);
    SDL_RenderDrawRect(screen, &outlineRect);
}