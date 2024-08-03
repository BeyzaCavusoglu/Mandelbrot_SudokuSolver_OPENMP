mandelbrot-kuacc: mandelbrot.cpp mandelbrot.hpp main.cpp
	g++ -Wall -Werror -W -Wextra -DKUACC main.cpp mandelbrot.cpp -fopenmp  -o ./mandelbrot


mandelbrot-vis: mandelbrot.cpp mandelbrot.hpp main.cpp
	g++ -Wall -Werror -W -Wextra main.cpp mandelbrot.cpp -fopenmp -lsfml-system -lsfml-window -lsfml-graphics -o ./mandelbrot

vis: mandelbrot-vis
	./mandelbrot

run: mandelbrot-kuacc
	./mandelbrot

clean:
	rm -f -- ./tags ./mandelbrot ./*.ppm
