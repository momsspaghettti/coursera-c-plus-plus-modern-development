#include "test_part_i.h"
#include "transport_guide_manager.h"


int main() {
	TestPartI();

	TransportGuideManager manager;
	manager.PerformQueries();

	return 0;
}