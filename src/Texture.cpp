#include <glk/Texture.h>

#include <string>
#include <iostream>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include <glk/UniquePtrMaker.h>
#include <glk/vect.h>
#include <glk/screen.h>

namespace glk {

	Texture::Texture(std::string const &fName) :
	Texture(SurfacePtr(IMG_Load(fName.c_str()))) {
		///LOG
		if (!_tex)
			std::clog << "Loading texture \"" << fName << "\" failed.\n";
	}

	Texture::Texture(SurfacePtr const &surf) :
	_tex(SDL_CreateTextureFromSurface(screen::getRenderer(), surf.get())),
	_w(_tex ? surf->w : 0), _h(_tex ? surf->h : 0) { }

	void Texture::blit(short x, short y) const {
		SDL_Rect const dest {
			x/* - static_cast<short> (Viewport::x())*/,
			y/* - static_cast<short> (Viewport::y())*/,
			_w, _h
		};
		SDL_RenderCopy(screen::getRenderer(), _tex.get(), nullptr, &dest);
	}

	void Texture::blitEx(short tx, short ty, short tw, short th, short x, short y, short w, short h) const {
		SDL_Rect const src {tx, ty, tw, th};
		SDL_Rect const dest {
			x/* - static_cast<short> (Viewport::x())*/,
			y/* - static_cast<short> (Viewport::y())*/,
			w, h
		};
		SDL_RenderCopy(screen::getRenderer(), _tex.get(), &src, &dest);
	}
}