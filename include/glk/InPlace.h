#pragma once

#include <new>
#include <utility>
#include <cassert>
#include <utility>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

namespace glk {

	/**
	 * Espace de stockage pour une instance de T, qui peut être construite et détruite à tout moment.
	 */
	template <typename T>
	struct InPlace {

		InPlace() : _constructed(false) { }
		
		//TODO
		InPlace(InPlace const &) = delete;
		InPlace(InPlace &&) = delete;
		InPlace &operator = (InPlace const &) = delete;
		InPlace &operator = (InPlace &&) = delete;
		//***
		
		~InPlace() {
			if (_constructed)
				destruct();
		}

		/**
		 * @return si le T est construit
		 */
		bool isConstructed() const {
			return _constructed;
		}

		/**
		 * Construit le T
		 * @param args paramètres transmis au constructeur de T
		 * @return le T construit
		 */
		template <typename... Args>
		T &construct(Args &&... args) {
			assert(!_constructed && "Tried to construct object twice");
			new (_storage) T(std::forward<Args>(args)...);
			_constructed = true;
			return *reinterpret_cast<T*> (_storage);
		}

		/**
		 * @return le T
		 */
		T &operator *() {
			return *operator ->();
		}

		/**
		 * @return le T
		 */
		T *operator ->() {
			assert(_constructed && "Object was not constructed yet");
			return reinterpret_cast<T*> (_storage);
		}

		/**
		 * Détruit le T
		 */
		void destruct() {
			assert(_constructed && "Tried to destruct non-constructed object");
			reinterpret_cast<T*> (_storage)->~T();
			_constructed = false;
		}
	private:
		alignas(T) char _storage[sizeof (T)];
		bool _constructed;
	};
}

#pragma GCC diagnostic pop
