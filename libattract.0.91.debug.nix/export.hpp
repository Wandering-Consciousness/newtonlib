#ifndef EXPORT_HPP
#define EXPORT_HPP
// export.hpp
#pragma once

#include <cstdio>
#include <vector>
#include <export_h.h>

namespace Attractors {

	//ext struct with dots comprising attracor
	struct FinalAttractorExt { //extended struct, export variant
		FinalAttractor a; //c structure
		std::vector<size_t> dots = {}; //indexes of coords comprising attractor
		std::vector<Coordinate> geoDots = {}; //same but translated
	};

		// First: decide how many points are used for calculation

		// spot radius is expected minimum attractor radius, usualy the are bigger

		// extern "C" EXPORTED unsigned long long getOptimizedDots(const double areaRadiusM); //how many coordinates is needed for requested radius, optimized for performance on larger areas
		// extern "C" EXPORTED unsigned long long getDotsBySpotRadius(const double spotRadiusM, const double areaRadiusM); //how many dots are needed for the chosen spot size
		// extern "C" EXPORTED double getSpotSize(const unsigned long long N, const double areaRadiusM); //reverse problem: what is the expected minimum attractor radius given number of dots and area

		//Second: obtain the necessary amounts of enthropy from RNG, either as raw bytes, or as hex string

		// extern "C" EXPORTED unsigned long long requiredEnthropyBytes(const unsigned long long N); // N* POINT_ENTROPY_BYTES 
		// extern "C" EXPORTED unsigned long long requiredEnthropyHex(const unsigned long long N); // N* POINT_ENTROPY_HEX 

		//Third: init engine, the number returned is engine index;

		//for testing init with pseudo circular coords in [0..1):
		// extern "C" EXPORTED unsigned long long initWithPseudo(const unsigned long long N, unsigned int seed);

		//for production use 
		// extern "C" EXPORTED unsigned long long initWithBytes(const unsigned char byteinput[], unsigned long long bytelength);
		// extern "C" EXPORTED unsigned long long initWithHex(const char hexstring[], unsigned long long bytelength);

		//C++ variant:

		//for testing use this or test_coords.hpp to get coords:
		extern "C++" EXPORTED std::vector<double> getPseudorandomCircularCoords(const unsigned long long N, unsigned int seed);
		
		//for production
		extern "C++" EXPORTED std::vector<double> randomBytesToCircular(const unsigned char byteinput[], unsigned long long bytelength);
		extern "C++" EXPORTED std::vector<double> randomHexToCircular(const char hexstring[], unsigned long long bytelength);

		//init with circular coords in [0..1):
		extern "C++" EXPORTED unsigned long long initWithCoords(unsigned long long handle, std::vector<double>& coords);

		//Fourth: 

		//long long the object and do the calculation, returns engine instance
		// extern "C" EXPORTED unsigned long long findAttractors(const unsigned long long engineInstance, const double significance, const double filtering_significance);

		//apply geometry fixes and get result
		// extern "C" EXPORTED unsigned long long getAttractors(struct FinalAttractor results[], const unsigned long long engineInstance, const double radiusM, const LatLng center, const unsigned long long gid);
		// free memory allocated for array:
		// extern "C" EXPORTED void releaseAttractors(struct FinalAttractor results[], unsigned long long length);

		//apply geometry fixes and get all coords (optional):

		// extern "C" EXPORTED unsigned long long getFinalCoords(struct Coordinate results[], const unsigned long long engineInstance, const double radiusM, const LatLng center);
		// free memory allocated for array:
		// extern "C" EXPORTED void releaseCoords(struct Coordinate results[], unsigned long long length);

		/* *********************** */

		//apply geometry fixes and get blindfold result
		// std::vector<FinalAttractor> getBlind(const double radiusM, const LatLng center, const unsigned long long gid); //TODO

		/* *********************** */

		//apply geometry fixes and get result
		extern "C++" EXPORTED std::vector<FinalAttractor> getAttractors(const unsigned long long engineInstance, const double radiusM, const LatLng center, const unsigned long long gid);
		extern "C++" EXPORTED std::vector<FinalAttractorExt> getAttractorsExt(const unsigned long long engineInstance, const double radiusM, const LatLng center, const unsigned long long gid);
		extern "C++" EXPORTED std::vector<Coordinate> getFinalCoords(const unsigned long long engineInstance, const double radiusM, const LatLng center);  //apply geodesic fixes to individual coords

		//simple print:
		// extern "C" EXPORTED void printAttractor(const struct FinalAttractor result);
		// extern "C" EXPORTED void printAttractors(struct FinalAttractor results[], unsigned long long length);
		// extern "C" EXPORTED void printCoords(struct Coordinate results[], unsigned long long length);

		//C++ variant with vectors:
		extern "C++" EXPORTED void printAttractors(std::vector<FinalAttractor>& results);
		extern "C++" EXPORTED void printAttractors(std::vector<FinalAttractorExt>& results);
		extern "C++" EXPORTED void printCoords(std::vector<Coordinate>& coords);

		//free allocated resources
		// extern "C" EXPORTED void releaseEngine(const unsigned long long engineInstance); // release single instance
		// extern "C" EXPORTED void finalize(); 	// !!!CAUTION!!! this frees all engines systemwide, call before unloading the dll
		//хардресет
}

#endif
