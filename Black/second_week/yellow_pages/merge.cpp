#include "yellow_pages.h"
#include <map>
#include <cstdint>
#include <set>
#include <tuple>
#include <memory>


namespace YellowPages {

    bool operator<(const Name& lhs, const Name& rhs) {
        if (lhs.value() == rhs.value()) {
            return lhs.type() < rhs.type();
        } else {
            return lhs.value() < rhs.value();
        }
    }

    bool operator<(const Phone& lhs, const Phone& rhs) {
        const auto lhs_t = std::tie(
                lhs.formatted(),
                lhs.country_code(),
                lhs.local_code(),
                lhs.number(),
                lhs.extension(),
                lhs.description()
        );

        const auto rhs_t = std::tie(
                rhs.formatted(),
                rhs.country_code(),
                rhs.local_code(),
                rhs.number(),
                rhs.extension(),
                rhs.description()
        );

        if (lhs_t == rhs_t) {
            return lhs.type() < rhs.type();
        } else {
            return lhs_t < rhs_t;
        }
    }

    bool operator<(const Url& lhs, const Url& rhs) {
        return lhs.value() < rhs.value();
    }


    class CompanyMerger
    {
    public:
        CompanyMerger() {
            address_max_priority = 0;
            working_time_max_priority = 0;

            priority_to_names = std::make_unique<std::map<uint64_t, std::set<Name>>>();
            priority_to_phones = std::make_unique<std::map<uint64_t, std::set<Phone>>>();
            priority_to_urls = std::make_unique<std::map<uint64_t, std::set<Url>>>();
        }

        [[nodiscard]] Company Merge(const Signals &signals, const Providers &providers) {
            Company company;

            for (const auto& signal : signals) {
                merge_address(signal, providers);
                merge_working_time(signal, providers);
                merge_names(signal, providers);
                merge_phones(signal, providers);
                merge_urls(signal, providers);
            }
            update_attrs(company);

            return company;
        }

    private:
        uint64_t address_max_priority;
        Address address{};

        uint64_t working_time_max_priority;
        WorkingTime working_time{};

        std::unique_ptr<std::map<uint64_t, std::set<Name>>> priority_to_names;
        std::unique_ptr<std::map<uint64_t, std::set<Phone>>> priority_to_phones;
        std::unique_ptr<std::map<uint64_t, std::set<Url>>> priority_to_urls;

        void merge_address(const Signal& signal, const Providers &providers) {
            if (signal.has_company()) {
                if (signal.company().has_address()) {
                    const auto priority = providers.at(signal.provider_id()).priority();
                    if (priority > address_max_priority) {
                        address_max_priority = priority;
                        address = signal.company().address();
                    }
                }
            }
        }

        void merge_names(const Signal& signal, const Providers &providers) {
            if (signal.has_company()) {
                if (!signal.company().names().empty()) {
                    for (const auto& name : signal.company().names()) {
                        (*priority_to_names)[
                                providers.at(signal.provider_id()).priority()
                        ].insert(name);
                    }
                }
            }
        }

        void merge_phones(const Signal& signal, const Providers &providers) {
            if (signal.has_company()) {
                if (!signal.company().phones().empty()) {
                    for (const auto& phone : signal.company().phones()) {
                        (*priority_to_phones)[
                                providers.at(signal.provider_id()).priority()
                        ].insert(phone);
                    }
                }
            }
        }

        void merge_urls(const Signal& signal, const Providers &providers) {
            if (signal.has_company()) {
                if (!signal.company().urls().empty()) {
                    for (const auto& url : signal.company().urls()) {
                        (*priority_to_urls)[
                                providers.at(signal.provider_id()).priority()
                        ].insert(url);
                    }
                }
            }
        }

        void merge_working_time(const Signal& signal, const Providers &providers) {
            if (signal.has_company()) {
                if (signal.company().has_working_time()) {
                    const auto priority = providers.at(signal.provider_id()).priority();

                    if (priority > working_time_max_priority) {
                        working_time_max_priority = priority;
                        working_time = signal.company().working_time();
                    }
                }
            }
        }

        void update_attrs(Company& company) {
            *company.mutable_address() = address;
            *company.mutable_working_time() = working_time;

            for (const auto& name : priority_to_names->crbegin()->second) {
                *company.mutable_names()->Add() = name;
            }

            for (const auto& phone : priority_to_phones->crbegin()->second) {
                *company.mutable_phones()->Add() = phone;
            }

            for (const auto& url : priority_to_urls->crbegin()->second) {
                *company.mutable_urls()->Add() = url;
            }
        }
    };


    Company Merge(const Signals &signals, const Providers &providers) {
        CompanyMerger merger;

        return merger.Merge(signals, providers);
    }
}