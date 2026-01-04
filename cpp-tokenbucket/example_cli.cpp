#include <iostream>
#include <thread>
#include <vector>
#include "token_bucket.h"
#include "grpc_client.cpp"

int main() {
    std::cout << "Distributed Rate Limiter (C++ + Go + gRPC)\n";

    // ----- Connect to gRPC server -----
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    PolicyClient client(channel);

    // Ask user which policy key to fetch
    std::string key;
    std::cout << "Enter policy key (e.g., userA): ";
    std::cin >> key;

    // Fetch policy from Go Control Plane
    auto policy = client.GetPolicy(key);

    std::cout << "Fetched Policy from Go:\n";
    std::cout << "Key = " << policy.key << "\n";
    std::cout << "Capacity = " << policy.capacity << "\n";
    std::cout << "Refill Rate = " << policy.refill_rate << "\n";

    // Create token bucket with fetched values
    TokenBucket bucket(policy.capacity, policy.refill_rate);

    while (true) {
        std::cout << "\n[Menu]\n";
        std::cout << "1 -> Try consume token\n";
        std::cout << "2 -> Show tokens\n";
        std::cout << "3 -> Refresh policy from Go\n";
        std::cout << "0 -> Exit\n";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            if (bucket.try_consume(1)) {
                std::cout << "Allowed\n";
            } else {
                std::cout << "Rate Limited\n";
            }
        }
        else if (choice == 2) {
            std::cout << "Tokens: " << bucket.get_tokens() << "\n";
        }
        else if (choice == 3) {
            auto p = client.GetPolicy(key);
            std::cout << "Updated Policy: cap=" << p.capacity << ", refill=" << p.refill_rate << "\n";
            bucket.update(p.capacity, p.refill_rate);
        }
        else if (choice == 0) {
            break;
        }
    }

    return 0;
}
