#pragma once

#include <cstdint>

namespace novax::common
{

using OrderId = uint64_t;
using Price = int64_t;
using Quantity = uint32_t;

enum class Side : uint8_t
{
    BUY,
    SELL
};

enum class OrderType : uint8_t
{
    LIMIT,
    MARKET
};

enum class OrderStatus : uint8_t
{
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED,
    REJECTED
};

enum class CommandType : uint8_t
{
    NEW_ORDER,
    CANCEL_ORDER,
    MODIFY_ORDER,
    SHUTDOWN
};

}