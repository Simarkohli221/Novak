#pragma once

#include <atomic>
#include <thread>

#include "../common/Trade.hpp"
#include "../orderbook/OrderBook.hpp"
#include "../queue/ThreadSafeQueue.hpp"
#include "OrderRequest.hpp"

namespace novax::engine
{

class MatchingEngine
{
public:

    MatchingEngine();

    ~MatchingEngine();

    MatchingEngine(const MatchingEngine&) = delete;
    MatchingEngine& operator=(const MatchingEngine&) = delete;

    void start();

    void stop();

    novax::common::OrderId submitOrder(
        OrderRequest request
    );

    bool getTrade(
        novax::common::Trade& trade
    );

    void printOrderBook() const;

private:

    void run();

private:

    novax::orderbook::OrderBook orderBook_;

    novax::queue::ThreadSafeQueue<OrderRequest> requestQueue_;

    novax::queue::ThreadSafeQueue<novax::common::Trade> tradeQueue_;

    std::thread workerThread_;

    std::atomic<bool> running_{false};

    std::atomic<novax::common::OrderId> nextOrderId_{1};
};

} // namespace novax::engine