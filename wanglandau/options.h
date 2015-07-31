#ifndef cgoOptions_h
#define cgoOptions_h

#include <getopt.h>
#include <random>
#include <iostream>
#include <cmath>

#include "wlConfig.h"

#define true (1)
#define false (!true)


extern char man_page[PATH_MAX];
extern struct _options_ cgoOptions;

using prng_seed_type = PRNG::result_type;

struct _options_ {
	struct {
		unsigned int t_mod_factor_threshold	: 1;
		unsigned int l_edge_length			: 1;
		unsigned int s_flatness_threshold	: 1;
		unsigned int f_shrink_factor		: 1;
		unsigned int r_seed					: 1;
		unsigned int g_mod_factor			: 1;
	} flags;

	double f_threshold;
	unsigned int edge;
	double flatness_threshold;
	double f_shrink;
	prng_seed_type seed;
	double f;
};

static struct _options_ cgoDefaults = {
	.flags = {
		.t_mod_factor_threshold	= false,
		.l_edge_length			= false,
		.s_flatness_threshold	= false,
		.f_shrink_factor		= false,
		.r_seed					= false,
		.g_mod_factor			= false
	},
	.f_threshold = 1 + 1e-8,
	.edge = 16,
	.flatness_threshold = 0.9,
	.f_shrink = 0.5,
	.seed = PRNG::default_seed,
	.f = std::exp(1)
};

static const char *cgoOptionsString = "t:l:s:f:r:g:";

static struct option cgoOptionsStruct[] = {
	{"mod-factor",				required_argument, 0, 'g'},
	{"shrink-factor",			required_argument, 0, 's'},
	{"mod-factor-threshold",	required_argument, 0, 't'},
	{"flatness-threshold",		required_argument, 0, 'f'},
	{"edge-length",				required_argument, 0, 'l'},
	{"seed",					required_argument, 0, 'r'},
	{NULL, 0, NULL, 0}
};

#define cgoOptionsFormat "{{%s, %s, %s, %s, %s, %s}, {%lf, %u, %lf, %lf, %lu, %lf}}"
#define cgoFlagsList																\
	(char *) ((cgoOptions.flags.t_mod_factor_threshold) ? ("True") : ("False")),		\
	(char *) ((cgoOptions.flags.l_edge_length) ? ("True") : ("False")),				\
	(char *) ((cgoOptions.flags.s_flatness_threshold) ? ("True") : ("False")),		\
	(char *) ((cgoOptions.flags.f_shrink_factor) ? ("True") : ("False")),			\
	(char *) ((cgoOptions.flags.r_seed) ? ("True") : ("False")),					\
	(char *) ((cgoOptions.flags.g_mod_factor) ? ("True") : ("False"))

#define cgoArgumentsList ,			\
	cgoOptions.f_threshold,			\
	cgoOptions.edge,				\
	cgoOptions.flatness_threshold,	\
	cgoOptions.f_shrink,			\
	cgoOptions.seed,				\
	cgoOptions.f

#define _argument &cgoOptions.
#define _flag cgoOptions.flags.

using std::cout;
using std::endl;

#pragma mark options special code

void g_mod_factor_handler() {
	auto &x = cgoOptions.f;

		//	cout << "got 'mod-factor' option." << endl;

	if (x <= 1) {
		cout << endl << "EE: value *must* be (strictly) greater than 1!" << endl;
		exit(1);
	}
}

void f_shrink_factor_handler() {
	auto &x = cgoOptions.f_shrink;

		//	cout << "got 'shrink-factor' option." << endl;

	if (x <= 0 || x >= 1) {
		cout << endl << "EE: value *must* be in ]0,1[!" << endl;
		exit(1);
	}
}

void t_mod_factor_threshold_handler() {
	auto &x = cgoOptions.f_threshold;

		//	cout << "got 'mod-factor-threshold' option." << endl;

	if (x <= 1 || x >= cgoOptions.f) {
		cout << endl << "EE: value *must* be in ]1,f[!" << endl;
		exit(1);
	}
}

void l_edge_length_handler() {
	auto &x = cgoOptions.edge;

		//	cout << "got 'edge-length' option." << endl;

	if (std::pow(x, DIMENSIONALITY) > MAX_SITES) {
		cout << endl << "EE: value is too large!" << endl;
		cout << "NOTE: this implementation supports at most " << MAX_SITES << " sites."  << endl;
		exit(1);
	}
}

void s_flatness_threshold_handler() {
	auto &x = cgoOptions.flatness_threshold;

		//	cout << "got 'flatness-threshold' option." << endl;

	if (x <= 0 || x >= 1) {
		cout << endl << "EE: value *must* be in ]0,1[!" << endl;
		exit(1);
	}
}

void r_seed_handler() {
	auto &x = cgoOptions.seed;

		//	cout << "got 'seed' option." << endl;

	if (x == 0) { x = PRNG::default_seed; }
}


#pragma mark utility


void cgoDumpUsage() {
	if (std::strcmp(man_page, "")) return;
	char command[64];
	sprintf(command, "man %s", man_page);
	system(command);
}


#pragma mark parser

void cgoParse(int argc, char * const argv[]) {
	cgoOptions = cgoDefaults;
	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, cgoOptionsString,
							cgoOptionsStruct,
							&option_index);
		if (c == -1) break;
		switch (c) {
			case 'g': _flag g_mod_factor			= true;	sscanf(optarg, "%lf",	_argument f);					g_mod_factor_handler();				break;
			case 's': _flag f_shrink_factor			= true;	sscanf(optarg, "%lf",	_argument f_shrink);			f_shrink_factor_handler();			break;
			case 't': _flag t_mod_factor_threshold	= true;	sscanf(optarg, "%lf",	_argument f_threshold);			t_mod_factor_threshold_handler();	break;
			case 'l': _flag l_edge_length			= true;	sscanf(optarg, "%u",	_argument edge);				l_edge_length_handler();			break;
			case 'f': _flag s_flatness_threshold		= true;	sscanf(optarg, "%lf",	_argument flatness_threshold);	s_flatness_threshold_handler();		break;
			case 'r': _flag r_seed					= true;	sscanf(optarg, "%llu",	_argument seed);				r_seed_handler();					break;

			case ':':	cout << argv[0] << ": option '-" << optopt << "' requires an argument\n" << endl;
			case '?':	cgoDumpUsage();
			default:	exit(EXIT_FAILURE);
		}
	}
}




#endif
