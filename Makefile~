rm:
	rm euler
	rm rk2
	rm rk4
euler:
	g++ -std=c++11 -g nspring-mass-Euler-method.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o euler -lGL -lGLU -lglut
rk2:
	g++ -std=c++11 -g nspring-mass-Runga-Kutta-2.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk2 -lGL -lGLU -lglut
rk4:	
	g++ -std=c++11 -g nspring-mass-Runga-Kutta-4.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk4 -lGL -lGLU -lglut

rk-n:
		g++ -std=c++11 -g rk-n.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk-n -lGL -lGLU -lglut

adam:
		g++ -std=c++11 -g adam-bashforth-nparticles.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o adam-nparticle -lGL -lGLU -lglut

all:
		g++ -std=c++11 -g adam-bashforth-nparticles.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o adam-nparticle -lGL -lGLU -lglut
		g++ -std=c++11 -g nspring-mass-Runga-Kutta-4.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk4 -lGL -lGLU -lglut
		g++ -std=c++11 -g nspring-mass-Runga-Kutta-2.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk2 -lGL -lGLU -lglut
		g++ -std=c++11 -g nspring-mass-Euler-method.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o euler -lGL -lGLU -lglut
runall:
	./euler &
	./rk2 &
	./rk4 &
	./adam-nparticle &
