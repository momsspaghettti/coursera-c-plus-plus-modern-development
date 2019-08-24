#include "test_part_m.h"
#include "transport_guide_manager.h"


int main() {
	TestPartM();

    TransportGuideManager manager;
    manager.PerformQueries();

    return 0;
}