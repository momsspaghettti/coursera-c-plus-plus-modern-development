#pragma once
#include <utility>


namespace RAII
{
    template<class Provider>
    class Booking
    {
    public:
        Booking(Provider* provider, int id) : provider_(std::move(provider)), id_(id) {}

		Booking(const Booking&) = delete;
		Booking& operator=(const Booking&) = delete;

        Booking(Booking&& other) noexcept
        {
			provider_ = std::move(other.provider_);
			other.provider_ = nullptr;

			id_ = std::move(other.id_);
        }

        Booking& operator=(Booking&& other) noexcept
        {
			provider_ = std::move(other.provider_);
			other.provider_ = nullptr;

			id_ = std::move(other.id_);

			return *this;
        }

        ~Booking()
        {
            if (provider_)
			    provider_->CancelOrComplete(*this);
        }

    private:
		Provider* provider_;
		int id_;
    };
}


void TestBooking();