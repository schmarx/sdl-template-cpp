src_dir = ./src
main_file = ./src/main.cpp
other_files = ./src/update.cpp ./src/render.cpp ./src/vec.cpp

output_file = ./bin/main.o

includes = -L/usr/include/SDL2/
links = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

run:
	mkdir -p bin
	g++ -Wall -pedantic -O3 ${main_file} ${other_files} -o ${output_file} ${includes} ${links}
	${output_file} ${N} ${w} ${h}

test_vec:
	mkdir -p bin
	g++ -Wall -pedantic -O3 ./test/test_vec.cpp ./src/vec.cpp -o ./bin/vec.o -I./src
	./bin/vec.o

count:
	@wc -l $$(git ls-files | grep 'src/.*')
	@echo ""
	@echo commits:
	@git rev-list --count --all