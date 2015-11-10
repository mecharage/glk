//////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe		: vect / point																		//
// Description	: Classe représentant un point ou un vecteur défini dans un espace à 2 dimensions.  //
//                      Elle définie en outre de nombreux opérateurs applicable aux vecteurs.	    //
// Version		: 2.2																				//
// Auteur(s)	: Daniel LACHAL	- Quentin DIAZ													    //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VECT_H
	#define VECT_H

	#include <glk/util.h>

namespace glk {

	/**
	 * Flottant simple précision représentant un angle en radian
	 */
	struct radian {

		inline constexpr radian(float f) : _f(f) { }

		inline constexpr operator float() const {
			return _f;
		}

		inline radian &operator =(float f) {
			_f = f;
			return *this;
		}
	private:
		float _f;
	};

	inline constexpr radian operator "" _rad(long double f) {
		return radian(f);
	}

	struct vect_norme;

	/**
	 * Représente un point ou un vecteur du plan
	 */
	struct vect {
		typedef vect point;

		/**
		 * Constructeur par défaut. Aucune initialisation de la structure.
		 */
		constexpr vect() : x(0.0f), y(0.0f) { }

		/**
		 * Construit un vecteur à partir de ses coordonées cartésiennes.
		 * @param x coordonnée x
		 * @param y coordonnée y
		 */
		constexpr vect(float x, float y) : x(x), y(y) { }
		/**
		 * Constructeur de copie de vect
		 * @param orig un vect à copier
		 */
		constexpr vect(const vect& orig) = default;

		/**
		 * Constructeur de copie à partir d'un vect_norme
		 * @param orig un vect_norme à copier
		 */
		constexpr vect(const vect_norme& orig);

		/**
		 * Opérateur d'affectation à partir d'un vecteur normé.
		 * @param other le vecteur normé de base
		 * @return une référence sur cette instance, ayant les même coordonnées que other
		 */
		vect& operator =(const vect_norme& other);

		/**
		 * Opérateur d'affectation membre à membre
		 * @param other un vecteur
		 * @return une référence sur cette instance
		 */
		vect& operator =(const vect& other) = default;

		/**
		 * Factory construisant un vecteur à partir de ses coordonnées polaire
		 * @param length longueur du vecteur (module du point)
		 * @param angle angle par rapport à l'axe x, dans le sens direct, en degré (argument du point)
		 * @return Un nouveau vecteur
		 */
		static vect lengthAngle(float length, float angle) {
			return vect(length * std::cos(angle), length * std::sin(angle));
		}

		/**
		 * Factory construisant un vecteur à partir de ses coordonnées polaire
		 * @param length longueur du vecteur (module du point)
		 * @param angle angle par rapport à l'axe x, dans le sens direct, en radian (argument du point)
		 * @return Un nouveau vecteur
		 */
		static vect lengthAngle(float length, radian angle) {
			return vect(length * std::cos(angle), length * std::sin(angle));
		}


		friend constexpr vect operator -(const vect& a);
		friend constexpr vect operator +(const vect& a, const vect& b);
		friend constexpr vect operator -(const vect& a, const vect& b);
		friend constexpr vect operator *(const vect& a, float d);
		friend constexpr vect operator *(float d, const vect& a);
		friend constexpr vect operator /(const vect& a, float d);

		/**
		 * Ajoute le vecteur other à cette instance de vect
		 * @param other le vecteur à ajouter
		 * @return une référence sur cette instance
		 */
		vect& operator +=(vect const& other);
		/**
		 * Soustrait le vecteur other à cette instance de vect
		 * @param other le vecteur à soustraire
		 * @return une référence sur cette instance
		 */
		vect& operator -=(vect const& other);
		/**
		 * Multiplie cette instance de vect par scale
		 * @param scale le facteur multiplicatif
		 * @return une référence sur cette instance
		 */
		vect& operator *=(float scale);
		/**
		 * Divise cette instance de vect par scale
		 * @param scale le ratio
		 * @return une référence sur cette instance
		 */
		vect& operator /=(float scale);

		/**
		 * Pivote cette instance de vect de 90° dans le sens direct
		 * @return une référence sur cette instance
		 */
		vect & rotate90() {
			return *this = {-y, x};
		}

		/**
		 * Construit une copie de cette instance pivotée à 90° dans le sens direct
		 * @return le vecteur orthogonal dans le sens direct
		 */
		constexpr vect rotated90() const {
			return vect(-y, x);
		}

		/**
		 * Pivote cette instance de vect de 90° dans le sens indirect
		 * @return une référence sur cette instance
		 */
		vect & rotate270() {
			return *this = {y, -x};
		}

		/**
		 * Construit une copie de cette instance pivotée à 90° dans le sens indirect
		 * @return le vecteur orthogonal dans le sens indirect
		 */
		constexpr vect rotated270() const {
			return vect(y, -x);
		}

		/**
		 * Teste l'orthogonalité de 2 vecteurs
		 * @param a un vecteur
		 * @param b un vecteur
		 * @return true si les vecteurs sont perpendiculaires
		 */
		static constexpr bool areOrtho(const vect& a, const vect& b) {
			return a.isOrthoTo(b);
		}

		/**
		 * Teste l'orthogonalité entre cette instance de vect et un autre vecteur
		 * @param other un vecteur
		 * @return true si other est perpendiculaire à cette instance
		 */
		constexpr bool isOrthoTo(const vect& other) const {
			return (x * other.x + y * other.y == 0.0f);
		}

		/**
		 * Teste la colinéarité de 2 vecteurs
		 * @param a un vecteur
		 * @param b un vecteur
		 * @return true si les vecteurs sont parallèles
		 */
		static constexpr bool areColinear(const vect& a, const vect& b) {
			return a.isColinearTo(b);
		}

		/**
		 * Teste la colinéarité entre cette instance de vect et un autre vecteur
		 * @param other un vecteur
		 * @return true si other est parallèle à cette instance
		 */
		constexpr bool isColinearTo(const vect& other) const {
			return (x * other.y == y * other.x);
		}

		/**
		 * Projette cette instance de vect sur la droite de vecteur directeur other
		 * @param other le vecteur support de la droite sur laquelle projeter
		 * @return une référence sur cette instance
		 */
		vect & projectOn(vect const& other);
		/**
		 * Calcule le projeté orthogonal de cette instance de vect sur la droite de vecteur directeur other
		 * @param other le vecteur support de la droite sur laquelle projeter
		 * @return le vecteur projeté calculé
		 */
		constexpr vect projectedOn(vect const& other) const;
		/**
		 * Projette cette instance de vect sur la droite de vecteur directeur other
		 * @param other le vecteur normé support de la droite sur laquelle projeter
		 * @return une référence sur cette instance
		 */
		vect & projectOn(vect_norme const& other);
		/**
		 * Calcule le projeté orthogonal de cette instance de vect sur la droite de vecteur directeur other
		 * @param other le vecteur normé support de la droite sur laquelle projeter
		 * @return le vecteur projeté calculé
		 */
		constexpr vect projectedOn(vect_norme const& other) const;

		/**
		 * Refléchis cette instance de vect par rapport à la droite de vecteur directeur other
		 * @param other le vecteur support de la droite axe de symétrie
		 * @return une référence sur cette instance
		 */
		vect & reflectOn(vect const& other) {
			return *this = reflectedOn(other);
		}
		/**
		 * Calcule le symétrique de cette instance de vect par rapport à la droite de vecteur directeur other
		 * @param other le vecteur support de la droite axe de symétrie
		 * @return le symétrique du vecteur
		 */
		constexpr vect reflectedOn(vect const& other) const;

		/**
		 * Refléchis cette instance de vect par rapport à la droite de vecteur directeur other
		 * @param other le vecteur normé support de la droite axe de symétrie
		 * @return une référence sur cette instance
		 */
		vect & reflectOn(vect_norme const& other) {
			return *this = reflectedOn(other);
		}
		/**
		 * Calcule le symétrique de cette instance de vect par rapport à la droite de vecteur directeur other
		 * @param other le vecteur normé support de la droite axe de symétrie
		 * @return le symétrique du vecteur
		 */
		constexpr vect reflectedOn(vect_norme const& other) const;

		/**
		 * Norme, ou longueur, du vecteur (module du point)
		 * @return le norme de cette instance de vect
		 */
		float length() const {
			return std::sqrt(lengthPow2());
		}

		/**
		 * Norme du vecteur au carré (plus rapide à calculer que length)
		 * @return la norme de cette instance de vect au carré
		 */
		constexpr float lengthPow2() const {
			return x * x + y*y;
		}

		/**
		 * Norme cette instance de vect. Le vecteur ne doit pas être nul
		 * @return une référence sur cette instance
		 */
		vect & normalize() {
			return *this /= length();
		}
		/**
		 * Calcule le vecteur normé colinéaire et de même sens à cette instance de vect (non nul)
		 * @return le vecteur normé calculé
		 */
		constexpr vect_norme normalized() const;

		/**
		 * Change la norme de cette instance en conservant sa direction
		 * @param length la nouvelle norme
		 * @return une référence sur cette instance
		 */
		vect & setLength(float newLength) {
			return *this *= newLength / length();
		}

		/**
		 * Calcule le vecteur colinéaire et de même sens à cette instance de vect dont la norme est length
		 * @param length la norme du nouveau vecteur
		 * @return le vecteur calculé
		 */
		vect withLength(float newLength) const {
			return (*this) * (newLength / length());
		}

		/**
		 * Calcule la distance entre les points passés en paramètre (ou la norme du vecteur ab)
		 * @param a un point
		 * @param b un point
		 * @return la distance entre les 2 points
		 */
		static float distance(const point& a, const point& b) {
			return (b - a).length();
		}

		/**
		 * Calcule l'angle formé par le vecteur avec l'horizontale en degré
		 * @return l'argument du vecteur en degré dans [-180, 180]
		 */
		float angle() const {
			return std::atan2(x, y);
		}

		/**
		 * Calcule l'angle formé par le vecteur avec l'horizontale en radian
		 * @return l'argument du vecteur en radian dans [-π, π]
		 */
		radian angleRad() const {
			return std::atan2(x, y);
		}

		/**
		 * Calcule l'angle orienté entre a et b, en degré
		 * @param a un vecteur
		 * @param b un vecteur
		 * @return l'angle ab orienté dans le sens direct, en degré dans [-180, 180]
		 */
		static float angleBetween(const vect& a, const vect& b);
		/**
		 * Calcule l'angle orienté entre a et b, en radian
		 * @param a un vecteur
		 * @param b un vecteur
		 * @return l'angle ab orienté dans le sens direct, en radian dans [-π, π]
		 */
		static radian angleRadBetween(const vect& a, const vect& b);

		/**
		 * Effectue une rotation de cette instance de vect
		 * @param other le vecteur normé représentant la rotation à appliquer
		 * @return une référence sur cette instance
		 */
		vect & rotate(const vect_norme& other);
		/**
		 * Calcule une rotation de cette instance de vect
		 * @param other le vecteur normé représentant la rotation à appliquer
		 * @return le vecteur pivoté
		 */
		constexpr vect rotated(const vect_norme& other) const;
		/**
		 * Effectue une rotation de cette instance de vect
		 * @param angle l'angle de rotation en degré
		 * @return une référence sur cette instance
		 */
		vect & rotate(float angle);
		/**
		 * Calcule une rotation de cette instance de vect
		 * @param angle l'angle de rotation en degré
		 * @return le vecteur pivoté
		 */
		constexpr vect rotated(float angle) const;
		/**
		 * Effectue une rotation de cette instance de vect
		 * @param angle l'angle de rotation en radian
		 * @return une référence sur cette instance
		 */
		vect & rotate(radian angle);
		/**
		 * Calcule une rotation de cette instance de vect
		 * @param angle l'angle de rotation en radian
		 * @return le vecteur pivoté
		 */
		constexpr vect rotated(radian angle) const;

		/**
		 * Effectue une transformation sur cette instance de vect
		 * @param other le vecteur représentant la transformation à appliquer
		 * @return une référence sur cette instance
		 */
		vect & transform(vect const& other) {
			return *this = {x * other.x - y * other.y, x * other.y + y * other.x};
		}

		/**
		 * Calcule une transformation de cette instance de vect
		 * @param other le vecteur représentant la transformation à appliquer
		 * @return le vecteur transformé
		 */
		constexpr vect transformed(vect const& other) const {
			return vect(x * other.x - y * other.y, x * other.y + y * other.x);
		}

		/**
		 * Assigne le vecteur nul.
		 * @return le vecteur annulé
		 */
		vect &zero() {
			x = y = 0.0f;
			return *this;
		}

		/**
		 * Opérateur de cast en tableau de flottant
		 * @return le pointeur sur ce vect sous forme de tableau de ses coordonnées
		 */
		operator float*() {
			return static_cast<float*> (static_cast<void*>(this));
		}

		/**
		 * Opérateur de cast en tableau de flottant constant
		 * @return le pointeur sur ce vect sous forme de tableau constant de ses coordonnées
		 */
		constexpr operator const float*() const {
			return static_cast<float const *> (static_cast<void const *>(this));
		}

		float x, y;
	};

	using point = vect;

	inline constexpr vect operator -(const vect& a) {
		return vect(-a.x, -a.y);
	}

	inline constexpr vect operator +(const vect& a, const vect& b) {
		return vect(a.x + b.x, a.y + b.y);
	}

	inline constexpr vect operator -(const vect& a, const vect& b) {
		return vect(a.x - b.x, a.y - b.y);
	}

	inline constexpr vect operator *(const vect& a, float d) {
		return vect(a.x * d, a.y * d);
	}

	inline constexpr vect operator *(float d, const vect& a) {
		return vect(a.x * d, a.y * d);
	}

	inline constexpr vect operator /(const vect& a, float d) {
		return vect(a.x / d, a.y / d);
	}

	inline constexpr bool operator ==(const vect& a, const vect& b) {
		return (a.x == b.x) && (a.y == b.y);
	}

	inline constexpr bool operator !=(const vect& a, const vect& b) {
		return (a.x != b.x) || (a.y != b.y);
	}

	/**
	 * Produit scalaire entre 2 vecteurs
	 */
	inline constexpr float operator *(const vect& a, const vect& b) {
		return a.x * b.x + a.y * b.y;
	}

	/**
	 * Déterminant de 2 vecteurs
	 * NB : det(a,b) = -det(b,a)
	 */
	inline constexpr float operator &(const vect& a, const vect& b) {
		return a.x * b.y - a.y * b.x;
	}

	/**
	 * Calcule le déterminant de 2 vecteurs
	 * @param a un vecteur
	 * @param b un vecteur
	 * @return le déterminant de a et b.
	 * NB : det(a,b) = -det(b,a)
	 */
	inline constexpr float det(const vect& a, const vect& b) {
		return a & b;
	}

}

#include <glk/vect.tpp>

#endif //VECT_H