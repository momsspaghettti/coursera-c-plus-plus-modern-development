#include "test_part_l.h"
#include "transport_guide_manager.h"


int main() {
	TestPartL();

    TransportGuideManager manager;
    manager.PerformQueries();

	return 0;
}