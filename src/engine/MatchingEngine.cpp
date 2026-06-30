#include "../../include/engine/MatchingEngine.hpp"
#include <atomic>
#include <thread>
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
}

} // namespace novax::engine