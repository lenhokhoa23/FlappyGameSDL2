
#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include "CommonFunc.h"

class Gemometric
{
public:
	static void RenderRectange(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen);
	static void RenderOutline(const GeometricFormat& geo_size, const ColorData& color, SDL_Renderer* screen);
};


#endif