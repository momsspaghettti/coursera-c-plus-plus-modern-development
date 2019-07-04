#include "bus_stops_tests.h"
#include "routes_database_tests.h"
#include "transport_guide_manager_tests.h"


void TestTransportGuide()
{
	TestBusStops();
	TestRoutes();
	TestTransportGuideManager();
}