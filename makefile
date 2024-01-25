default:
	@echo "Bad"
main:
	@g++ main.cpp -lncursesw -o bin/main -I include
	@./bin/main