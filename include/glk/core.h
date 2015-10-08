#pragma once

#include <vector>

namespace glk {

	void init();

	void exit();

	struct GlkState {
		friend struct OnInit;
		friend struct OnExit;

		friend GlkState &glkState();
		friend void init();
		friend void exit();

	private:
		GlkState() = default;

		bool initialized = false;

		using FuncPtr = void (*)();
		std::vector<FuncPtr> initFunctions, exitFunctions;
	} ;

	GlkState &glkState();
}
