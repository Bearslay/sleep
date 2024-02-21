default:
	@g++ main.cpp -lncursesw -o bin/main -I include
	@./bin/main
first:
	@mkdir bin -p
	@g++ main.cpp -lncursesw -o bin/main -I include
	@./bin/main
ui:
	@mkdir bin -p
	@g++ ui.cpp -lncursesw -o bin/ui -I include
	@./bin/ui