build:
	@g++ main.cpp -o ./bin/main -std=c++20 -lpng
run: build
	@./bin/main
