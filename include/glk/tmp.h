#pragma once

#include <type_traits>

namespace glk {
	namespace tmp {
		template <class T>
		struct type_is {
			using type = T;
		};

		template <class T>
		struct tag : type_is<T> {};

		template <class...>
		struct pack;

		template <class T, class... Us>
		struct type_in;

		template <class T, class... Us>
		struct type_in<T, T, Us...> : std::true_type {};

		template <class T>
		struct type_in<T> : std::false_type {};

		template <class T, class U, class... Us>
		struct type_in<T, U, Us...> : type_in<T, Us...> {};
	}
}
