#include "test_part_n.h"
#include "transport_guide_manager.h"


int main() {
    TestPartN();

    TransportGuideManager manager;
    manager.PerformQueries();

    return 0;
}