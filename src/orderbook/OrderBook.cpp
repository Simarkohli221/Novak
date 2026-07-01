#include "../../include/orderbook/OrderBook.hpp"
#include <iterator>
#include <algorithm>
#include <chrono>
#include <iostream>

namespace novax::orderbook
{

bool OrderBook::empty() const
{
    return bids_.empty() && asks_.empty();
}
bool OrderBook::cancelOrder(OrderId orderId)
{
    auto indexIt = orderIndex_.find(orderId);

    if (indexIt == orderIndex_.end())
    {
        return false;
    }

    const OrderLocation& location = indexIt->second;

    if (location.side == Side::BUY)
    {
        auto bidLevel = bids_.find(location.price);

        if (bidLevel != bids_.end())
        {
            bidLevel->second.erase(location.orderIterator);

            if (bidLevel->second.empty())
            {
                bids_.erase(bidLevel);
            }
        }
    }
    else
    {
        auto askLevel = asks_.find(location.price);

        if (askLevel != asks_.end())
        {
            askLevel->second.erase(location.orderIterator);

            if (askLevel->second.empty())
            {
                asks_.erase(askLevel);
            }
        }
    }

    orderIndex_.erase(indexIt);

    return true;
}

void OrderBook::printBook() const
{
    std::cout
        << "\n=========================================\n";

    std::cout
        << "              ORDER BOOK\n";

    std::cout
        << "=========================================\n\n";

    std::cout
        << "SELL\n";

    std::cout
        << "Price\tQuantity\n";

    for (const auto& [price, orders] : asks_)
    {
        Quantity totalQuantity = 0;

        for (const auto& order : orders)
        {
            totalQuantity += order.remainingQuantity;
        }

        std::cout
            << price
            << '\t'
            << totalQuantity
            << '\n';
    }

    std::cout
        << "\n-----------------------------------------\n\n";

    std::cout
        << "BUY\n";

    std::cout
        << "Price\tQuantity\n";

    for (const auto& [price, orders] : bids_)
    {
        Quantity totalQuantity = 0;

        for (const auto& order : orders)
        {
            totalQuantity += order.remainingQuantity;
        }

        std::cout
            << price
            << '\t'
            << totalQuantity
            << '\n';
    }

    std::cout
        << "\n=========================================\n";
}
void OrderBook::addRestingOrder(Order&& order)
{
    if (order.remainingQuantity == 0)
{
    return;
}
     order.status = OrderStatus::NEW;
    if (order.side == Side::BUY)
    {
        auto levelIt = bids_.find(order.price);

        if (levelIt == bids_.end())
        {
            levelIt = bids_.emplace(order.price, std::list<Order>{}).first;
        }

        levelIt->second.push_back(std::move(order));

        auto orderIt = std::prev(levelIt->second.end());

       orderIndex_[orderIt->orderId] =
    {
    orderIt->price,
    Side::BUY,
    orderIt
    };
    }
    else
    {
        auto levelIt = asks_.find(order.price);

        if (levelIt == asks_.end())
        {
            levelIt = asks_.emplace(order.price, std::list<Order>{}).first;
        }

        levelIt->second.push_back(std::move(order));

        auto orderIt = std::prev(levelIt->second.end());

       orderIndex_[orderIt->orderId] =
    {
    orderIt->price,
    Side::SELL,
    orderIt
    };
    }
}
std::vector<Trade> OrderBook::matchBuyOrder(Order& order)
{
    std::vector<Trade> trades;

    while (order.remainingQuantity > 0 && !asks_.empty())
    {
        auto bestAskLevel = asks_.begin();

        auto& restingOrders = bestAskLevel->second;

        auto restingOrder = restingOrders.begin();

        if (restingOrder->price > order.price)
        {
            break;
        }

        Quantity executedQuantity =
            std::min(
                order.remainingQuantity,
                restingOrder->remainingQuantity
            );

        trades.push_back(
            createTrade(
                order,
                *restingOrder,
                executedQuantity,
                restingOrder->price
            )
        );

        order.remainingQuantity -= executedQuantity;

        restingOrder->remainingQuantity -= executedQuantity;

        order.status =
            (order.remainingQuantity == 0)
                ? OrderStatus::FILLED
                : OrderStatus::PARTIALLY_FILLED;

        restingOrder->status =
            (restingOrder->remainingQuantity == 0)
                ? OrderStatus::FILLED
                : OrderStatus::PARTIALLY_FILLED;

        if (restingOrder->remainingQuantity == 0)
        {
            orderIndex_.erase(restingOrder->orderId);

            restingOrders.erase(restingOrder);

            if (restingOrders.empty())
            {
                asks_.erase(bestAskLevel);
            }
        }

        if (order.remainingQuantity == 0)
        {
            break;
        }
    }

    if (order.remainingQuantity > 0)
    {
        addRestingOrder(std::move(order));
    }

    return trades;
}
std::vector<Trade> OrderBook::addOrder(Order order)
{
    if (order.side == Side::BUY)
    {
        return matchBuyOrder(order);
    }
    else
    {
        return matchSellOrder(order);
    }
}
Trade OrderBook::createTrade(
    const Order& buyOrder,
    const Order& sellOrder,
    Quantity executedQuantity,
    Price executionPrice)
{
    Trade trade;

    trade.buyOrderId = buyOrder.orderId;

    trade.sellOrderId = sellOrder.orderId;

    trade.symbol = buyOrder.symbol;

    trade.price = executionPrice;

    trade.quantity = executedQuantity;

    trade.timestamp = std::chrono::steady_clock::now();

    return trade;
}
std::vector<Trade> OrderBook::matchSellOrder(Order& order)
{
    std::vector<Trade> trades;

    while (order.remainingQuantity > 0 && !bids_.empty())
    {
        auto bestBidLevel = bids_.begin();

        auto& restingOrders = bestBidLevel->second;

        auto restingOrder = restingOrders.begin();

        if (restingOrder->price < order.price)
        {
            break;
        }

        Quantity executedQuantity =
            std::min(order.remainingQuantity,
                     restingOrder->remainingQuantity);

        trades.push_back(
            createTrade(
                *restingOrder,
                order,
                executedQuantity,
                restingOrder->price
            )
        );

        order.remainingQuantity -= executedQuantity;

        restingOrder->remainingQuantity -= executedQuantity;

        order.status =
            (order.remainingQuantity == 0)
                ? OrderStatus::FILLED
                : OrderStatus::PARTIALLY_FILLED;

        restingOrder->status =
            (restingOrder->remainingQuantity == 0)
                ? OrderStatus::FILLED
                : OrderStatus::PARTIALLY_FILLED;

        if (restingOrder->remainingQuantity == 0)
        {
            orderIndex_.erase(restingOrder->orderId);

            restingOrders.erase(restingOrder);

            if (restingOrders.empty())
            {
                bids_.erase(bestBidLevel);
            }
        }

        if (order.remainingQuantity == 0)
        {
            break;
        }
    }

    if (order.remainingQuantity > 0)
    {
        addRestingOrder(std::move(order));
    }

    return trades;
}
std::vector<Trade> OrderBook::modifyOrder(
    OrderId orderId,
    Price newPrice,
    Quantity newQuantity)
{
    auto indexIt = orderIndex_.find(orderId);

    if (indexIt == orderIndex_.end())
    {
        return {};
    }

    Order modifiedOrder = *(indexIt->second.orderIterator);

    cancelOrder(orderId);

    modifiedOrder.price = newPrice;

    modifiedOrder.quantity = newQuantity;

modifiedOrder.remainingQuantity =
    modifiedOrder.quantity;
    modifiedOrder.status = OrderStatus::NEW;

    modifiedOrder.timestamp = std::chrono::steady_clock::now();

    return addOrder(modifiedOrder);
}
Price OrderBook::getBestBid() const
{
    if (bids_.empty())
    {
        return 0;
    }

    return bids_.begin()->first;
}

Price OrderBook::getBestAsk() const
{
    if (asks_.empty())
    {
        return 0;
    }

    return asks_.begin()->first;
}

} // namespace novax::orderbook