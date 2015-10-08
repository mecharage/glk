#include <glk/Color.h>

namespace glk {

	Color const
	Color::white	(0xffffffff),
	Color::black	(0x000000ff),
	Color::dkgray	(0x404040ff),
	Color::gray		(0x808080ff),
	Color::ltgray	(0xc0c0c0ff),
	Color::red		(0xff0000ff),
	Color::lime		(0x00ff00ff),
	Color::blue		(0x0000ffff),
	Color::yellow	(0xffff00ff),
	Color::fuchsia	(0xff00ffff),
	Color::aqua		(0x00ffffff),
	Color::orange	(0xff8000ff),
	Color::brown	(0x804000ff);

	namespace {

		float hue2rgb(float p, float q, float t) {
			if (t < 0.0f) t += 1.0f;
			if (t > 1.0f) t -= 1.0f;
			if (t < 1.0f / 6) return p + (q - p) * 6.0f * t;
			if (t < 0.5f) return q;
			if (t < 2.0f / 3) return p + (q - p) *(2.0f / 3 - t) * 6.0f;
			return p;
		}
	}

	Color Color::hsla(float h, float s, float l, float a) {
		Color col;
		float p, q;

		col.a = 0xff * a;

		if (s) {
			if (l < 0.5f)
				q = l * (s + 1.0f);
			else
				q = l + s - l * s;

			p = 2.0f * l - q;

			col.r = 0xff * hue2rgb(p, q, h + 1.0f / 3);
			col.g = 0xff * hue2rgb(p, q, h);
			col.b = 0xff * hue2rgb(p, q, h - 1.0f / 3);
		} else {
			col.r = col.g = col.b = 0xff * l;
		}
		return col;
	}
}
