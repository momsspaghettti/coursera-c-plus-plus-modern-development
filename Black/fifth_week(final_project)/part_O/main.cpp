#include <iostream>
#include <string_view>
#include "transport_guide_manager.h"


int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
        return 5;
    }

    const std::string_view mode(argv[1]);

    TransportGuideManager manager;

    if (mode == "make_base") {
        manager.PerformReadQueries();
    } else if (mode == "process_requests") {
        manager.PerformWriteQueries();
    }

    return 0;
}