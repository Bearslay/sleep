default:
	@mkdir bin -p
	@g++ main.cpp -lncursesw -o bin/main -I include
	@./bin/main
mac:
	@g++ main.cpp -lncursesw -o bin/main -I include
	@./bin/main
old:
	@g++ old.cpp -lncursesw -o bin/old -I include
	@./bin/old
ui:
	@mkdir bin -p
	@g++ ui.cpp -lncursesw -o bin/ui -I include
	@./bin/ui
