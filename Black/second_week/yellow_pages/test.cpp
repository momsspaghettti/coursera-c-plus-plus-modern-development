#include "yellow_pages.h"
#include "UnitTestsFramework.h"
#include <algorithm>
#include <utility>


YellowPages::Signals GenerateSignals() {
    YellowPages::Signals signals;
    {
        // Сигнал №1 с адресом, названием и двумя телефонами
        YellowPages::Signal signal;
        signal.set_provider_id(31415);
        auto& company = *signal.mutable_company();
        {
            auto& address = *company.mutable_address();
            address.set_formatted("Brazil, Rio de Janeiro, Avenida Atlântica");
            address.mutable_coords()->set_lat(-22.969879);
            address.mutable_coords()->set_lon(-43.180382);
        }
        {
            auto& name = *company.add_names();
            name.set_value("Рога и копыта");
            name.set_type(YellowPages::Name::MAIN);
        }
        {
            auto& phone = *company.add_phones();
            phone.set_formatted("+7 (999) 123-45-67");
            phone.set_type(YellowPages::Phone::PHONE);
            phone.set_country_code("7");
            phone.set_local_code("999");
            phone.set_number("1234567");
            phone.set_description("Справочная");
        }
        {
            auto& phone = *company.add_phones();
            phone.set_formatted("+7 (555) 765-43-21");
            phone.set_type(YellowPages::Phone::FAX);
            phone.set_country_code("7");
            phone.set_local_code("555");
            phone.set_number("7654321");
        }
        signals.push_back(std::move(signal));
    }

    {
        // Сигнал №2 с названием, двумя телефонами и временем работы
        YellowPages::Signal signal;
        signal.set_provider_id(271828); // у этого поставщика будет такой же приоритет, как и в первом сигнале
        auto& company = *signal.mutable_company();
        { // такое же название, как в первом сигнале
            auto& name = *company.add_names();
            name.set_value("Рога и копыта");
            name.set_type(YellowPages::Name::MAIN);
        }
        {
            auto& phone = *company.add_phones();
            phone.set_formatted("+7 (444) 444-44-44 (доб. 44)");
            phone.set_type(YellowPages::Phone::PHONE);
            phone.set_country_code("7");
            phone.set_local_code("444");
            phone.set_number("4444444");
            phone.set_extension("44");
        }
        { // этот телефон такой же, как в первом сигнале
            auto& phone = *company.add_phones();
            phone.set_formatted("+7 (555) 765-43-21");
            phone.set_type(YellowPages::Phone::FAX);
            phone.set_country_code("7");
            phone.set_local_code("555");
            phone.set_number("7654321");
        }
        {
            auto& wt = *company.mutable_working_time();
            wt.set_formatted("По будням с 10 до 19, обед с 14 до 15");
            for (int i = 1; i != 5; ++i) {
                YellowPages::WorkingTimeInterval wti;
                wti.set_day(static_cast<YellowPages::WorkingTimeInterval::Day>(i));
                wti.set_minutes_from(10 * 60);
                wti.set_minutes_to(14 * 60);
                *wt.add_intervals() = wti;
                wti.set_minutes_from(15 * 60);
                wti.set_minutes_to(19 * 60);
                *wt.add_intervals() = wti;
            }
        }
        signals.push_back(std::move(signal));
    }

    { // Сигнал №3 с адресом, названием и двумя сайтами
        YellowPages::Signal signal;
        signal.set_provider_id(100500);  // этот поставщик будет менее приоритетным, чем два предыдущих
        auto& company = *signal.mutable_company();
        // адрес отличается от адреса первого сигнала тем, что в нем не указаны координаты
        company.mutable_address()->set_formatted("Brazil, Rio de Janeiro, Avenida Atlântica");
        {
            auto& name = *company.add_names();
            name.set_value("Horns and Hooves Ltd.");
            name.set_type(YellowPages::Name::SYNONYM);
        }
        company.add_urls()->set_value("http://www.horns-and-hooves.com/");
        company.add_urls()->set_value("https://рога-и-копыта.рф/");
        signals.push_back(std::move(signal));
    }

    return signals;
}


template <typename Message>
bool operator == (const Message& m1, const Message& m2) {
    return m1.SerializeAsString() == m2.SerializeAsString();
}


template <typename Messages, typename Message>
bool Contains(const Messages& ms, const Message& m) {
    auto it = std::find_if(
            ms.begin(), ms.end(),
            [&m](const auto& item) { return item == m; }
    );
    return it != ms.end();
}

void TestMerge() {
    auto signals = GenerateSignals();
    YellowPages::Providers providers;
    {
        auto& provider = providers[31415];
        provider.set_name("Golden Lamb");
        provider.set_priority(10);
    }
    {
        auto& provider = providers[271828];
        provider.set_name("Twelve Chairs");
        provider.set_priority(10);
    }
    {
        auto& provider = providers[100500];
        provider.set_name("Some other provider");
        provider.set_priority(3);
    }

    auto company = Merge(signals, providers);

    ASSERT(company.address() == signals[0].company().address())

    ASSERT(company.names().size() == 1)
    ASSERT(company.names(0) == signals[0].company().names(0))

    ASSERT(company.phones().size() == 3)
    ASSERT(Contains(company.phones(), signals[0].company().phones(0)))
    ASSERT(Contains(company.phones(), signals[0].company().phones(1)))
    ASSERT(Contains(company.phones(), signals[1].company().phones(0)))

    ASSERT(company.urls().size() == 2)
    ASSERT(Contains(company.urls(), signals[2].company().urls(0)))
    ASSERT(Contains(company.urls(), signals[2].company().urls(1)))

    ASSERT(company.working_time() == signals[1].company().working_time())
}


void TestYellowPages() {
    TestRunner tr;
    RUN_TEST(tr, TestMerge);
}