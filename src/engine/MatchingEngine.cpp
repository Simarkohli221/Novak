#include "../../include/engine/MatchingEngine.hpp"

#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
namespace novax::engine
{

MatchingEngine::MatchingEngine() = default;

MatchingEngine::~MatchingEngine()
{
    stop();
}

void MatchingEngine::start()
{
    if (running_)
    {
        return;
    }

    running_ = true;

    workerThread_ = std::thread(
        &MatchingEngine::run,
        this
    );
}
void MatchingEngine::printOrderBook() const
{
    orderBook_.printBook();
}
void MatchingEngine::stop()
{
    if (!running_)
    {
        return;
    }

    running_ = false;

    requestQueue_.stop();

    if (workerThread_.joinable())
    {
        workerThread_.join();
    }
}

novax::common::OrderId MatchingEngine::submitOrder(
    OrderRequest request
)
{
    request.order.orderId = nextOrderId_++;

    request.order.timestamp =
        std::chrono::steady_clock::now();

    requestQueue_.push(request);

    return request.order.orderId;
}

void MatchingEngine::run()
{
    OrderRequest request;

    while (running_)
    {
        if (!requestQueue_.pop(request))
        {
            break;
        }

        std::vector<novax::common::Trade> trades;

switch (request.type)
{
case RequestType::NEW_ORDER:

    trades = orderBook_.addOrder(
        request.order
    );

    break;

case RequestType::CANCEL_ORDER:
{
    bool cancelled =
        orderBook_.cancelOrder(
            request.orderId
        );

    if (cancelled)
    {
        std::cout
            << "Order "
            << request.orderId
            << " cancelled\n";
    }
    else
    {
        std::cout
            << "Order "
            << request.orderId
            << " not found\n";
    }

    break;
}

case RequestType::MODIFY_ORDER:

    trades =
        orderBook_.modifyOrder(
            request.orderId,
            request.newPrice,
            request.newQuantity
        );

    break;

case RequestType::BOOK:

    orderBook_.printBook();

    break;
}

        for (const auto& trade : trades)
        {
            tradeQueue_.push(trade);
        }
    }
}

bool MatchingEngine::getTrade(
    novax::common::Trade& trade
)
{
    return tradeQueue_.tryPop(trade);
}

} // namespace novax::engine