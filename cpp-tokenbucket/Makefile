CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread
GRPC_CFLAGS = $(shell pkg-config --cflags protobuf grpc++)
GRPC_LDFLAGS = $(shell pkg-config --libs protobuf grpc++ grpc)

SRC = token_bucket.cpp example_cli.cpp grpc_client.cpp \
      generated/policy.pb.cc generated/policy.grpc.pb.cc

all: example_cli

example_cli: $(SRC)
	$(CXX) $(CXXFLAGS) -o example_cli $(SRC) $(GRPC_CFLAGS) $(GRPC_LDFLAGS)

clean:
	rm -f example_cli *.o generated/*.o
