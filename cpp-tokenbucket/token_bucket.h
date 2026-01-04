#pragma once
#include <chrono>
#include <mutex>

class TokenBucket {
public:
    TokenBucket(double capacity, double refill_rate);
    bool try_consume(int tokens = 1);
    double get_tokens();

    void update(double new_capacity, double new_refill_rate) {
        std::lock_guard<std::mutex> lock(mtx_);
        capacity_ = new_capacity;
        refill_rate_ = new_refill_rate;
        if (tokens_ > capacity_) tokens_ = capacity_;
        last_fill_ = std::chrono::steady_clock::now();
    }


private:
    double capacity_;
    double tokens_;
    double refill_rate_;
    std::chrono::steady_clock::time_point last_fill_;
    std::mutex mtx_;
    void refill();
};
