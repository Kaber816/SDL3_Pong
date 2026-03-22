program: renderer.c
	gcc renderer.c -o renderer `pkg-config --cflags --libs sdl3` 

clean:
	rm -f renderer 
