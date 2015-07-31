//
//  HypercubicIsingState.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_HypercubicIsingState_h
#define wanglandau_HypercubicIsingState_h

#include "../System.h"
#include "../System/HypercubicIsingSystem.h"

#include <vector>

namespace PGTL {

	/*! Alias for the D-dimensional square Ising system */
	template < size_t dimensionality, class Strategy >
	using HypercubicIsingState = State<HypercubicIsingSystem<dimensionality>, Strategy>;

	namespace tags { namespace State {

		/*! Tag for nearest neighbours interaction */
		struct NearestNeighboursInteraction;

	} }

	/*! State specialization for a D-dimensional square Ising system */
	template < size_t D > struct State
	<HypercubicIsingSystem<D>, tags::State::NearestNeighboursInteraction> {
		/*! Type of the base system */
		using System = HypercubicIsingSystem<D>;

		/*! Type for the index in the linear representation of all spins */
		using Index = typename System::Index;

		/*! Type for spin eigenvalue */
		using Spin = typename System::Spin;

		/*! Type for system degrees of Freedom */
		using DoF = std::vector<Spin>;

		/*! Type for the energy value */
		using Energy = long;

		/*! Type for spin coordinates */
		using Coordinates = typename System::Coordinates;

		/*! Type for nearest neighbours list */
		using NearestNeighboursList = typename System::template CoordinatesList<2*D>;

		/*! Construct from a system type */
		State(System const &system_)
		: system			(	system_										)
		, dof_				(	system_.size								)
		, spin_coupling		(	1											)
		, current_energy_	(	- spin_coupling * D * system_.size			)
		, min_energy_		(	current_energy_								)
		, energy_levels_	(	system_.grid.edge % 2				?
								system_.size - system.grid.edge		:
								system_.size								)
		{
			for (auto &spin : dof_) spin.value = System::SpinEigenvalue::UP;
		}

#pragma mark index-based accessors

		/*! Flip spin at given index */
		Energy const &flip(Index i) noexcept {
			Energy old_energy = spin_energy(i);
			dof_[i].value *= -1;
			current_energy_ -= 2*old_energy;
			return current_energy;
		}

		/*! Returns the spin value at the given index */
		Spin &spin(Index i) noexcept { return dof_[i]; }

		/*! Returns the spin value at the given index (const version) */
		Spin const &spin(Index i) const noexcept { return dof[i]; }

		Energy spin_energy(Index i) const noexcept {
			return spin_energy(system.position(i));
		}

#pragma mark position-based accessors

		/*! Flip spin at given position */
		Energy &flip(Coordinates const &p) noexcept { return flip(system.index(p)); }

		/*! Returns the spin value at the given position */
		Spin &spin(Coordinates const &p) noexcept {	return spin(system.index(p)); }

		/*! Returns the spin value at the given position (const version) */
		Spin const &spin(Coordinates const &p) const noexcept { return spin(system.index(p)); }

		/*! Returns the interaction energy of the spin at the given position */
		Energy spin_energy(Coordinates const &p) const noexcept {
			double result = 0;
			double coupling = -spin_coupling*spin(p).value;

			NearestNeighboursList const &nn = System::nearest_neighbours(p);
			for (typename System::Coordinates const &n : nn) {
				result += coupling*spin(n).value;
			}

			return result;
		}

#pragma mark functions

		/*! Returns the total energy for the current state */
		Energy energy() noexcept {
			Energy result = 0;

			for (Index i = 0; i < system.size; i++) {
				result += spin_energy(i);
			}

			return current_energy_ = 0.5*result;
		}

		/*! Underlying system representation (constant reference) */
		System const &system;

		/*! Degrees of freedom (constant reference) */
		DoF const &dof = dof_;

		/*! Spin-Spin coupling constant */
		Energy spin_coupling;

		/*! Current energy of the system (constant reference) */
		Energy const &current_energy = current_energy_;

		/*! Number of energy levels of the system (constant reference) */
		Index const &energy_levels = energy_levels_;

		/*! Minimum energy of the system (constant reference) */
		Energy const &min_energy = min_energy_;

	protected:
		/*! Degrees of freedom */
		DoF dof_;

		/*! Current energy of the system */
		Energy current_energy_;

		/*! Number of energy levels of the system */
		Index energy_levels_;

		/*! Minimum energy of the system */
		Energy min_energy_;
	};

	/*! Formatted printing on generic output stream */
	template < size_t D, class Strategy, class ostream > ostream &
	operator<<(ostream &cout, HypercubicIsingState<D, Strategy> const &state) noexcept {
		using System = typename HypercubicIsingState<D, Strategy>::System;
		using Spin = typename System::SpinEigenvalue;
		using Index = typename System::Size;

		for (Index i = 0; i < state.system.size; i++) {
				// Print a new line if an edge is reached
			if (i % state.system.grid.edge == 0) std::cout << std::endl;
			cout << (state.dof[i].name == Spin::UP ? "+ " : "- ");
		}

		return cout;
	}

} // namespace PGTL

#endif
