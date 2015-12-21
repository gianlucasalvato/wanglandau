//
//  HypercubicIsingSystem.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_HypercubicIsingSystem_h
#define wanglandau_HypercubicIsingSystem_h

#include "HypercubicQPottsSystem.h"

namespace PGTL {

	namespace names { namespace System {

		/*! Name for the hypercubic Ising system */
		template < size_t dimensionality > struct HypercubicIsing;

	} } // namespace names::System

	/*! Alias for hypercubic Ising system */
	template < size_t dimensionality > using HypercubicIsingSystem = System<names::System::HypercubicIsing<dimensionality>>;

	/*! Type for hypercubic Ising system properties */
	template < size_t dimensionality_ > struct System
	<names::System::HypercubicIsing<dimensionality_>> :
	HypercubicQPottsSystem<dimensionality_, 2> {

		/*! Base class type */
		using Base = HypercubicQPottsSystem<dimensionality_, 2>;

		/*! Type for underlying grid structure */
		using Grid = typename Base::Grid;

		/*! Constexpr constructor */
		explicit constexpr System(typename Grid::Size edge_)
		: Base{edge_} {}

		/*! Acceptable values for spin eigenvalue */
		enum SpinEigenvalue : char { UP = 1, DOWN = -1 };

		using Base::grid;
		using Base::size;

	};

} // namespace PGTL


#endif
