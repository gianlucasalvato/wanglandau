//
//  pow.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_pow_h
#define wanglandau_pow_h

#include "../result_of.h"

namespace PGTL {

	namespace utils {

		namespace internal {

			/*! Constexpr power function */
			template < size_t e, class T >
			struct pow_i {
				static constexpr T const eval(T const &b) noexcept {
					return b*pow_i<e - 1, T>::eval(b);
				}
			};

			/*! Constexpr power function */
			template < class T > struct pow_i<0, T> {
				static constexpr T const eval(T const &) noexcept { return 1; }
			};

			template < size_t e, class T > struct result_of
			<pow_i<e, T>> {
				using type = T;
			};

		} // namespace names

		/*! Constexpr power function */
		template < size_t e, class T >
		constexpr typename internal::result_of<internal::pow_i<e, T>>::type
		pow(T const b) {
			return internal::pow_i<e, T>::eval(b);
		}

	} // namespace utils

} // namespace PGTL

#endif
