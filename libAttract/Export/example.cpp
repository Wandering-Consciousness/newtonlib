//STD headers
#include "stddef.h"
#include <iostream>

//project specific
#include "project.h"

//C++ headers for the lib
#include "export.hpp"
#include "test_coords.hpp"

void printStructData(const bool print) {
	if (!print)
		return;
		
	FinalAttractor test;
	printf("struct FinalAttractor size %zu \n\n", sizeof(test));

	printf("\tGID size %zu, offset %zu\n", sizeof(test.GID), offsetof(FinalAttractor, GID));
	printf("\tTID size %zu, offset %zu\n", sizeof(test.TID), offsetof(FinalAttractor, TID));
	printf("\tLID size %zu, offset %zu\n\n", sizeof(test.LID), offsetof(FinalAttractor, LID));

	printf("\tSpotType type size %zu, offset %zu\n\n", sizeof(test.type), offsetof(FinalAttractor, type));

	printf("\tx size %zu, offset %zu\n", sizeof(test.x), offsetof(FinalAttractor, x));
	printf("\ty size %zu, offset %zu\n\n", sizeof(test.y), offsetof(FinalAttractor, y));

	printf("\tCoordinate center size %zu, offset %zu\n", sizeof(test.center), offsetof(FinalAttractor, center));
	printf("\t\tLatLng point  size %zu, offset %zu\n", sizeof(test.center.point), offsetof(Coordinate, point));
	printf("\t\tDistanceBrearing bearing size %zu, offset %zu\n", sizeof(test.center.bearing), offsetof(Coordinate, bearing));
	printf("\tCARDINAL_DIRECTION side size %zu, offset %zu\n\n", sizeof(test.side), offsetof(FinalAttractor, side));

	printf("\tdistanceErr size %zu, offset %zu\n", sizeof(test.distanceErr), offsetof(FinalAttractor, distanceErr));
	printf("\tradiusM size %zu, offset %zu\n\n", sizeof(test.radiusM), offsetof(FinalAttractor, radiusM));

	printf("\tn size %zu, offset %zu\n", sizeof(test.n), offsetof(FinalAttractor, n));
	printf("\tmean size %zu, offset %zu\n\n", sizeof(test.mean), offsetof(FinalAttractor, mean));

	printf("\tRarity rarity size %zu, offset %zu\n\n", sizeof(test.rarity), offsetof(FinalAttractor, rarity));

	printf("\tpower_old size %zu, offset %zu\n", sizeof(test.power_old), offsetof(FinalAttractor, power_old));
	printf("\tpower size %zu, offset %zu\n\n", sizeof(test.power), offsetof(FinalAttractor, power));

	printf("\tz_score size %zu, offset %zu\n", sizeof(test.z_score), offsetof(FinalAttractor, z_score));
	printf("\tprobability_single size %zu, offset %zu\n\n", sizeof(test.probability_single), offsetof(FinalAttractor, probability_single));

	printf("\tintegral_score size %zu, offset %zu\n\n", sizeof(test.integral_score), offsetof(FinalAttractor, integral_score));

	printf("\tsignificance size %zu, offset %zu\n", sizeof(test.significance), offsetof(FinalAttractor, significance));
	printf("\tprobability size %zu, offset %zu\n\n", sizeof(test.probability), offsetof(FinalAttractor, probability));

	printf("------ end struct\n");
	printf("Size of radius/spotradius vars %zu\n", sizeof(double));
	printf("Size of N/No vars %zu\n", sizeof(unsigned long));
	printf("Size of seed/i vars %zu\n", sizeof(unsigned int));
	printf("Size of hexSize/bytesSize vars %zu\n\n", sizeof(unsigned long));
	printf("Size of handle/instance/attl/coordl vars %zu\n\n", sizeof(unsigned long));
	printf("Size of struct FinalAttractor* ptr %zu\n", sizeof(struct FinalAttractor*));
	printf("Size of struct struct Coordinate* ptr %zu\n\n", sizeof(struct Coordinate*));
}

int main()
{
	{		
		unsigned long handle = getHandle(); //unique handle for this process
		printStructData(false); //print structs layout

		bool bPrintCoords = false;

		for (unsigned long i = 0; i < 3; i++)
		{
			double radius = 2000.0;
			
			unsigned long N = getDotsBySpotRadius(40, radius);
			unsigned long No = getOptimizedDots(radius);
			double spotRadius = getSpotSize(No, radius);
			printf("Test Run %lu for radius %.6f, spotradius %.6f\n Calculated n=%lu, n_opt=%lu!\n", i + 1, radius, spotRadius, N, No);
			
			unsigned int seed = (unsigned int)RANDOM_SEED + i;
			unsigned long hexSize = requiredEnthropyHex(No);
			unsigned long bytesSize = requiredEnthropyBytes(No);
			printf("Seed: %u Required bytes for TRNG: %lu, hex: %lu!\n", seed, bytesSize, hexSize);
			
			//char myHexString = getFromANU(hexSize);
			//std::vector<double> coords = Math::randomHexToCircular(&myHexString, hexSize);

			//simulate QRNG poll
			std::vector<double> coords = Attractors::getPseudorandomCircularCoords(N, seed);
			
			// std::vector<double>* coordsPtr = &Attractors::TEST_COORDS_VEC;
			unsigned long instance = Attractors::initWithCoords(handle, coords);
			printf("instance: %lu handle: %lu",instance , handle);

			findAttractors(instance, SIGNIFICANCE, FILTERING_SIGNIFICANCE);
			
			std::vector<Attractors::FinalAttractorExt> attsE = Attractors::getAttractorsExt(instance, radius, Moscow, seed);
			//std::vector<FinalAttractor> atts = Attractors::getAttractors(instance, radius, Moscow, seed);
			if (bPrintCoords) {
				std::vector<Coordinate> geocoords = Attractors::getFinalCoords(instance, radius, Moscow);
				Attractors::printCoords(geocoords);
			}
			releaseEngine(instance); //free the memory
			
			printAttractors(attsE);
			//printAttractors(atts);
			
			//(void)getchar();
		}
		releaseHandle(handle); //thread safe; only releases any resouces left for handle;
	}
	// finalize(); //single process instance only or debug, releases everything !!!
	//(void)getchar();
}

