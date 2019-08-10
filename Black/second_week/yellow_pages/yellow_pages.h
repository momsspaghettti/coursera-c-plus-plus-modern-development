#pragma once
#include "company.pb.h"
#include "provider.pb.h"
#include "signal.pb.h"
#include <unordered_map>
#include <vector>


namespace YellowPages {
  using Signals = std::vector<Signal>;
  using Providers = std::unordered_map<uint64_t, Provider>;

  // Объединяем данные из сигналов об одной и той же организации
  Company Merge(const Signals& signals, const Providers& providers);
}


void TestYellowPages();