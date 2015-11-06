#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <vector>
#include <tuple>

#define STR(s)		DO_STR(s)
#define DO_STR(s)	#s

namespace glk {
	//	inline namespace util {

	namespace util_detail {

		template <class T>
		struct Id {
			using type = T;
		} ;
	}

	template <class T>
	using Id = typename util_detail::Id<T>::type;

#if __cplusplus >= 201402L || defined(CPP14)

	template <class T>
	std::make_signed_t<T> asSigned(T &&val) {
		return val;
	}

	template <class T>
	std::make_unsigned_t<T> asUnsigned(T &&val) {
		return val;
	}

	template <class... Ts >
	constexpr auto moveToTuple(Ts&&... ts) {
		return std::make_tuple(std::move(ts)...);
	}

	template <class Tenum>
	constexpr auto val(Tenum && enumVal) {
		return static_cast<std::underlying_type_t<std::remove_reference_t<Tenum>>>(enumVal);
	}

#else

	namespace util_detail {
		template <class Tenum>
		using UnderlyingType = typename std::underlying_type<typename std::remove_reference<Tenum>::type>::type;
	}

	template <class Tenum>
	constexpr util_detail::UnderlyingType<Tenum> val(Tenum && enumVal) {
		return static_cast<util_detail::UnderlyingType < Tenum >> (enumVal);
	}

#endif

	template <class T>
	constexpr T &lvalue(T &&v) {
		return v;
	}

	template <class T, class... Targs>
	std::array<T, sizeof...(Targs) > makeArray(Targs &&... args) {
		return std::array<T, sizeof...(Targs)>{
			std::forward<Targs>(args)...
		};
	}

	/**
	 * Égalité de deux chaînes C
	 */
	inline bool eq(char const *a, char const *b) {
		return !std::strcmp(a, b);
	}

	/**
	 * Fonction carré
	 */
	template<typename T>
	inline constexpr T sqr ( T a ) {
		return a * a;
	}

	template<typename T>
	inline constexpr T abs ( T a ) {
		return a >= T() ? a : -a;
	}

	/**
	 * @return	-1 si a < 0
	 *			0 si a == 0
	 *			1 si a > 0
	 */
	template<typename T>
	inline constexpr T sign(T a) {
		return a < 0 ? -1 : a > 0 ? 1 : a;
	}

	/**
	 * Borne val entre min et max
	 */
	template<typename T>
	inline T const &clamp(T const &val, Id<T> const &min, Id<T> const &max) {
		return val < min ? min : val > max ? max : val;
	}

	/**
	 * Fait boucler val entre min et max
	 */
	template<typename T>
	inline T loop(T const &val, Id<T> const &min, Id<T> const &max) {
		T range = T(1) + max - min;
		T ret = val;

		while (ret < min)
			ret += range;
		while (ret > max)
			ret -= range;

		return ret;
	}

	inline float stepCeil(float x, float step) {
		return std::ceil(x / step) * step;
	}

	inline float stepFloor(float x, float step) {
		return std::floor(x / step) * step;
	}

	inline constexpr float deg2rad(float angle) {
		return 0.017453293f * angle;
	}

	inline constexpr float rad2deg(float angle) {
		return 57.295779513f * angle;
	}

	template <class T>
	inline void destroy(std::vector<T> &vec) {
		std::vector<T>().swap(vec);
	}

	inline std::string dumpFile(std::string const &fName) {
		std::ifstream file(fName);
		file.exceptions(std::ios::badbit | std::ios::failbit);

		std::stringstream str;

		str << file.rdbuf();

		return str.str();
	}
	//	}
}
