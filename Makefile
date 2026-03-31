program: pong.c
	gcc pong.c -o pong `pkg-config --cflags --libs sdl3` 

clean:
	rm -f pong 
