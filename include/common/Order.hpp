#pragma once

#include <chrono>
#include <string>

#include "Types.hpp"

namespace novax::common
{

struct Order
{
    OrderId orderId = 0;

    std::string symbol;

    Side side = Side::BUY;

    OrderType type = OrderType::LIMIT;

    Price price = 0.0;

    Quantity quantity = 0;

    Quantity remainingQuantity = 0;

    OrderStatus status = OrderStatus::NEW;

    std::chrono::steady_clock::time_point timestamp;
};

} // namespace novax::common