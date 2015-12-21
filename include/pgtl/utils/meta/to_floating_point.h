//
//  to_floating_point.h
//  vmc
//
//  Created by Gianluca Salvato on 03/08/15.
//
//

#ifndef vmc_to_floating_point_h
#define vmc_to_floating_point_h

namespace PGTL {

	namespace meta {

		namespace tag {
			struct default_tag;
		}

		using default_tag = tag::default_tag;

		template < class T, class Tag = default_tag > struct to_floating_point;

		template < class T > struct to_floating_point
		< T, default_tag > {
			using type = PGTL::defaults::types::floating_point;
		};

	}

}


#endif
