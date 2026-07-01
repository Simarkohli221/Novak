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

    void submitOrder(const OrderRequest& request);

    bool getTrade(novax::common::Trade& trade);

private:

    void run();

private:

    novax::orderbook::OrderBook orderBook_;

    novax::queue::ThreadSafeQueue<OrderRequest> requestQueue_;

    novax::queue::ThreadSafeQueue<novax::common::Trade> tradeQueue_;

    std::thread workerThread_;

    std::atomic<bool> running_{false};
};

} // namespace novax::engine