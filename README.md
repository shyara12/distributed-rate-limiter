A high-performance distributed rate limiter with dynamic policy updates.

🚀 Overview

This project implements a production-style rate limiting system built with:

C++ for the high-performance token bucket algorithm

Go for the control-plane (API + gRPC)

gRPC for fast policy distribution

HTTP for simple REST interactions

🧱 Architecture

           ┌──────────────────────────────┐
           │       Go Coordinator         │
           │  (HTTP + gRPC Control Plane) │
           ├───────────┬──────────────────┤
           │ HTTP      │ gRPC             │
/policy/set│           │ GetPolicy(key)   │
           ▼           ▼
        ┌───────────────────────┐
        │ C++ Token Bucket Agent│
        │   (Data Plane)        │
        └───────────────────────┘

✨ Features

Dynamic rate limit updates via gRPC

High-throughput C++ token bucket

Thread-safe implementation using mutex

REST API for policy creation

Multiple clients can subscribe to same policy

WSL compatible

🏁 Run the system
1) Start Go Coordinator
>> cd go-coordinator
>> go run .

2) Set policy via HTTP
>> curl -X POST -H "Content-Type: application/json" \
-d '{"key":"userA","capacity":10,"refill_rate":5}' \
http://localhost:8080/policy/set

3) Run C++ agent
>> cd cpp-tokenbucket
>> make
>> ./example_cli

Enter >> userA

Now:

Enter policy key (e.g., userA): userA
Fetched Policy from Go:
Key = userA
Capacity = 2
Refill Rate = 0.5

Press 1 → consume token

Press 2 → show tokens

Press 3 → refresh policy via gRPC

🧪 Dynamic Update Demo

Change policy:
>> curl -X POST -H "Content-Type: application/json" \
-d '{"key":"userA","capacity":2,"refill_rate":0.2}' \
http://localhost:8080/policy/set

CLI : 
3   ← refresh policy