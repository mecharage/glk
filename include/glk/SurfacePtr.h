#pragma once

#include <SDL2/SDL_surface.h>

#include <glk/UniquePtrMaker.h>

namespace glk {
	using SurfacePtr = UniquePtrFor<SDL_Surface, SDL_FreeSurface>;
}
