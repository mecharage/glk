//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe		: vect_norme																				//
// Description	: Classe représentant un vecteur défini dans un espace à 2 dimensions dont la norme est 1.  //
//                      Elle redéfinie certaines méthodes avec une implémentation optimizée				    //
// Version		: 2.2																						//
// Auteur(s)	: Daniel LACHAL	- Quentin DIAZ													    		//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VECT_NORME_H
#define VECT_NORME_H

#include <glk/vect.h>

#include <iostream>

namespace glk {

	/**
	 * Vecteur normé sur le cercle trigonométrique.
	 * Attention : l'affectation des coordonnées est autorisée, mais ne garantie pas
	 *	que le vecteur résultant soit normé. Utilisez alors forceNormalize().
	 */
	struct vect_norme {

		/**
		 * Constructeur à partir des coordonnées cartésiennes quelquonques.
		 * La direction du vecteur est calculée à partir de ces coordonnées
		 * @param x coordonnée x
		 * @param y coordonnée y
		 */
		constexpr vect_norme(float x, float y)
			: vect_norme(x, y, std::sqrt(x * x + y * y)) { }

		/* Constructeur par défaut */

		/**
		 * Constructeur à partir de l'angle (ou argument) en degré.
		 * @param angle l'argument du vecteur. 0 (horizontal) si non spécifié.
		 */
		constexpr vect_norme(float angle = 0.0f)
			: x(std::cos(angle)), y(std::sin(angle)) { }

		/**
		 * Constructeur à partir de l'angle (ou argument) en radian.
		 * @param angle l'argument du vecteur. 0 (horizontal) si non spécifié.
		 */
		constexpr vect_norme(radian angle)
			: x(std::cos(angle)), y(std::sin(angle)) { }

		/**
		 * Constructeur à partir d'un vecteur non normé. Le nouveau vecteur pointe dans la même direction
		 * @param other le vecteur de base
		 */
		vect_norme(vect const &other)
			: vect_norme(other.x, other.y, other.length()) { }

		/**
		 * Constructeur de copie
		 * @param other le vecteur normé de base
		 */
		constexpr vect_norme(vect_norme const &other)
			: x(other.x), y(other.y) { }

	private:

		inline constexpr vect_norme(float x, float y, float l)
			: x(x / l), y(y / l) { }

		explicit inline constexpr vect_norme(float x, float y, int)
			: x(x), y(y) { }

		explicit inline constexpr vect_norme(vect const &v, int)
			: x(v.x), y(v.y) { }

	public:

		/**
		 * Opérateur d'affectation à partir d'un vecteur non normé. Le vecteur normé pointe dans la même direction
		 * @param other le vecteur de base
		 * @return une référence sur cette instance
		 */
		inline vect_norme &operator =(const vect &other) {
			return *this = other.normalized();
		}

		/**
		 * Opérateur d'affectation membre à membre
		 * @param other un vecteur normé
		 * @return une référence sur cette instance
		 */
		vect_norme &operator =(const vect_norme &other) = default;


		friend inline constexpr vect_norme operator -(const vect_norme &a);
		friend inline constexpr vect operator +(const vect_norme &a, const vect_norme &b);
		friend inline constexpr vect operator +(const vect &a, const vect_norme &b);
		friend inline constexpr vect operator +(const vect_norme &a, const vect &b);
		friend inline constexpr vect operator -(const vect_norme &a, const vect_norme &b);
		friend inline constexpr vect operator -(const vect &a, const vect_norme &b);
		friend inline constexpr vect operator -(const vect_norme &a, const vect &b);
		friend inline constexpr vect operator *(const vect_norme &a, float d);
		friend inline constexpr vect operator *(float d, const vect_norme &a);
		friend inline constexpr vect operator /(const vect_norme &a, float d);

		/**
		 * Norme, ou longueur, du vecteur, i.e. 1
		 * @return 1.f
		 */
		constexpr float length() const {
			return 1.0f;
		}

		/**
		 * Force la normalisation de ce vecteur.
		 * Utilisez par exemple après affectation de x ou y, ou de nombreuses transformations
		 * @return un référence sur cette instance
		 */
		vect_norme &forceNormalize() {
			return *this = vect(*this);
		}

		/**
		 * Pivote cette instance de vect_norme de 90° dans le sens direct
		 * @return une référence sur cette instance
		 */
		vect_norme &rotate90() {
			return *this = rotated90();
		}

		/**
		 * Construit une copie de cette instance pivotée à 90° dans le sens direct
		 * @return le vecteur orthogonal dans le sens direct
		 */
		constexpr vect_norme rotated90() const {
			return vect_norme(-y, x, 0);
		}

		/**
		 * Pivote cette instance de vect_norme de 90° dans le sens indirect
		 * @return une référence sur cette instance
		 */
		vect_norme &rotate270() {
			return *this = rotated270();
		}

		/**
		 * Construit une copie de cette instance pivotée à 90° dans le sens indirect
		 * @return le vecteur orthogonal dans le sens indirect
		 */
		constexpr vect_norme rotated270() const {
			return vect_norme(y, -x, 0);
		}

		/**
		 * Teste l'orthogonalité de 2 vecteurs
		 * @param a un vecteur normé
		 * @param b un vecteur normé
		 * @return true si les vecteurs sont perpendiculaires
		 */
		static constexpr bool areOrtho(const vect_norme &a, const vect_norme &b) {
			return a.isOrthoTo(b);
		}

		/**
		 * Teste l'orthogonalité entre cette instance de vect et un autre vecteur
		 * @param other un vecteur normé
		 * @return true si other est perpendiculaire à cette instance
		 */
		constexpr bool isOrthoTo(const vect_norme &other) const {
			return (x == other.y || y == other.x);
		}

		/**
		 * Calcule l'angle formé par le vecteur avec l'horizontale en degré
		 * @return l'argument du vecteur en degré dans [-180, 180]
		 */
		constexpr float angle() const {
			return std::atan2(x, y);
		}

		/**
		 * Calcule l'angle formé par le vecteur avec l'horizontale en radian
		 * @return l'argument du vecteur en radian dans [-π, π]
		 */
		constexpr radian angleRad() const {
			return std::atan2(x, y);
		}

		/**
		 * Teste la colinéarité de 2 vecteurs
		 * @param a un vecteur normé
		 * @param b un vecteur normé
		 * @return true si les vecteurs sont parallèles
		 */
		static constexpr bool areColinear(const vect_norme &a, const vect_norme &b) {
			return a.isColinearTo(b);
		}

		/**
		 * Teste la colinéarité entre cette instance de vect et un autre vecteur
		 * @param other un vecteur normé
		 * @return true si other est parallèle à cette instance
		 */
		constexpr bool isColinearTo(const vect_norme &other) const {
			return (x == other.x || x == -other.x);
		}

		/**
		 * Calcule l'angle orienté entre a et b, en degré
		 * @param a un vecteur normé
		 * @param b un vecteur
		 * @return l'angle ab orienté dans le sens direct, en degré dans [-180, 180]
		 */
		static float angleBetween(const vect_norme &a, const vect &b) {
			return rad2deg((det(a, b) >= 0.0f) ? std::acos((a * b) / b.length()) : -std::acos((a * b) / b.length()));
		}

		/**
		 * Calcule l'angle orienté entre a et b, en degré
		 * @param a un vecteur
		 * @param b un vecteur normé
		 * @return l'angle ab orienté dans le sens direct, en degré dans [-180, 180]
		 */
		static float angleBetween(const vect &a, const vect_norme &b) {
			return rad2deg((det(a, b) >= 0.0f) ? std::acos((a * b) / a.length()) : -std::acos((a * b) / a.length()));
		}

		/**
		 * Calcule l'angle orienté entre a et b, en degré
		 * @param a un vecteur normé
		 * @param b un vecteur normé"
		 * @return l'angle ab orienté dans le sens direct, en degré dans [-180, 180]
		 */
		static float angleBetween(const vect_norme &a, const vect_norme &b) {
			return rad2deg((det(a, b) >= 0.0f) ? std::acos(a * b) : -std::acos(a * b));
		}

		/**
		 * Calcule l'angle orienté entre a et b, en radian
		 * @param a un vecteur normé
		 * @param b un vecteur
		 * @return l'angle ab orienté dans le sens direct, en radian dans [-π, π]
		 */
		static radian angleRadBetween(const vect_norme &a, const vect &b) {
			return (det(a, b) >= 0.0f) ? std::acos((a * b) / b.length()) : -std::acos((a * b) / b.length());
		}

		/**
		 * Calcule l'angle orienté entre a et b, en radian
		 * @param a un vecteur
		 * @param b un vecteur normé
		 * @return l'angle ab orienté dans le sens direct, en radian dans [-π, π]
		 */
		static radian angleRadBetween(const vect &a, const vect_norme &b) {
			return (det(a, b) >= 0.0f) ? std::acos((a * b) / a.length()) : -std::acos((a * b) / a.length());
		}

		/**
		 * Calcule l'angle orienté entre a et b, en radian
		 * @param a un vecteur normé
		 * @param b un vecteur normé
		 * @return l'angle ab orienté dans le sens direct, en radian dans [-π, π]
		 */
		static radian angleRadBetween(const vect_norme &a, const vect_norme &b) {
			return (det(a, b) >= 0.0f) ? std::acos(a * b) : -std::acos(a * b);
		}

		/**
		 * Refléchis cette instance de vect_norme par rapport à la droite de vecteur directeur other
		 * @param other le vecteur support de la droite axe de symétrie
		 * @return une référence sur cette instance
		 */
		vect_norme &reflectOn(vect const &other) {
			return *this = reflectedOn(other);
		}

		/**
		 * Calcule le symétrique de cette instance de vect_norme par rapport à la droite de vecteur directeur other
		 * @param other le vecteur support de la droite axe de symétrie
		 * @return le symétrique du vecteur
		 */
		constexpr vect_norme reflectedOn(vect const &other) const {
			return vect_norme((2.0f * (other * *this) / other.lengthPow2()) * other - *this, 0);
		}

		/**
		 * Refléchis cette instance de vect_norme par rapport à la droite de vecteur directeur other
		 * @param other le vecteur normé support de la droite axe de symétrie
		 * @return une référence sur cette instance
		 */
		vect_norme &reflectOn(vect_norme const &other) {
			return *this = reflectedOn(other);
		}

		/**
		 * Calcule le symétrique de cette instance de vect_norme par rapport à la droite de vecteur directeur other
		 * @param other le vecteur normé support de la droite axe de symétrie
		 * @return le symétrique du vecteur
		 */
		constexpr vect_norme reflectedOn(vect_norme const &other) const {
			return vect_norme((2.0f * (other * *this)) * other - *this, 0);
		}

		/**
		 * Effectue une rotation de cette instance de vect_norme
		 * @param other le vecteur normé représentant la rotation à appliquer
		 * @return une référence sur cette instance
		 */
		vect_norme &rotate(const vect_norme &other) {
			return *this = rotated(other);
		}

		/**
		 * Calcule une rotation de cette instance de vect_norme
		 * @param other le vecteur normé représentant la rotation à appliquer
		 * @return le vecteur pivoté
		 */
		constexpr vect_norme rotated(const vect_norme &other) const {
			return vect_norme(x * other.x - y * other.y, x * other.y + y * other.x, 0);
		}

		/**
		 * Effectue une rotation de cette instance de vect_norme
		 * @param angle l'angle de rotation en degré
		 * @return une référence sur cette instance
		 */
		vect_norme &rotate(float angle) {
			return rotate(vect_norme(angle));
		}

		/**
		 * Calcule une rotation de cette instance de vect_norme
		 * @param angle l'angle de rotation en degré
		 * @return le vecteur pivoté
		 */
		constexpr vect_norme rotated(float angle) const {
			return rotated(vect_norme(angle));
		}

		/**
		 * Effectue une rotation de cette instance de vect_norme
		 * @param angle l'angle de rotation en radian
		 * @return une référence sur cette instance
		 */
		vect_norme &rotate(radian angle) {
			return rotate(vect_norme(angle));
		}

		/**
		 * Calcule une rotation de cette instance de vect_norme
		 * @param angle l'angle de rotation en radian
		 * @return le vecteur pivoté
		 */
		constexpr vect_norme rotated(radian angle) const {
			return rotated(vect_norme(angle));
		}

		/**
		 * Réciproque de cette rotation :
		 * Modifie cette instance de vect_norme de façon à prendre l'opposé de l'angle qu'elle représente (~calcul du conjugué)
		 * @return une référence sur cette instance
		 */
		vect_norme &negateAngle() {
			y = -y;
			return *this;
		}

		/**
		 * Réciproque de cette rotation :
		 * Calcule le vect_norme d'angle opposé de cette instance (~calcul du conjugué)
		 * @return le vecteur normé calculé
		 */
		vect_norme negatedAngle() const {
			return vect_norme(x, -y, 0);
		}

		/**
		 * Opérateur de cast en tableau de flottant
		 * @return le pointeur sur ce vect_norme sous forme de tableau de ses coordonnées
		 */
		operator float *() {
			return static_cast<float *> (static_cast<void *>(this));
		}

		/**
		 * Opérateur de cast en tableau de flottant constant
		 * @return le pointeur sur ce vect_norme sous forme de tableau constant de ses coordonnées
		 */
		constexpr operator const float *() const {
			return static_cast<float const *> (static_cast<void const *>(this));
		}

		/**
		 * Opérateur de cast en vect
		 * @return une référence sur cette instance
		 */
		operator vect &() {
			return reinterpret_cast<vect &> (*this);
		}

		/**
		 * Opérateur de cast en vect constant
		 * @return une référence constante sur cette instance
		 */
		constexpr operator const vect &() const {
			return *static_cast<const vect *> (static_cast<const void *>(this));
		}

		float x, y;

	};

	inline constexpr vect_norme operator -(const vect_norme &a) {
		return vect_norme(-a.x, -a.y, 0);
	}

	inline constexpr vect operator +(const vect_norme &a, const vect_norme &b) {
		return vect(a.x + b.x, a.y + b.y);
	}

	inline constexpr vect operator +(const vect &a, const vect_norme &b) {
		return vect(a.x + b.x, a.y + b.y);
	}

	inline constexpr vect operator +(const vect_norme &a, const vect &b) {
		return vect(a.x + b.x, a.y + b.y);
	}

	inline constexpr vect operator -(const vect_norme &a, const vect_norme &b) {
		return vect(a.x - b.x, a.y - b.y);
	}

	inline constexpr vect operator -(const vect &a, const vect_norme &b) {
		return vect(a.x - b.x, a.y - b.y);
	}

	inline constexpr vect operator -(const vect_norme &a, const vect &b) {
		return vect(a.x - b.x, a.y - b.y);
	}

	inline constexpr vect operator *(const vect_norme &a, float d) {
		return vect(a.x * d, a.y * d);
	}

	inline constexpr vect operator *(float d, const vect_norme &a) {
		return vect(a.x * d, a.y * d);
	}

	inline constexpr vect operator /(const vect_norme &a, float d) {
		return vect(a.x / d, a.y / d);
	}

}

#endif //VECT_NORME_H