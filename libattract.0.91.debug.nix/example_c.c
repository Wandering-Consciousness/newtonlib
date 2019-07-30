#include <export_h.h>

int main()
{
	{
		unsigned long long handle = getHandle(); //unique handle for this process
		for (unsigned int i = 0; i < 3; i++)
		{
			double radius = 2000.0;
			
			unsigned long long N = getDotsBySpotRadius(40, radius);
			unsigned long long No = getOptimizedDots(radius);
			double spotRadius = getSpotSize(No, radius);
			printf("Test Run %u for radius %.6f, spotradius %.6f\n Calculated n=%zu, n_opt=%zu!\n", i + 1, radius, spotRadius, N, No);
			
			unsigned int seed = (unsigned int)RANDOM_SEED + i;
			unsigned long long hexSize = requiredEnthropyHex(No);
			unsigned long long bytesSize = requiredEnthropyBytes(No);
			printf("Seed: %u Required bytes for TRNG: %zu, hex: %zu!\n", seed, bytesSize, hexSize);
			
			//char myHexString = getFromANU(hexSize);
			//std::vector<double> coords = Math::randomHexToCircular(&myHexString, hexSize);

			//simulate QRNG poll
			
			unsigned long long instance = initWithPseudo(handle, N, seed);
			findAttractors(instance, SIGNIFICANCE, FILTERING_SIGNIFICANCE);
			
			unsigned long long attl = getAttractorsLength(instance);
			struct FinalAttractor* atts = getAttractors(instance, radius, Moscow, seed);
			printAttractors(atts, attl);
			releaseAttractors(atts, attl);
						
			unsigned long long coordl = getFinalCoordsLength(instance);
			struct Coordinate* geocoords = getFinalCoords( instance, radius, Moscow);
			//printCoords(geocoords, coordl);
			releaseCoords(geocoords, coordl);

			releaseEngine(instance); //free the memory
			//(void)getchar();
		}
		releaseHandle(handle); //thread safe; only releases any resouces left for handle;
	}
	finalize(); //single process instance only or debug, relaeases everything !!!
	(void)getchar();
}
