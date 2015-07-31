//
//  WangLandau.h
//  wanglandau
//
//  Created by Gianluca Salvato on 19/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#ifndef wanglandau_WangLandau_h
#define wanglandau_WangLandau_h

#include <random>
#include <functional>

#include "../Algorithm.h"
#include "../State/HypercubicIsingState.h"

#ifdef DEBUG
#	define dmesg(A) std::cout << A << std::endl
#else
#	define dmesg(A) do {} while(0)
#endif

#ifndef mesg
#	error("You have to define a logging macro (empty macro is could be used)!")
#endif

namespace PGTL {

	namespace names { namespace Algorithm {

		/*! Name for the Wang-Landau algorithm */
		template < class System > struct WangLandau;

	} } // namespace names::Algorithm

	/*! Alias for the Wang-Landau method on an hypercubic Ising
		system with nearest neighbours interaction */
	template < class System, class Strategy = names::Algorithm::Strategies::default_strategy >
	using WangLandauAlgorithm = Algorithm<names::Algorithm::WangLandau<System>, Strategy>;


	/*! Algorithm specialization for the Wang-Landau method on an hypercubic Ising
		system with nearest neighbours interaction */
	template < class System_ > struct Algorithm
	<names::Algorithm::WangLandau<System_>, names::Algorithm::Strategies::default_strategy> {

		/*! Name of the strategy that will be used */
		using Strategy = names::Algorithm::Strategies::default_strategy;

		/*! Type for the system on which the algorithm is to act */
		using System = System_;

		/*! Type for the state of the given System type */
		using State = State<System, tags::State::NearestNeighboursInteraction>;

		/*! Type for the index in the linear representation of all spins */
		using Index = typename System::Index;

		/*! Type for the energy value */
		using Energy = typename State::Energy;

		/*! Type for probability vales */
		using Probability = double;

		/*! Type for the Density (number) of States */
		using DoS = long double;

		/*! Type for the natural logarithm of the Density (number) of States */
		using DoSLog = long double;

		/*! Type for energy values map */
		using EnergyChannels = std::vector<Energy>;

		/*! Type for of the Density (number) of States histogram */
		using DoSHistogram = std::vector<DoS>;

		/*! Type for the natural logarithm of the Density (number) of States histogram */
		using DoSLogHistogram = std::vector<DoSLog>;

		/*! Type for the histogram of visited energy values */
		using VisitedEnergiesHistogram = std::vector<size_t>;

		/*! Type for the prng with a uniform distribution of integer values */
		using UniformIntDistribution = std::uniform_int_distribution<Index>;

		/*! Type for the prng with a uniform distribution of floating-point values */
		using UniformRealDistribution = std::uniform_real_distribution<Probability>;

		/*! Type for the simulation parameters */
		struct Parameters {

			/*! Natural logarithm of DoS modification factor */
			DoSLog mf_log;

			/*! Shrinking parameter for the modification factor */
			double mf_log_shrink;

			/*! Threshold for the modification factor */
			DoSLog mf_log_threshold;

			/*! Flatness threshold */
			Probability flatness_threshold;

			/*! Validate modification factor */
			bool check_mf_log() const noexcept { return mf_log > 1; }

			/*! Validate shrinking parameter */
			bool check_mf_log_shrink() const noexcept { return mf_log_shrink > 0 && mf_log_shrink < 1; }

			/*! Validate mf_log_threshold */
			bool check_mf_log_threshold() const noexcept { return mf_log_threshold > 0 && mf_log_threshold < mf_log; }

			/*! Validate flatness threshold */
			bool check_flatness_threshold() const noexcept { return flatness_threshold > 0 && flatness_threshold < 1; }

			/*! Validate all parameters */
			bool check_all() const noexcept {
				return	(	check_mf_log()
						&&	check_mf_log_shrink()
						&&	check_mf_log_threshold()
						&&	check_flatness_threshold()	);
			}
			
		};

		/*! Basic constructor accepting only a system object and the initial modification factor */
		template < class prng >
		explicit Algorithm(System const &system_, Parameters p, prng const &g) noexcept
		: params_		(	p															)
		, state_		(	system_														)
		, energy_		(	state_.energy_levels + 1									)
		, dos_log_		(	state_.energy_levels + 1									)
		, visited_		(	state_.energy_levels + 1									)
		, random_int_	(	std::bind(UniformIntDistribution(0, system_.size - 1), g)	)
		, random_real_	(	std::bind(UniformRealDistribution(0, 1), g)					)
		, mf_log_update	(	[&] { return params_.mf_log_shrink*mf_log; }				)
		, moved_total_(0), sweeps_(0), moved_(0), accepted_total_(0)
		{
			for (Index i = 0; i < energy_.size(); i++)
				energy_[i] = state_.min_energy + 2*i*System::D;
			std::memset(dos_log_.data(), 0, dos_log.size()*sizeof(DoSLog));
			std::memset(visited_.data(), 0, visited.size()*sizeof(size_t));
		}

		/*! Single move function */
		bool move() noexcept {

				// Old energy index - retained
			Index old_idx = energy_index(state.current_energy);

				// Choose a random site and flip the local spin
			Index i = random_int_();

				// Flip the selected spin and obtain the new energy index
			Index const new_idx = energy_index(state_.flip(i));

				// Calculate acceptance probability
			Probability const p = std::exp(dos_log[old_idx] - dos_log[new_idx]);
			bool const accepted = ( p >= 1 ) || ( random_real_() < p );

				// If the new state is accepted the current energy becomes the new energy
			if (accepted) old_idx = new_idx;
				// Otherwhise flip the local spin again
			else state_.flip(i);
				// Update the visited energies histogram
			visited_[old_idx]++;
				// Update the (log of) DoS histogram
			dos_log_[old_idx] += mf_log;

				// Return true if new state was accepted, false otherwise.
			return accepted;

		}

		/*! Runs many Monte-Carlo steps without any check */
		Probability step(size_t steps) noexcept {
			unsigned long long moves = steps*state.system.size;
			for (Index n = 0; n < moves; n++) if (move()) accepted_total_++;
			moved_ += moves;

			return acceptance_ratio();
		}

		/*! Loops until the modification factor is small enough */
		Probability sweep(size_t steps_per_sweep) noexcept {
			do {
				mesg("Starting sweep with log(f) = " << mf_log << ";");

				step(steps_per_sweep);

				if (visits_are_flat()) {
					dmesg("Flatness reached after " << moved << " steps!");

					dmesg("Reinitializing histogram...");
					memset(visited_.data(), 0, visited.size()*sizeof(size_t));

					dmesg("Updating (log of) modification factor...");
					mf_log_ = mf_log_update();

					moved_total_ += moved;
					moved_ = 0;
					sweeps_++;
				}

			} while (mf_log_ > params.mf_log_threshold);

			mesg("Normalizing density of states (so that g[0] = 2)...");
			DoSLog log2 = std::log(2), normalization = dos_log[0] - log2;
			for (Index i = 1; i < dos_log.size(); i++) dos_log_[i] -= normalization;
			dos_log_[0] = log2;

			mesg("Simulation terminated after " << sweeps_ << " updates of the modifcation factor.");
			return acceptance_ratio();
		}

		/*! Check for visited energies histogram flatness */
		bool visits_are_flat() const noexcept {
			if (moved == 0) return true;

			Probability threshold = params.flatness_threshold*moved/visited.size();

			for (Index i = 0; i < state.energy_levels; i++) {
				if ((i == 1) || (i == state.system.size - 1)) continue;
				if (visited_[i] < threshold) return false;
			}
			return true;
		}

		/*! Returns the acceptance ratio up to the current state */
		Probability acceptance_ratio() const noexcept {
			return accepted_total*1.0/moved_total;
		}

		/*! Returns the histogram index for the given energy value */
		Index energy_index(Energy const &e) const noexcept {
			return (e - state.min_energy) / Energy(2 * System::D);
		}

	protected:

		/*! Current state of the system */
		State state_;

		/*! Eenergy values map */
		EnergyChannels energy_;

		/*! Natural logarithm of the Density of States */
		DoSLogHistogram dos_log_;

		/*! Visited energies histogram */
		VisitedEnergiesHistogram visited_;

		/*! Simulation parameters */
		Parameters params_;

		/*! Natural logarithm of DoS modification factor */
		DoSLog &mf_log_ = params_.mf_log;

		/*! Total number of moves */
		unsigned long long moved_total_;

		/*! Number of sweeps */
		unsigned long long sweeps_;

		/*! Number of moves */
		unsigned long long moved_;

		/*! Number of accepted moves */
		unsigned long long accepted_total_;

		/*! Pseudo-random number generator for integer values */
		std::function<Index(void)> random_int_;

		/*! Pseudo-random number generator for floating-point values */
		std::function<Probability(void)> random_real_;

	public:

		/*! Current state of the system (constant reference) */
		State const &state = state_;

		/*! Eenergy values map (const reference) */
		EnergyChannels const &energy = energy_;

		/*! Natural logarithm of the Density of States (constant reference) */
		DoSLogHistogram const &dos_log = dos_log_;

		/*! Visited energies histogram (constant reference) */
		VisitedEnergiesHistogram const &visited = visited_;

		/*! Simluation parameters (const reference) */
		Parameters const &params = params_;

		/*! Natural logarithm of DoS modification factor (constant reference) */
		DoSLog const &mf_log = mf_log_;

		/*! Updater function for the (LOGARITHM OF!!!) the DoS modification factor */
		std::function<DoSLog(void)> mf_log_update;

		/*! Total number of moves (constant reference) */
		unsigned long long const &moved_total = moved_total_;

		/*! Number of sweeps (constant reference) */
		unsigned long long const &sweeps = sweeps_;

		/*! Number of moves per sweep (constant reference) */
		unsigned long long const &moved = moved_;

		/*! Number of accepted moves (constant reference */
		unsigned long long const &accepted_total = accepted_total_;

	};

} // namespace PGTL

#undef dmesg

#endif
