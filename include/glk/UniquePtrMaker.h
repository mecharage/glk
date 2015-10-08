#pragma once

#include <memory>

namespace glk {

	namespace detail {

		template <class T>
		using Deleter = void (&)(T*);

		template <class T, Deleter<T> Del>
		struct UniquePtrFor {

			using type = std::unique_ptr<T, UniquePtrFor<T, Del>>;

			void operator ()(T *t)
			noexcept(noexcept(Del(std::declval<T*>()))) {
				Del(t);
			}
		} ;
	}

	template <class T, detail::Deleter<T> Del>
	using UniquePtrFor = typename detail::UniquePtrFor<T, Del>::type;
}
