#pragma once

#include <string>
#include <memory>
#include "transport_catalog.pb.h"
#include "json.h"


class TransportGuideManager;

class DataBaseKeeper {
public:
    DataBaseKeeper() = default;

    void AddDataBase(const TransportGuideManager *base);

    void AddRenderSettings(const Json::Node &configs);

    void SaveAll(const std::string &file_name);

    void LoadDataBase(const std::string &file_name);

    [[nodiscard]] const std::unique_ptr<Serializer::TransportCatalog> &
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