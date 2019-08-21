#include "test_part_k.h"
#include "transport_guide_manager.h"


int main() {
	TestPartK();

    TransportGuideManager manager;
    manager.PerformQueries();

	return 0;
}