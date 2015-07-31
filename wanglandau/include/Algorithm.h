//
//  Algorithm.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_Algorithm_h
#define wanglandau_Algorithm_h

namespace PGTL {

	/*! Basic Algorithm declaration */
	template < class System, class Strategy > struct Algorithm;

	/*! Global namespace for named empty classes */
	namespace names {

		/*! Namespace for specialized Algorithm names */
		namespace Algorithm {

			/*! Namespace for specialized Algorithm strategy names */
			namespace Strategies {

				/*! Self-explainatory default strategy */
				struct default_strategy;

			} // namespace Strategies

		}
		
	}

} // namespace PGTL
#endif
