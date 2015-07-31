//
//  main.cpp
//  test_HypercubicGrid
//
//  Created by Gianluca Salvato on 22/07/15.
//  Copyright (c) 2015 Gianluca Salvato. All rights reserved.
//

#include <iostream>
#include <array>
#include "../wanglandau/utils/Grid/HypercubicGrid.h"

using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {
	using Grid = HypercubicGrid<3>;

	size_t size = 3;
	Grid grid{size};
	Grid::Coordinates<> p = { -2, 2, 1 };

	cout << "Testing LinearIndex construction..." << endl << "i: ";
	Grid::Index i = grid.linearIndex(p);
	cout << i << endl;

	for (int x = -27; x < 2*27; x++) {
		grid.putCoordinates(x, p);
		cout << "i: " << x << " p: ";
		for (int d = 0; d < 3; d++) {
			cout << p[d] << " ";
		}
		cout << endl;
	}
}