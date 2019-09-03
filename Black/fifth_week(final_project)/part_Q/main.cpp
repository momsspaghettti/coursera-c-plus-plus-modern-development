#include <iostream>
#include <string_view>
#include "transport_guide_manager.h"


int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
        return 5;
    }

    const std::string_view mode(argv[1]);

    if (mode == "make_base") {
        TransportGuideManager::PerformReadQueries();
    } else if (mode == "process_requests") {
        TransportGuideManager manager;
        manager.PerformWriteQueries();
    }

    return 0;
}