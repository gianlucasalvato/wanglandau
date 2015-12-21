//
//  function.h
//  vmc
//
//  Created by Gianluca Salvato on 03/08/15.
//
//

#ifndef vmc_function_h
#define vmc_function_h

#include "result_of.h"

namespace PGTL {

	/*! Basic function declaration */
	template < class Type > struct function;

	/*! Global namespace for named empty classes */
	namespace names {

		/*! Namespace for specialized function names */
		namespace function {

			template < class f, class _op >
			struct unary_op;

		}

		/*! Namespace for tagging specialized functions */
		namespace tag { }

	}

	template < class f, class _op > struct function
	< names::function::unary_op< f, _op > > {
		
	};

}

#endif
