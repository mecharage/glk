#pragma once

#include <SDL2/SDL_video.h>

#include <glk/Color.h>

struct SDL_Window;
struct SDL_Renderer;
typedef void *SDL_GLContext;

namespace glk {
	namespace screen {
		SDL_Window *getWindow();
		SDL_Renderer *getRenderer();
		SDL_GLContext getGlContext();

		void setSize(int w, int h, int scale = 1);

		int width();
		int height();
		int scale();

		void beginFrame();
		void endFrame();

		void setRendererColor(Color col);
	}
}
