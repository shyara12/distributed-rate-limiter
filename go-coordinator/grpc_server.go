package main

import (
    "context"

    pb "distributed-rate-limiter/proto"
)

type GrpcPolicyServer struct {
    pb.UnimplementedPolicyServiceServer
}

// gRPC handler for GetPolicy
func (s *GrpcPolicyServer) GetPolicy(ctx context.Context, req *pb.PolicyRequest) (*pb.PolicyResponse, error) {
    key := req.GetKey()

    mu.RLock()
    p, exists := policies[key]
    mu.RUnlock()

    if !exists {
        return &pb.PolicyResponse{
            Key:        key,
            Capacity:   0,
            RefillRate: 0,
        }, nil
    }

    return &pb.PolicyResponse{
        Key:        p.Key,
        Capacity:   p.Capacity,
        RefillRate: p.RefillRate,
    }, nil
}
