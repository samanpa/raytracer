EXTRA_DIST 	= ply2mesh

bin_PROGRAMS	= banna
lib_LIBRARIES	= libbp_ray.a

common_CXXFLAGS = -g -Wall -pedantic -O3 -pipe -march=native \
	-std=gnu++0x -Wno-strict-aliasing

CLEANFILES              =  lexer.cc 

libbp_ray_a_CXXFLAGS 	= $(common_CXXFLAGS)
libbp_ray_a_SOURCES	= lexer.ll parser.cpp io.cpp \
			triangle.cpp barycentric.cpp \
			utils.cpp box.cpp kdtree.cpp kdtreebuilder.cpp \
			shader.cpp canvas.cpp scene.cpp 

banna_CXXFLAGS 		= $(common_CXXFLAGS)
banna_SOURCES 		= banna.cpp
banna_LDADD 		= libbp_ray.a
banna_LDFLAGS 		= -lgd -lGL -lglut
