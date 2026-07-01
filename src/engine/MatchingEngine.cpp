#include "../../include/engine/MatchingEngine.hpp"
#include <atomic>
#include <thread>
#include <vector>
using novax::common::Trade;
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

    workerThread_ = std::thread(&MatchingEngine::run, this);
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

void MatchingEngine::submitOrder(const OrderRequest& request)
{
    requestQueue_.push(request);
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
            trades = orderBook_.addOrder(request.order);
            break;

        case RequestType::CANCEL_ORDER:
            orderBook_.cancelOrder(request.orderId);
            break;

        case RequestType::MODIFY_ORDER:
            trades = orderBook_.modifyOrder(
                request.orderId,
                request.newPrice,
                request.newQuantity
            );
            break;
        }
        for (const auto& trade : trades)
        {
            tradeQueue_.push(trade);
        }
    }
}
bool MatchingEngine::getTrade(Trade& trade)
{
    return tradeQueue_.pop(trade);
}

} // namespace novax::engine