#ifndef BUFFERED_CHANNEL_H
#define BUFFERED_CHANNEL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <utility>

template<typename T>
class BufferedChannel {
public:
    explicit BufferedChannel(size_t buffer_size) 
        : capacity_(buffer_size), closed_(false) {}

    void send(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this]() { 
            return closed_ || queue_.size() < capacity_; 
        });
        
        if (closed_) {
            throw std::runtime_error("Cannot send to closed channel");
        }
        
        queue_.push(std::move(value));
        not_empty_.notify_one();
    }

    std::pair<T, bool> recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this]() { 
            return closed_ || !queue_.empty(); 
        });
        
        if (!queue_.empty()) {
            T value = std::move(queue_.front());
            queue_.pop();
            not_full_.notify_one();
            return {std::move(value), true};
        }
        
        //  закрыт и очередь пустаЯ
        return {T(), false};
    }

    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        not_empty_.notify_all();
        not_full_.notify_all();
    }

    ~BufferedChannel() {
        close();
    }

private:
    std::queue<T> queue_;
    size_t capacity_;
    bool closed_;
    
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
};

#endif // BUFFERED_CHANNEL_H