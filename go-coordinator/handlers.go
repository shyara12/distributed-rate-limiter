package main

import (
    "encoding/json"
    "net/http"
    "sync"
)

type Policy struct {
    Key        string  `json:"key"`
    Capacity   float64 `json:"capacity"`
    RefillRate float64 `json:"refill_rate"`
}

var policies = make(map[string]Policy)
var mu sync.RWMutex

func healthHandler(w http.ResponseWriter, r *http.Request) {
    w.Write([]byte("ok"))
}

func setPolicyHandler(w http.ResponseWriter, r *http.Request) {
    var p Policy
    if err := json.NewDecoder(r.Body).Decode(&p); err != nil {
        http.Error(w, "bad request", http.StatusBadRequest)
        return
    }
    mu.Lock()
    policies[p.Key] = p
    mu.Unlock()
    w.Write([]byte("policy set"))
}

func getPolicyHandler(w http.ResponseWriter, r *http.Request) {
    q := r.URL.Query().Get("key")
    mu.RLock()
    p, ok := policies[q]
    mu.RUnlock()
    if !ok {
        http.Error(w, "not found", http.StatusNotFound)
        return
    }
    json.NewEncoder(w).Encode(p)
}

func checkHandler(w http.ResponseWriter, r *http.Request) {
    key := r.URL.Query().Get("key")
    mu.RLock()
    p, ok := policies[key]
    mu.RUnlock()
    if !ok {
        http.Error(w, "policy not found", http.StatusNotFound)
        return
    }
    json.NewEncoder(w).Encode(p)
}
