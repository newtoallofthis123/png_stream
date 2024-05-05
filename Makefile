build:
	@g++ main.cpp -o ./bin/main -std=c++20 -lz
run: build
	@./bin/main
