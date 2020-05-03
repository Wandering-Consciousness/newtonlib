#ifndef TEST_COORDS_HPP
#define TEST_COORDS_HPP

#include <vector>
#include <algorithm>
#include <type_traits>

//;

#include "test_coords.h"

namespace Attractors {

	//const Geometry::CartesianCoordinates TEST_COORDS;
	//constexpr auto TEST_COORDS_SIZE = std::extent<decltype(TEST_COORDS)>{};

	std::vector<double> TEST_COORDS_VEC(TEST_COORDS, TEST_COORDS + sizeof(TEST_COORDS) / sizeof(TEST_COORDS[0]));
}

#endif
