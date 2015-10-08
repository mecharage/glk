#pragma once

#include <vector>

#include <glk/core.h>
#include <glk/util.h>

namespace glk {

	struct OnInit {

		using FuncPtr = void (*)();

		template <class T>
		OnInit(T&& function) {
			glkState().initFunctions.push_back(function);
		}

	private:
		friend void init();

		static void initAll() {
			for (auto f : glkState().initFunctions)
				f();
			destroy(glkState().initFunctions);
		}
	} ;

}