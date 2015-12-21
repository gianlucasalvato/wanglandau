//
//  HypercubicGrid.h
//  wanglandau
//
//  Created by Gianluca Salvato on 20/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_HypercubicGrid_h
#define wanglandau_HypercubicGrid_h

#include "../Grid.h"
#include "../functions/mod.h"
#include "../functions/pow.h"

#include <array>

namespace PGTL {

	namespace names {
		namespace Grid {
			template < size_t D > struct Hypercubic;
		}
	}

	/*! Alias for an hypercubic grid */
	template < size_t D > using HypercubicGrid = Grid<names::Grid::Hypercubic<D>>;

	#pragma mark D-dimensional grid template specialization

	template < unsigned int D_ > struct Grid
	<names::Grid::Hypercubic<D_>> {

		/*! Type for linear indexed array representation */
		using Index = long;

		/*! Type for size of coordinates array */
		using Size = size_t;

		/*! Dimensionality of the hypercubic grid */
		static constexpr Size D = D_;

		/*! Type for coordinates in the grid */
		using Coordinates = std::array<Index, D>;

	protected:
		/*! Type for super or sub grid of the same species */
		template < Size n > using Slice = Grid<names::Grid::Hypercubic<n>>;
		/*! Type for coordinates coming from super or sub grid of the same species */
		template < Size D2 = D > using NCoordinates = std::array<Index, D2>;
	public:

		/*! Constexpr constructor */
		constexpr Grid(Size edge_)
		: edge	(	edge_					)
		, base	(	utils::pow<D - 1>(edge)	)
		, size	(	base*edge				)
		{}

		/*! Returns the index in a linear representation of the grid */
		template < Size D2 >
		constexpr Index linearIndex(NCoordinates<D2> const &p) const {
			return utils::mod(p[D - 1], edge)*utils::pow<D - 1>(edge)
					+ Slice<D - 1>{edge}.linearIndex(p);
		}

		/*! Returns the coordinates of a given index of the position list */
		template < Size D2 >
		constexpr NCoordinates<D2> &putCoordinates(Index i, NCoordinates<D2> &p) const {
			return (p[D - 1] = utils::mod(i, size)/base,
					Slice<D - 1>{edge}.putCoordinates(i - base*p[D - 1], p));
		}

		/*! Linear length of each edge of the grid */
		const Size edge;
		/*! Total number of sites in a D-1 dimensional grid of the same species */
		const Size base;
		/*! Total number of sites in the grid */
		const Size size;

	};

	#pragma mark One-dimensional grid template specialization

	/**! One-dimensional grid template specialization */
	template <  > struct Grid
	<names::Grid::Hypercubic<1>> {

		/*! Type for linear indexed array representation */
		using Index = long;

		/*! Type for size of coordinates array */
		using Size = size_t;

		/*! Dimensionality of the hypercubic grid */
		static constexpr Size D = 1;

		/*! Type for coordinates in the grid */
		using Coordinates = std::array<Index, D>;

	protected:
		/*! Type for super or sub grid of the same species */
		template < Size n > using Slice = Grid<names::Grid::Hypercubic<n>>;
		/*! Type for coordinates coming from super or sub grid of the same species */
		template < Size D2 = D > using NCoordinates = std::array<Index, D2>;
	public:

		/*! Constexpr constructor */
		constexpr Grid(Size edge_)
		: edge{edge_}
		, base{1}
		, size{edge} {}

		/*! Returns the index in a linear representation of the grid */
		template < Size D2 >
		constexpr Index linearIndex(NCoordinates<D2> const &p) const {
			return utils::mod(p[D - 1], edge);
		}

		/*! Returns the coordinates of a given index of the position list */
		template < Size D2 >
		constexpr NCoordinates<D2> &putCoordinates(Index i, NCoordinates<D2> &p) const {
			return (p[D - 1] = utils::mod(i, edge), p);
		}

		/*! Linear length of each edge of the grid */
		const Size edge;
		/*! Total number of sites in a D-1 dimensional grid of the same species */
		const Size base;
		/*! Total number of sites in the one-dimensional grid */
		const Size size;
	};

} // namespace PGTL

#endif
