#include <SDL2/SDL.h>

#include <glk/core.h>

#include <glk/OnInit.h>
#include <glk/OnExit.h>
#include <glk/error.h>

namespace glk {

	GlkState &glkState() {
		static GlkState theInstance;
		return theInstance;
	}

	void init() {
		if (glkState().initialized)
			return;

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		OnInit::initAll();

		log("GLK initialized");

		glkState().initialized = true;
	}

	void exit() {
		if (!glkState().initialized)
			return;

		OnExit::exitAll();
		SDL_Quit();

		log("GLK exited");

		glkState().initialized = false;
	}

}
