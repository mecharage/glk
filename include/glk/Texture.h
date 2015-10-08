#pragma once

#include <string>

#include <SDL2/SDL_render.h>

#include <glk/UniquePtrMaker.h>
#include <glk/SurfacePtr.h>

namespace glk {

	using TexturePtr = UniquePtrFor<SDL_Texture, SDL_DestroyTexture>;

	struct Texture {

		Texture() = default;
		Texture(std::string const &fName);
		Texture(SurfacePtr const &surf);
		Texture(Texture &&) = default;

		~Texture() = default;

		Texture &operator = (Texture &&rhs) = default;

		int width() const {
			return _w;
		}

		int height() const {
			return _h;
		}

		void blit(short x, short y) const;

		void blitPart(short tx, short ty, short tw, short th, short x, short y) const {
			blitEx(tx, ty, tw, th, x, y, tw, th);
		}

		void blitEx(short tx, short ty, short tw, short th, short x, short y, short w, short h) const;

		SDL_Texture *get() {
			return _tex.get();
		}

		SDL_Texture const *get() const {
			return _tex.get();
		}

	private:

		TexturePtr _tex;
		int _w, _h;
	} ;
}