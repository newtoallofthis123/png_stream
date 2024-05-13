build:
	@g++ main.cpp -o ./bin/main -std=c++20 -lpng
run: build
	@./bin/main
test:
	@g++ tests.cpp -o ./bin/test -std=c++20 -lpng
	@./bin/test
