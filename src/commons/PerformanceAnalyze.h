#include <Windows.h>
#include <print>

void CountTimeWindows(const LARGE_INTEGER& freq, const LARGE_INTEGER& start, const LARGE_INTEGER& end)
{
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&start);

	// ----- Measure this section -----
	// ----- For example	      ----- 
	// for (volatile int i = 0; i < 1000000; i++);
	// --------------------------------

	//QueryPerformanceCounter(&end);

	double elapsed = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
	printf("Elapsed time: %f seconds\n", elapsed);
}