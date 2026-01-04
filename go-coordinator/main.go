package main

import (
    "log"
    "net"
    "net/http"

    pb "distributed-rate-limiter/proto"
    "google.golang.org/grpc"
)

func main() {
    // ---------- HTTP SERVER ----------
    http.HandleFunc("/health", healthHandler)
    http.HandleFunc("/policy/set", setPolicyHandler)
    http.HandleFunc("/policy/get", getPolicyHandler)
    http.HandleFunc("/check", checkHandler)

    go func() {
        log.Println("[HTTP] Running on :8080")
        if err := http.ListenAndServe(":8080", nil); err != nil {
            log.Fatalf("HTTP Server Error: %v", err)
        }
    }()

    // ---------- gRPC SERVER ----------
    lis, err := net.Listen("tcp", ":50051")
    if err != nil {
        log.Fatalf("failed to listen: %v", err)
    }

    grpcServer := grpc.NewServer()
    pb.RegisterPolicyServiceServer(grpcServer, &GrpcPolicyServer{})

    log.Println("[gRPC] Running on :50051")

    if err := grpcServer.Serve(lis); err != nil {
        log.Fatalf("failed to serve: %v", err)
    }
}
