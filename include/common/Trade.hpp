#pragma once

#include <chrono>
#include <string>

#include "Types.hpp"

namespace novax::common
{

struct Trade
{
    OrderId buyOrderId = 0;

    OrderId sellOrderId = 0;

    std::string symbol{};

    Price price = 0;

    Quantity quantity = 0;

    std::chrono::steady_clock::time_point timestamp{};
};

}