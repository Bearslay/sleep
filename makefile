default:
	@g++ main.cpp -lncursesw -o bin/main -I include
	@./bin/main
ui:
	@g++ ui.cpp -lncursesw -o bin/ui -I include
	@./bin/ui