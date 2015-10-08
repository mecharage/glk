#pragma once

#include <cstdint>

namespace glk {

	struct Color {
		Color() = default;

		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) :
		r(r),
		g(g),
		b(b),
		a(a) { }

		Color(uint32_t rgba) :
		r(rgba >> 24),
		g(rgba >> 16),
		b(rgba >> 8),
		a(rgba) { }

		uint8_t r, g, b, a;

		Color operator ()(int alpha) const {
			return Color(r, g, b, alpha);
		}

		Color operator ()(float alpha) const {
			return Color(r, g, b, alpha * 255.0f);
		}

		static Color hsla(float h, float s, float l, float a);

		static Color const white, black, dkgray, gray, ltgray, red, lime, blue, yellow, fuchsia, aqua, orange, brown;
	};
}
