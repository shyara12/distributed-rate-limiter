#include "token_bucket.h"
#include <algorithm>

TokenBucket::TokenBucket(double capacity, double refill_rate)
    : capacity_(capacity), tokens_(capacity), refill_rate_(refill_rate),
      last_fill_(std::chrono::steady_clock::now()) {}

void TokenBucket::refill() {
    using namespace std::chrono;
    auto now = steady_clock::now();
    duration<double> elapsed = now - last_fill_;
    double add = elapsed.count() * refill_rate_;
    if (add > 0) {
        tokens_ = std::min(capacity_, tokens_ + add);
        last_fill_ = now;
    }
}

bool TokenBucket::try_consume(int tokens) {
    std::lock_guard<std::mutex> lock(mtx_);
    refill();
    if (tokens_ >= tokens) {
        tokens_ -= tokens;
        return true;
    }
    return false;
}

double TokenBucket::get_tokens() {
    std::lock_guard<std::mutex> lock(mtx_);
    refill();
    return tokens_;
}
