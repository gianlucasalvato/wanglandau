//
//  System.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_System_h
#define wanglandau_System_h

namespace PGTL {

	/*! Basic System declaration */
	template < class Type > struct System;

	/*! Basic State declaration */
	template < class System, class Tag > struct State;

	/*! Global namespace for named empty classes */
	namespace names {
		
		/*! Namespace for specialized System names */
		namespace System { }

		/*! Namespace for specialized State names */
		namespace State {

			/*! Namespace for tagging specialized States */
			namespace tag { }

		}
		
	}

} // namespace PGTL

#endif
