#include <glk/vect.h>
#include <glk/vect_norme.h>

namespace glk {

	inline constexpr vect::vect(const vect_norme &orig)
		: x(orig.x), y(orig.y) { }

	inline vect &vect::operator =(const vect_norme &other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	inline vect &vect::operator +=(const vect &other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	inline vect &vect::operator -=(const vect &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline vect &vect::operator *=(float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}

	inline vect &vect::operator /=(float scale) {
		x /= scale;
		y /= scale;
		return *this;
	}

	inline vect &vect::projectOn(vect const &other) {
		x = y = (other * (*this)) / other.lengthPow2();
		x *= other.x;
		y *= other.y;
		return *this;
	}

	inline constexpr vect vect::projectedOn(vect const &other) const {
		return ((other * (*this)) / other.lengthPow2()) * other;
	}

	inline vect &vect::projectOn(vect_norme const &other) {
		x = y = other * (*this);
		x *= other.x;
		y *= other.y;
		return *this;
	}

	inline constexpr vect vect::projectedOn(const vect_norme &other) const {
		return (other * (*this)) * other;
	}

	inline constexpr vect vect::reflectedOn(vect const &other) const {
		return (2.0f * (other * (*this)) / other.lengthPow2()) * other - *this;
	}

	inline constexpr vect vect::reflectedOn(vect_norme const &other) const {
		return (2.0f * (other * (*this))) * other - *this;
	}

	inline vect_norme vect::normalized() const {
		return (vect_norme) * this;
	}

	/* FIXIT : Est-ce que c'est optimisé par le compilateur, comme ça ? */
	inline float vect::angleBetween(const vect &a, const vect &b) {
		return (((det(a, b) >= 0.0f) ? 1.0f : -1.0f) *
			rad2deg(std::acos(
				/*cos(A^B) = */ (a * b) / std::sqrt(a.lengthPow2() * b.lengthPow2())
			))
		);
	}

	inline radian vect::angleRadBetween(const vect &a, const vect &b) {
		return (((det(a, b) >= 0.0f) ? 1.0f : -1.0f) *
			std::acos(
				/*cos(A^B) = */ (a * b) / std::sqrt(a.lengthPow2() * b.lengthPow2())
			)
		);
	}

	inline vect &vect::rotate(const vect_norme &other) {
		return transform(other);
	}

	inline constexpr vect vect::rotated(const vect_norme &other) const {
		return transformed(other);
	}

	inline vect &vect::rotate(float angle) {
		return rotate(vect_norme(angle));
	}

	inline constexpr vect vect::rotated(float angle) const {
		return rotated(vect_norme(angle));
	}

	inline vect &vect::rotate(const radian angle) {
		return rotate(vect_norme(angle));
	}

	inline constexpr vect vect::rotated(const radian angle) const {
		return rotated(vect_norme(angle));
	}

}
