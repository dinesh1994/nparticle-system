rm:
	rm euler
	rm rk2
	rm rk4
euler:
	g++ -g nspring-mass-Euler-method.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o euler -lGL -lGLU -lglut
rk2:
	g++ -g nspring-mass-Runga-Kutta-2.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk2 -lGL -lGLU -lglut
rk4:
	g++ -g nspring-mass-Runga-Kutta-4.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk4 -lGL -lGLU -lglut

rk-n:
		g++ -g rk-n.cpp vec3f.cpp Polygon.cpp Vertex.cpp -o rk-n -lGL -lGLU -lglut
