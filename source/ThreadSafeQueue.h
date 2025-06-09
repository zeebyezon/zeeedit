// Created by David Bizon on 09/06/2025.
#pragma once

#include <queue>
#include <mutex>
#include <functional>

template<class T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() :
        m_queue(),
        m_queueMutex()
    {
    }

    ~ThreadSafeQueue() = default;

    void push(T t)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_queue.push(t);
    }

    void popAll(std::function<void(const T&)> f)
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        while (!m_queue.empty())
        {
            f(m_queue.front());
            m_queue.pop();
        }
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_queueMutex;
};
