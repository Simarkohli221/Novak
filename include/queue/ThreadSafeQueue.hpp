#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace novax::queue
{

template<typename T>
class ThreadSafeQueue
{
public:

    ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    ThreadSafeQueue(ThreadSafeQueue&&) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

    void push(const T& item);

    bool pop(T& item);

    void stop();

    bool empty();

private:

    std::queue<T> queue_;

    std::mutex mutex_;

    std::condition_variable condition_;

    bool stopped_ = false;
};

template<typename T>
void ThreadSafeQueue<T>::push(const T& item)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (stopped_)
    {
        return;
    }

    queue_.push(item);

    condition_.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T& item)
{
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(lock,
        [this]
        {
            return stopped_ || !queue_.empty();
        });

    if (stopped_ && queue_.empty())
    {
        return false;
    }

    item = queue_.front();
    queue_.pop();

    return true;
}

template<typename T>
void ThreadSafeQueue<T>::stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopped_ = true;
    }

    condition_.notify_all();
}

template<typename T>
bool ThreadSafeQueue<T>::empty()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

} // namespace novax::queue