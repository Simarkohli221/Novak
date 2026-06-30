#pragma once

#include "../common/Order.hpp"

namespace novax::engine
{

enum class RequestType
{
    NEW_ORDER,
    CANCEL_ORDER,
    MODIFY_ORDER
};

struct OrderRequest
{
    RequestType type = RequestType::NEW_ORDER;

    novax::common::Order order{};

    novax::common::OrderId orderId = 0;

    novax::common::Price newPrice = 0;

    novax::common::Quantity newQuantity = 0;
};

} // namespace novax::engine