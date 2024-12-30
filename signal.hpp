#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>

#define signals
#define slots
#define emit

template <typename... Args>
class Signal {
public:
    using SlotType = std::function<void(Args...)>;
    using Connection = std::weak_ptr<bool>;

    explicit Signal(std::mutex *mutex = nullptr)
        : m_mtx{mutex} {}

    Connection connect(SlotType slot) {
        auto slotWrapper = std::make_shared<SlotType>(std::move(slot));
        auto active = std::make_shared<bool>(true);
        m_connections.emplace_back(active, slotWrapper);
        return active;
    }

    void disconnect(const Connection& connection) {
        if (auto active = connection.lock()) {
            *active = false;
            cleanup();
        }
    }

    void operator()(Args... args) {
        _emit(std::forward<Args>(args)...);
    }

    ~Signal() {
        m_connections.clear();
    }

private:
    void _emit(Args... args) {
        for (auto& [active, slotWrapper] : m_connections) {
            if (auto lockedActive = active.lock(); lockedActive and *lockedActive) {
                if (m_mtx)
                    m_mtx->lock();
                (*slotWrapper)(args...);
                if (m_mtx)
                    m_mtx->unlock();
            }
        }

        cleanup();
    }

    std::mutex *m_mtx;
    std::vector<std::pair<std::weak_ptr<bool>, std::shared_ptr<SlotType>>> m_connections;

    void cleanup() {
        m_connections.erase(std::remove_if(m_connections.begin(), m_connections.end(),
                                         [](const auto& pair) {
                                             return pair.first.expired() or !*pair.first.lock();
                                        }),
                          m_connections.end());
    }
};

#endif
