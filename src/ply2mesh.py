#!/usr/bin/env python
import sys
import math

num_vertices = 0
vertices = []
vertex_faces = []
normals = []
num_faces = 0
faces = []
elements = {}

class Vertex (object):
    def __init__ (self, attrs, values):
        for i in range (len (attrs)):
            self.__setattr__(attrs[i], float (values[i]))

    def __add__ (a, b):
        return Vertex (['x', 'y', 'z'], [a.x + b.x, a.y + b.y, a.z + b.z])

    def __sub__ (a, b):
        return Vertex (['x', 'y', 'z'], [a.x - b.x, a.y - b.y, a.z - b.z])

    def __mul__(a, b):
        return Vertex (['x', 'y', 'z'], [a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x])

    def mag (self):
        return math.sqrt (self.x*self.x + self.y*self.y + self.z*self.z)
    
    def normalize (self):
        mag = self.mag ()
        self.x = self.x/mag
        self.y = self.y/mag
        self.z = self.z/mag

    def __repr__(self):
        return "<%f, %f, %f>" % (self.x, self.y, self.z)

class Face (object):
    def __init__(self, v1, v2, v3):
        self.v1 = v1
        self.v2 = v2
        self.v3 = v3
        self.n  = (vertices [v2] - vertices [v1]) * (vertices [v3] - vertices [v1])

    def __repr__(self):
        return "%s, %s, %s, %s, %s, %s" \
               % (vertices [self.v1], normals [self.v1], vertices [self.v2],
                  normals [self.v2], vertices [self.v3], normals [self.v3])
    def __xxx__(self):
        return "%s, %s, %s" % (vertices [self.v1], vertices [self.v2], vertices [self.v3])
        
def read_vertices (file):
    for i in range (num_vertices):
        vertices.append (Vertex (elements ['vertex'], file.readline ().split ()))
        vertex_faces.append ([])

def read_faces (file):
    for i in range (num_faces):
        arr = file.readline ().split ()
        if len (arr) != 4 or arr [0] != '3':
            print ("Wrong number of faces on triangle ", arr [0])
            exit (1)
        v1 = int (arr [1])
        v2 = int (arr [2])
        v3 = int (arr [3])
        faces.append (Face (v1, v2, v3))
        vertex_faces [v1].append (i)
        vertex_faces [v2].append (i)
        vertex_faces [v3].append (i)

def post_process ():
    for i in range (num_vertices):
        v1 = Vertex (['x','y','z'], [0,0,0])
        for j in vertex_faces [i]:
            v1 = faces [j].n + v1
        if len (vertex_faces [i]):
            v1.normalize ()

        normals.append (v1)

def print_pov (file):
    print(num_vertices)
    file.write ("kmesh {\n");
    
    file.write ("\tvertices { \n\t\t%i\n" % num_vertices);
    for i in range (num_vertices):
        file.write ("\t\t%s\n" % (vertices [i]))
    file.write ("\t}\n");
    
    file.write ("\tnormals { \n\t\t%i\n" % num_vertices);
    for i in range (num_vertices):
        file.write ("\t\t%s\n" % (normals [i]))
    file.write ("\t}\n");
    
    file.write ("\tface_indices {\n\t\t%i\t\n" % num_faces);
    for i in range (num_faces):
        file.write ("\t\t<%i, %i, %i>\n" % (faces [i].v1, faces [i].v2, faces [i].v3))
    file.write ("\t}\n")
        
    file.write ("\tmaterial { the_material }\n");
    file.write ("}\n");

def read_header (file):
    items = ""
    curr_element = None
    while 1:
        items = file.readline ().split ()
        if items [0] == 'end_header':
            if num_vertices == 0 or num_faces == 0:
                print ("could not find number of vertices or faces")
                exit (1)
            return
        elif items [0] == 'element':
            curr_element = items [1]
            elements [curr_element] = []
            if items [1] == 'vertex':
                globals()['num_vertices'] = int (items [2])
            elif items [1] == 'face':
                globals()['num_faces'] = int (items [2])
            else:
                print ('element', items [1])
        elif items [0] == 'property':
            elements [curr_element].append (items [2])

def read_file (infilename, outfilename):
    file = open (infilename, 'r')

    read_header (file)
    print ("Finished reading header")
    read_vertices (file)
    print ("Finished reading vertices")
    read_faces (file)
    print ("Finished reading faces")
    post_process ()
    print ("Finished post processing")

    file.close ()
    file = open (outfilename, 'w')

    print_pov (file)
    print ("Finished printing POV file")

if __name__ == "__main__":
    read_file (sys.argv [1], sys.argv [1].replace ("ply", "msh"))
