//
//  mod.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_mod_h
#define wanglandau_mod_h

#include "../result_of.h"
#include <cmath>

namespace PGTL {

	namespace utils {

		namespace internal {

			/*! Constexpr mod funciton for integral types */
			template < class T1, class T2,
				class = typename std::enable_if<std::is_integral<T1>::value>::type,
				class = typename std::enable_if<std::is_integral<T2>::value>::type
			> struct mod_i {
				static constexpr typename result_of<mod_i<T1, T2>>::type
				eval(T1 lhs, T2 const rhs) {
					return (lhs = std::fmod(lhs, rhs)) < 0 ? lhs + rhs : lhs;
				}
			};

			/*! Meta function specialization for mod */
			template < class T1, class T2 > struct result_of
			<mod_i<T1, T2>> {
				using type = decltype(T1{} - T1{}/T2{});
			};

		}

		/*! Constexpr mod function */
		template < class T1, class T2 >
		constexpr typename internal::result_of<internal::mod_i<T1, T2>>::type
		mod(T1 const lhs, T2 const rhs) noexcept {
			return internal::mod_i<T1, T2>::eval(lhs, rhs);
		}

	}

} // namespace PGTL

#endif
