EXTRA_DIST 	= ply2mesh

bin_PROGRAMS	= banna
lib_LIBRARIES	= libbp_ray.a

common_CFLAGS 	= -g -Wall -Wimplicit -pedantic -O3 -pipe -std=gnu1x -march=native

libbp_ray_a_CFLAGS 	= $(common_CFLAGS)
libbp_ray_a_SOURCES	= ray.cpp scene.cpp

banna_CFLAGS  		= $(common_CFLAGS)
banna_SOURCES 		= banna.cpp
banna_LDADD 		= libbp_ray.a
banna_LDFLAGS 		= -lgd -lGL -lglut
