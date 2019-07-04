#include "transport_guide_tests.h"
#include "transport_guide_manager.h"


int main()
{
	TestTransportGuide();

	TransportGuideManager manager;

	manager.PerformQueries();

	return 0;
}