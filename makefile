build:
	gcc mandelbrot.c -lSDL2 -lm -o mandelbrot

install:
	mv mandelbrot /usr/bin/mandelbrot
