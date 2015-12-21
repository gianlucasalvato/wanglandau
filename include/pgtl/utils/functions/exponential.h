//
//  exponential.h
//  vmc
//
//  Created by Gianluca Salvato on 03/08/15.
//
//

#ifndef vmc_exponential_h
#define vmc_exponential_h

#include "../function.h"
#include "../result_of.h"
#include "../meta/to_floating_point.h"

#include <cmath>

namespace PGTL {

	namespace names {

		namespace function {

			template < class T > struct exp;

		}

	}

	namespace functions {
		template < class T >
		using exp = function<names::function::exp<T>>;
	}

	template < class T > struct function
	<names::function::exp<T>> {
		static constexpr T eval(T const &x) noexcept {
			return std::exp(x);
		}
	};

	template < class T > struct utils::result_of
	<functions::exp<T>> {
		using type = typename meta::to_floating_point<T>::type;
	};

	template < class T >
	typename utils::result_of<functions::exp<T>>::type
	exp(T const &x) noexcept {
		return functions::exp<T>::eval(x);
	}


}


#endif
