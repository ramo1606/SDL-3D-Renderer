#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

/* Constants for cube mesh */
#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) /* 6 cube faces, 2 triangles per face */

/* External declarations for cube mesh data */
extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

/* Structure for dynamic size meshes, with array of vertices and faces */
typedef struct
{
    vec3_t* vertices; /* dynamic array of vertices */
    face_t* faces;    /* dynamic array of faces */
    vec3_t rotation;  /* rotation with x, y, and z values */
} mesh_t;

/* External declaration for global mesh */
extern mesh_t mesh;

/* Function to load cube mesh data */
void load_cube_mesh_data(void);

/* Function to load mesh data from an OBJ file */
void load_obj_file_data(char* filename);

#endif /* MESH_H */