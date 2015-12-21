//
//  HypercubicQPottsSystem.h
//  wanglandau
//
//  Created by Gianluca Salvato on 05/08/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_HypercubicQPottsSystem_h
#define wanglandau_HypercubicQPottsSystem_h

#include <array>

#include "../System.h"

#include "../utils/Grid/HypercubicGrid.h"

namespace PGTL {

	namespace names { namespace System {

		/*! Name for the hypercubic Ising system */
		template < size_t dimensionality, char Q > struct HypercubicQPotts;

	} } // namespace names::System

	template < size_t dimensionality, char Q > using HypercubicQPottsSystem = System<names::System::HypercubicQPotts<dimensionality, Q>>;

	template < size_t dimensionality_, char Q_ > struct System
	<names::System::HypercubicQPotts<dimensionality_, Q_>> {
		static_assert(Q_ > 0, "Q-value must be a positive integer number.");

		/*! Type for underlying grid structure */
		using Grid			= HypercubicGrid<dimensionality_>;

		/*! Type for linear indexing of spins */
		using Index			= typename Grid::Index;

		/*! Type for size of spin array */
		using Size			= typename Grid::Size;

		/*! Type for spin coordinates */
		using Coordinates	= typename Grid::Coordinates;

		/*! Dimensionality of the system */
		static constexpr Size D = dimensionality_;

		/*! List of n-dimensional coordinates */
		template < size_t n > using CoordinatesList = std::array<Coordinates, n>;

		/*! Acceptable values for spin eigenvalue */
		enum SpinEigenvalue : char { MAX = Q_, MIN = -Q_ };

		/*! Union between spin eigenvalue and char */
		using Spin = union { SpinEigenvalue name; char value; };

		/*! Constexpr constructor */
		explicit constexpr System(typename Grid::Size edge_)
		: grid	(	edge_		)
		, size	(	grid.size	)
		{}

		/*! Returns the index in a linear representation of the spin array from given coordinates */
		constexpr Index index(Coordinates const &p) const {
			return grid.linearIndex(p);
		}

		/*! Returns the coordinates from given index in the spin list */
		Coordinates const position(Index const &i) const {
			Coordinates p;
			return grid.putCoordinates(i, p);
		}

		/*! Get a list of nearest neighbours of given coordinates */
		static CoordinatesList<2*D> const nearest_neighbours(Coordinates const &p) noexcept {
			CoordinatesList<2*D> result;

			for (Index d = 0; d < D; d++) {
				Index i = 2*d;
				result[i] = result[i + 1] = p;
				result[i][d]++;
				result[i + 1][d]--;
			}

			return std::move(result);
		}

		/*! Underlying hypercubic grid */
		const Grid grid;

		/*! Total number of sites in the system */
		const Size size;

	};

} // namespace PGTL

#endif
