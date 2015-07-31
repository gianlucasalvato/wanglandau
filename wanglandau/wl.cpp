//
//  main.cpp
//  wanglandau
//
//  Created by Gianluca Salvato on 18/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "wlConfig.h"

#ifndef QUIET
#	define mesg(A) std::cout << A << std::endl
#else
#	define mesg(A) do {} while(0)
#endif

#include "include/Algorithm/WangLandau.h"
#include "options.h"

char man_page[PATH_MAX] = "./wl.1";
_options_ cgoOptions;

using std::cin;
using std::cout;
using std::endl;

#define stringify(A) stringify_(A)
#define stringify_(A) #A

using System = PGTL::HypercubicIsingSystem<DIMENSIONALITY>;
using State = PGTL::State<System, PGTL::tags::State::NearestNeighboursInteraction>;

using Algorithm = PGTL::WangLandauAlgorithm<System, PGTL::names::Algorithm::Strategies::default_strategy>;

/*! Simulation parameters */
struct Parameters {
	Algorithm::Parameters algorithm;
	System::Size edge;
	prng_seed_type seed;
};

/*! Prints input fed to the program */
void print_input(Parameters const &);

/*! Prints output to the program */
void print_output(Algorithm const &);

int main(int argc, char * const argv[]) {

	cgoParse(argc, argv);

	Parameters params{
		.algorithm = {	.mf_log				= std::log(cgoOptions.f),
						.mf_log_shrink		= cgoOptions.f_shrink,
						.mf_log_threshold	= std::log(cgoOptions.f_threshold),
						.flatness_threshold	= cgoOptions.flatness_threshold },
		.edge = cgoOptions.edge,
		.seed = cgoOptions.seed
	};

	PRNG g{};

	Algorithm wl{System{params.edge}, params.algorithm, g};

	print_input(params);

	wl.sweep(10000);

	print_output(wl);


	return 0;
}

/*! Prints input fed to the program */
void print_input(Parameters const &params) {
	std::ofstream input_datafile("input.txt");

	input_datafile << "-- Wang-Landau Sampling --" << endl				<< endl
	<< "system_type: "			<< "Ising"								<< endl
	<< "geometry: "				<< "Hypercubic"							<< endl
	<< "dimensionality: "		<< DIMENSIONALITY						<< endl
	<< "edge_length: "			<< params.edge							<< endl
	<< ""																<< endl
	<< "mod_factor_log(f): "	<< params.algorithm.mf_log				<< endl
	<< "f_shrink_parameter: "	<< params.algorithm.mf_log_shrink		<< endl
	<< "f_threshold_log: "		<< params.algorithm.mf_log_threshold	<< endl
	<< "flatness_threshold: "	<< params.algorithm.flatness_threshold	<< endl
	<< ""																<< endl
	<< "prng: "					<< stringify(PRNG)						<< endl
	<< "prng_seed: "			<< params.seed							<< endl;

	input_datafile.close();
}

#include <functional>
#include <limits>

/*! Prints output to the program */
void print_output(Algorithm const &a) {
	std::ofstream	dos_datafile("dos_formatted.txt"),
					log_datafile("log.txt");

		// Print basic information

	log_datafile << "-- Wang-Landau Sampling --"	<< endl	<< endl
	<< "monte_carlo_steps: "	<< a.moved_total			<< endl
	<< "monte_carlo_sweeps: "	<< a.sweeps					<< endl
	<< "accepted_steps: "		<< a.accepted_total			<< endl
	<< "acceptance_ratio: "		<< a.acceptance_ratio()		<< endl;

		// Print actual data in dos_datafile

	Algorithm::EnergyChannels const &energy = a.energy;
	Algorithm::DoSLogHistogram const &dos_log = a.dos_log;
	Algorithm::DoSHistogram dos(dos_log.size());

	auto f = [](Algorithm::DoSLog const &x) { return std::exp(x); };

	std::transform(dos_log.begin(), dos_log.end(),
				   dos.begin(), f);

	dos_datafile.precision(std::numeric_limits<Algorithm::DoS>::digits10);
	for (Algorithm::Index i = 0; i < a.dos_log.size(); i++) {
		dos_datafile	<< i			<< "\t"
						<< energy[i]	<< "\t"
						<< dos[i]		<< "\t"
						<< dos_log[i]	<< endl;
	}

	dos_datafile.close();
	log_datafile.close();
}

#undef DIMENSIONALITY
#undef MAX_SITES
#undef PRNG

#undef stringify
#undef stringify_

#undef mesg


