#pragma once

#include <string>
#include <memory>
#include "transport_catalog.pb.h"


class TransportGuideManager;

class DataBaseKeeper {
public:
    DataBaseKeeper() = default;

    void SaveDataBase(const std::string& file_name, const TransportGuideManager *base);

    void LoadDataBase(const std::string& file_name);

    [[nodiscard]] const std::unique_ptr<Serializer::TransportCatalog>&
    GetDeserializedDataBase() const {
        return deserialized_catalog_;
    }

private:
    std::unique_ptr<Serializer::TransportCatalog> serialized_catalog_;
    std::unique_ptr<Serializer::TransportCatalog> deserialized_catalog_;

    void save_stops(const TransportGuideManager *base);
    void save_buses(const TransportGuideManager *base);
    void save_navigation_data_base(const TransportGuideManager *base);
};