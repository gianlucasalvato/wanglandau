//
//  defaults.h
//  vmc
//
//  Created by Gianluca Salvato on 03/08/15.
//
//

#ifndef vmc_defaults_h
#define vmc_defaults_h

namespace PGTL {

	namespace defaults {

		namespace types {

			using integer = int;

			using floating_point = double;

			using index = unsigned int;

			using index_long = unsigned long;

			using index_llong = unsigned long long;

			using complex = std::complex<floating_point>;

			using gaussian = std::complex<integer>;


		}

	}

}

#endif
