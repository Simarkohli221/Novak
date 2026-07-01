#pragma once

#include <functional>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

#include "../common/Order.hpp"
#include "../common/Trade.hpp"

namespace novax::orderbook
{

using novax::common::Order;
using novax::common::Trade;
using novax::common::OrderId;
using novax::common::Price;
using novax::common::Quantity;
using novax::common::Side;
using novax::common::OrderStatus;

struct OrderLocation
{
    Price price;

    Side side;

    std::list<Order>::iterator orderIterator;
};

class OrderBook
{
public:

    OrderBook() = default;

    std::vector<Trade> addOrder(Order order);

    bool cancelOrder(OrderId orderId);

    std::vector<Trade> modifyOrder(
        OrderId orderId,
        Price newPrice,
        Quantity newQuantity
    );

    Price getBestBid() const;

    Price getBestAsk() const;

    bool empty() const;

    void printBook() const;

private:

    Trade createTrade(
        const Order& buyOrder,
        const Order& sellOrder,
        Quantity executedQuantity,
        Price executionPrice
    );

    std::vector<Trade> matchBuyOrder(Order& order);

    std::vector<Trade> matchSellOrder(Order& order);

    void addRestingOrder(Order&& order);

private:

    std::map<
        Price,
        std::list<Order>,
        std::greater<Price>
    > bids_;

    std::map<
        Price,
        std::list<Order>
    > asks_;

    std::unordered_map<
        OrderId,
        OrderLocation
    > orderIndex_;
};

} // namespace novax::orderbook