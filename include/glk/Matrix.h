#pragma once

#include <cstddef>
#include <array>

namespace glk {
	namespace detail {
		template <class T>
		struct Matrix {
			using type = T;
		};

		template <class T, std::size_t N>
		struct Matrix<T[N]> {
			using type = std::array<typename Matrix<T>::type, N>;
		};
	}

	template <class T>
	using Matrix = typename detail::Matrix<T>::type;
}

