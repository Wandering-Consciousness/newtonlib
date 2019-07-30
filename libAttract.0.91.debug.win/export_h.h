#ifndef EXPORT_H
#define EXPORT_H
// export.hpp
#pragma once

// Define EXPORTED for any platform
#if defined _WIN32 && !defined TESTAPP
# ifdef WIN_EXPORT
#   define EXPORTED  __declspec( dllexport )
# else
#   define EXPORTED  __declspec( dllimport )
# endif
#else
# define EXPORTED
#endif

	enum CARDINAL_DIRECTION
	{
		N,
		NE,
		E,
		SE,
		S,
		SW,
		W,
		NW
	};

	struct LatLng {
		double latitude;
		double longitude;
	};

	struct DistanceBearring {
		double distance;
		double initialBearing;
		double finalBearing;
	};

	struct Coordinate {
		LatLng point;
		DistanceBearring bearing; //how it was calculated
	};

	const LatLng Moscow = { 55.751244 , 37.618423 };
	const LatLng Paris = { 48.864716 , 2.349014 };
	const LatLng London = { 51.509865 , -0.118092 };
	
	//https://unix4lyfe.org/random-double/

	// double random number in the range[1, 2) needs
	// 0x3FFn nnnn nnnn nnnn enthropy
	// Where the ns are random hex digits.
	// 13 bytes of entropy are required for a random point: doubl X + double Y

	const unsigned int POINT_ENTROPY_BYTES = 13;
	const unsigned int POINT_ENTROPY_HEX = 2 * POINT_ENTROPY_BYTES; //string hex representation
	const unsigned int RANDOM_SEED = 23;

	

	//https://en.wikipedia.org/wiki/Standard_score
	//Z-score or Standart Score is an adaptation of standart deviations used to assess normally distributed data to a different probability distribution, in our case Poisson.

	//From the rules for normally distributed data for a daily event:
	//μ ± 0.5σ 	0.382924922548026 	2 in 3 	Four or five times a week
	//μ ± σ 	0.682689492137086 	1 in 3 	Twice a week
	//μ ± 1.5σ 	0.866385597462284 	1 in 7 	Weekly
	//μ ± 2σ 	0.954499736103642 	1 in 22 	Every three weeks
	//μ ± 2.5σ 	0.987580669348448 	1 in 81 	Quarterly
	//μ ± 3σ 	0.997300203936740 	1 in 370 	Yearly
	//μ ± 3.5σ 	0.999534741841929 	1 in 2149 	Every six years
	//μ ± 4σ 	0.999936657516334 	1 in 15787 	Every 43 years (twice in a lifetime)
	//μ ± 4.5σ 	0.999993204653751 	1 in 147160 	Every 403 years (once in the modern era)
	//μ ± 5σ 	0.999999426696856 	1 in 1744278 	Every 4776 years (once in recorded history)
	//μ ± 5.5σ 	0.999999962020875 	1 in 26330254 	Every 72090 years (thrice in history of modern humankind)
	//μ ± 6σ 	0.999999998026825 	1 in 506797346 	Every 1.38 million years (twice in history of humankind)
	//μ ± 6.5σ 	0.999999999919680 	1 in 12450197393 	Every 34 million years (twice since the extinction of dinosaurs)
	//μ ± 7σ 	0.999999999997440 	1 in 390682215445 	Every 1.07 billion years (four times in history of Earth)

	const unsigned long long MIN_DOTS_IN_RADIUS = 10; //used for oldstyle power calculation

	const double SIGNIFICANCE = 2.5; //significance absolute threshold for calculation, recommended value is in [2.5, 3.0]. Higher value speeds up the calculation resulting in less findings.
	const double FILTERING_SIGNIFICANCE = 4.0; //significance absolute threshold for filtering of results, recommended value is 4.0 or higher, this usualy produces 0..10 results 

	enum SpotType {
		UNDEF,
		ATTRACTOR,
		REPELLER,
		FAKE
	};

	enum Rarity {
		JUNK, //black			//3.0
		POOR, //gray			//3.5
		COMMON, //white			//4.0
		UNCOMMON, //green		//4.5
		RARE, //blue			//5.0
		EPIC, //purple			//5.5
		LEGENDARY, //orange		//6.0 
		RELIC, // light gold	//6.5
		JOKER //rainbow			//7.0
	};

	struct FinalAttractor { //struct, export variant
		// for speed sake all values are calculated for flat model, neglecting curvature
		// WGS coordinates are then applied to attractor coordinates, pertaining distances and bearing to central coordinate. 

		unsigned long long GID = 0; //global id, defined by user (for databases)
		unsigned long long TID = 0; //timestamp id
		unsigned long long LID = 0; //local id, i.e number in array

		SpotType type = SpotType(0);
		Coordinate center = {}; //center of attractor peak
		CARDINAL_DIRECTION side = CARDINAL_DIRECTION(0);

		double distanceErr = 0; //calculation error due to neglected curvature etc
		double radiusM = 0; //radius of attractor peak

		unsigned long long n = 0; //number of points
		double mean = 0; //mean average

		Rarity rarity = JUNK; //TODO: significance simplified ;)

		double power_old = 0; //oldstyle power - area taken by 10 central points of attractor/mean value for that area		
		double power = 0; //area taken by all points of attractor/mean value for the area

		double z_score = 0; // poisson z-score of single random event, if we chose this area on random and got this distribution
		long double probability_single = 0; //exact probability of the above event being random.

		double integral_score = 0; //abstract value of integral significance - how z-score varies with growth of radius, along with power characterises the density of condentsation/rarefaction

		double significance = 0; // poisson z-score of entire random event, i.e how possible is, that the event of finding this one attractor after the whole calculation was random.
		long double probability = 0; //exact probability of the above event being random.
	};
	

	// First: decide how many points are used for calculation
	// spot radius is expected minimum attractor radius, usualy the are bigger

	extern "C" EXPORTED unsigned long long getOptimizedDots(const double areaRadiusM); //how many coordinates is needed for requested radius, optimized for performance on larger areas
	extern "C" EXPORTED unsigned long long getDotsBySpotRadius(const double spotRadiusM, const double areaRadiusM); //how many dots are needed for the chosen spot size
	extern "C" EXPORTED double getSpotSize(const unsigned long long N, const double areaRadiusM); //reverse problem: what is the expected minimum attractor radius given number of dots and area

	//Second: obtain the necessary amounts of enthropy from RNG, either as raw bytes, or as hex string

	extern "C" EXPORTED unsigned long long requiredEnthropyBytes(const unsigned long long N); // N* POINT_ENTROPY_BYTES 
	extern "C" EXPORTED unsigned long long requiredEnthropyHex(const unsigned long long N); // N* POINT_ENTROPY_HEX 

	//Third: init engine, the number returned is engine index;

	//for testing init with pseudo circular coords in [0..1):
	extern "C" EXPORTED unsigned long long getHandle();
	extern "C" EXPORTED unsigned long long initWithPseudo(unsigned long long handle, const unsigned long long N, unsigned int seed);

	//for production use 
	extern "C" EXPORTED unsigned long long initWithBytes(unsigned long long handle, const unsigned char byteinput[], unsigned long long bytelength);
	extern "C" EXPORTED unsigned long long initWithHex(unsigned long long handle, const char hexstring[], unsigned long long bytelength);

	//Fourth: 

	//long long the object and do the calculation, returns engine instance
	extern "C" EXPORTED unsigned long long findAttractors(const unsigned long long engineInstance, const double significance, const double filtering_significance);

	//apply geometry fixes and get result
	extern "C" EXPORTED unsigned long long getAttractorsLength(const unsigned long long engineInstance);
	extern "C" EXPORTED struct FinalAttractor* getAttractors(const unsigned long long engineInstance, const double radiusM, const LatLng center, const unsigned long long gid);
	// free memory allocated for array:
	extern "C" EXPORTED void releaseAttractors(struct FinalAttractor* results, unsigned long long length);

	//apply geometry fixes and get all coords (optional):
	extern "C" EXPORTED unsigned long long getFinalCoordsLength(const unsigned long long engineInstance);
	extern "C" EXPORTED struct Coordinate* getFinalCoords(const unsigned long long engineInstance, const double radiusM, const LatLng center);
	// free memory allocated for array:
	extern "C" EXPORTED void releaseCoords(struct Coordinate* results, unsigned long long length);

	/* *********************** */

	//apply geometry fixes and get blindfold result
	// std::vector<FinalAttractor> getBlind(const double radiusM, const LatLng center, const unsigned long long gid); //TODO

	/* *********************** */

	//simple print:
	extern "C" EXPORTED void printAttractor(const struct FinalAttractor result);
	extern "C" EXPORTED void printAttractors(struct FinalAttractor results[], unsigned long long length);
	extern "C" EXPORTED void printCoords(struct Coordinate results[], unsigned long long length);


	//free allocated resources
	extern "C" EXPORTED void releaseEngine(const unsigned long long engineInstance); // release single instance
	extern "C" EXPORTED void releaseHandle(const unsigned long long handle); 	// this frees all engines tied to handle
	extern "C" EXPORTED void finalize(); 	// !!!CAUTION!!! this frees all engines systemwide, call before unloading the dll
	//хардресет
	

#endif
