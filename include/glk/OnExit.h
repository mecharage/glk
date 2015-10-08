#pragma once

#include <vector>

#include <glk/core.h>
#include <glk/util.h>

namespace glk {

	struct OnExit {

		template <class T>
		OnExit(T&& function) {
			glkState().exitFunctions.push_back(function);
		}

	private:
		friend void exit();

		static void exitAll() {
			for (auto f : glkState().exitFunctions)
				f();
			destroy(glkState().exitFunctions);
		}
	} ;

}