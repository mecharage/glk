#include <glk/screen.h>

#include <cassert>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_hints.h>

#include <glk/OnInit.h>
#include <glk/OnExit.h>
#include <glk/Color.h>
#include <glk/error.h>

namespace glk {
	namespace screen {
		namespace {
			SDL_Window *_theWindow = nullptr;
			SDL_Renderer *_theRenderer = nullptr;
			SDL_GLContext _theGlContext = nullptr;
			int _width = 800, _height = 600, _scale = 1;

			OnInit openWindow = [] {
				auto const &ls = LogSection("Opening window");

				_theWindow = SDL_CreateWindow(
					"glk Window",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					_width * _scale,
					_height * _scale,
					SDL_WINDOW_OPENGL
					);

				log("Window open");

				SDL_ShowWindow(_theWindow);
			};

			OnExit closeWindow = [] {
				SDL_GL_DeleteContext(_theGlContext);
				SDL_DestroyRenderer(_theRenderer);
				SDL_DestroyWindow(_theWindow);
			};
		}

		void setSize(int width, int height, int scale) {
			assert(((void) "You must call Screen::setSize() before initializing glk !", !_theWindow));
			_width = width;
			_height = height;
			_scale = scale;
		}

		SDL_Window *getWindow() {
			return _theWindow;
		}

		SDL_Renderer *getRenderer() {
			assert(_theWindow && "You must open the window before using the renderer.");

			if (!_theRenderer) {
				_theRenderer = SDL_CreateRenderer(_theWindow, -1, SDL_RENDERER_ACCELERATED);
				SDL_RenderSetLogicalSize(_theRenderer, _width, _height);
				log("Renderer created");
			}

			return _theRenderer;
		}

		SDL_GLContext getGlContext() {
			assert(_theWindow && "You must open the window before using the OpenGL context.");

			if (!_theGlContext) {
				_theGlContext = SDL_GL_CreateContext(_theWindow);
				log("OpenGL context created");
			}

			return _theGlContext;
		}

		int width() {
			return _width;
		}

		int height() {
			return _height;
		}

		int scale() {
			return _scale;
		}

		void setRendererColor(Color col) {
			SDL_SetRenderDrawColor(_theRenderer, col.r, col.g, col.b, col.a);
		}
	}
}
