#include "test_part_d.h"
#include "transport_guide_manager.h"


int main()
{
	TestPartD();

	TransportGuideManager manager;

	manager.PerformQueries();

	return 0;
}
