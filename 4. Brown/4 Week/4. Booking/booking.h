//
// Created by ilya on 18.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_4_WEEK_4_TASK_BOOKING_H_
#define YANDEXCPLUSPLUS_4_BROWN_4_WEEK_4_TASK_BOOKING_H_
#include <iostream>
namespace RAII {

template <typename Provider>
class Booking {
 public:
  using BookingId = typename Provider::BookingId;
  Booking(Provider* p, const BookingId& id): provider(p), booking_id(id){ }
  ~Booking() {
    if (provider != nullptr) {
      provider->CancelOrComplete(*this);
    }
  }

  Booking(const Booking&) = delete;

  Booking(Booking&& other)
      : provider(other.provider),
        booking_id(other.booking_id)
  {
    other.provider = nullptr;
  }

  Booking& operator = (const Booking&) = delete;

  Booking& operator = (Booking&& other) {
    std::swap(provider, other.provider);
    std::swap(booking_id, other.booking_id);
    return *this;
  }

  BookingId GetId() const {
    return booking_id;
  }
 private:
  Provider* provider;
  BookingId booking_id;
};
}

#endif //YANDEXCPLUSPLUS_4_BROWN_4_WEEK_4_TASK_BOOKING_H_
