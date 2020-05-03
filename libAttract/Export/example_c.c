//Thanks, Microsoft for continued efforts to make your own standards
//1) C11 annex K not in gcc:
//2) The MS implementation is incomplete and conforms neither to C11.annex.K nor to the original TR 24731-1
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

//STD C headers
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//project specific
#include "project.h"

//C headers for the lib
#include "test_coords.h"
#include "export_h.h"

unsigned char hex2int(const char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	return 0;
}

char* getFromANU( const unsigned long hexSize)
{
	// Simulated QRNG poll example, real ANU rng output loaded from file
	char* myHexString = malloc(hexSize);
	if (myHexString == NULL)
		return NULL;
	FILE* testFile = fopen(TEST_HEX_NAME, "r");
	if (testFile != NULL) {
		fread((void*)myHexString, sizeof(char), hexSize, testFile);
		fclose(testFile);
	}
	return myHexString;
};

unsigned char* getFromBinary(const unsigned long bytesSize ) {
	
	unsigned char* myByteString = malloc(bytesSize);
	char* myHexString = getFromANU(bytesSize*2);
	if (myByteString == NULL || myHexString == NULL)
		return NULL;
	unsigned long cnt = 0;
	for (unsigned long i = 0; i < bytesSize; i++)
	{
		myByteString[i] = hex2int(myHexString[cnt]) * 16 + hex2int(myHexString[cnt + 1]);
		cnt += 2;
	}
	free(myHexString);//deallocate	
	return myByteString;
}

void runEngine(
	const unsigned long handle, //process handle
	const unsigned long instance, //engine instance
	const unsigned long long GID, //GID
	const double radius, //radius in meters
	const double filtering_treshold, //filtering significance treshold
	const bool bPrintCoords //print coordinates?
) {
	
	printf("Engine instance: %lu handle: %lu\n", instance, handle);
	findAttractors(instance, 
		SIGNIFICANCE, //lower custom significance affects performance, handle with care
		filtering_treshold //only affects number of output results and memory consumption, low performance impact
	);
	//findAttractors(instance, SIGNIFICANCE, FILTERING_SIGNIFICANCE);

	unsigned long attl = getAttractorsLength(instance);
	struct FinalAttractor* atts = getAttractors(instance, radius, Moscow, GID);
	printAttractors(atts, attl);
	//releaseAttractors(atts);

	struct Coordinate* geocoords = getFinalCoords(instance, radius, Moscow);
	if (bPrintCoords) {
		unsigned long coordl = getFinalCoordsLength(instance);
		printCoords(geocoords, coordl);
	}
	//releaseCoords(geocoords);
	releaseEngine(instance); //free the memory

}

void printStructData(const bool print) {

	if (!print)
		return;
	FinalAttractor test;
	printf("struct FinalAttractor size %zu {\n\n", sizeof(test));

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

	printf("}\n");
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
		unsigned long long GID = 265; //your app global identifier;
		printStructData(true); //print structs layout
		
		double spotRadius = 40.0;
		double radius = 3000.0;
		unsigned long N = getDotsBySpotRadius(spotRadius, radius); //quadratic dependence on radius
		unsigned long No = getOptimizedDots(radius); //optimized for perfomance on large radiuses by mathematical criteria
		double spotRadiusOpt = getSpotSize(No, radius); //should match initial value

		bool bPrintCoords = false;
		const double custom_filtering_treshold = FILTERING_SIGNIFICANCE - 0.5; //custom filtering significance example
		
		if (false) { //init with Coords example
			unsigned long Nc = TEST_COORDS_SIZE / 2; // size for test_coords from test_coords.h, 15000 doubles means 7500 2D coords
			unsigned long instance = initWithCoords(handle, TEST_COORDS, Nc);
			runEngine(handle, instance, GID, radius, custom_filtering_treshold, bPrintCoords);
		}

		if (true) { //init with Hex example
			unsigned long th = getOptimizedDots(TEST_HEX_RADIUS); //radius for entropy.hex
			unsigned long hexSize = requiredEnthropyHex(th);
			printf("Required hex from TRNG: %lu\n", hexSize);
			char* myHexString = getFromANU(hexSize); //simulated ANU poll
			unsigned long instance = initWithHex(handle, myHexString, hexSize);
			free(myHexString); //deallocate	
			runEngine(handle, instance, GID, radius, custom_filtering_treshold, bPrintCoords);
		}

		if (false) { //init with Bytes example
			unsigned long th = getOptimizedDots(TEST_HEX_RADIUS); //radius for entropy.hex
			unsigned long bytesSize = requiredEnthropyBytes(th);
			printf("Required bytes from TRNG: %lu\n", bytesSize);
			unsigned char* myByteString = getFromBinary(bytesSize); //simulated binary TRNG poll, converts entropy.hex to bytes
			unsigned long instance = initWithBytes(handle, myByteString, bytesSize);
			free(myByteString);//deallocate
			runEngine(handle, instance, GID, radius, custom_filtering_treshold, bPrintCoords);
		}

		for (unsigned long i = 0; i < 3; i++)
		{
			printf("Test Run %lu for radius %.6f, spotradius %.6f\n Calculated n=%lu, n_opt=%lu!\n", i + 1, radius, spotRadiusOpt, N, No);

			unsigned int seed = (unsigned int)RANDOM_SEED + i;
			unsigned long hexSize = requiredEnthropyHex(No);
			unsigned long bytesSize = requiredEnthropyBytes(No);
			
			printf("Seed: %u Required bytes for TRNG: %lu, hex: %lu!\n", seed, bytesSize, hexSize);

			// Pseudo init example:
			unsigned long instance = initWithPseudo(handle, N, seed);
			runEngine(handle, instance, GID, radius, FILTERING_SIGNIFICANCE, bPrintCoords);
								
			//(void)getchar();
		}
		releaseHandle(handle); //thread safe; only releases any resouces left for handle;
	}
	// finalize(); //single process instance only or debug, releases everything !!!
	//(void)getchar();
}
