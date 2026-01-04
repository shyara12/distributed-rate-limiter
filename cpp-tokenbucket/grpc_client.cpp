#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "generated/policy.grpc.pb.h"
#include "generated/policy.pb.h"

struct PolicyData {
    std::string key;
    double capacity;
    double refill_rate;
};

class PolicyClient {
public:
    PolicyClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(policy::PolicyService::NewStub(channel)) {}

    PolicyData GetPolicy(const std::string &key) {
        // Request message
        policy::PolicyRequest req;
        req.set_key(key);

        // Response message
        policy::PolicyResponse resp;

        // gRPC call context
        grpc::ClientContext ctx;

        // Call RPC
        grpc::Status status = stub_->GetPolicy(&ctx, req, &resp);

        if (!status.ok()) {
            std::cerr << "gRPC Error: " << status.error_message() << std::endl;
            return PolicyData{"", 0.0, 0.0};
        }

        // Return data
        return PolicyData{
            resp.key(),
            resp.capacity(),
            resp.refill_rate()
        };
    }

private:
    std::unique_ptr<policy::PolicyService::Stub> stub_;
};
