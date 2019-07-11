#include "test_part_e.h"
#include "Profiler.h"
#include "transport_guide_manager.h"


int main()
{
	{
		TIME_IT("Tests");
		TestPartE();
	}

	TransportGuideManager manager;

	manager.PerformQueries();

	return 0;
}
