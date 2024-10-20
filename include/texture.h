#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include "upng.h"

/* Structure for 2D texture coordinates */
typedef struct 
{
	float u;
	float v;
} tex2_t;

/* External declarations for texture data */
extern int tex_width;
extern int tex_height;

extern upng_t* png_texture;
extern uint32_t* mesh_texture;

void load_png_texture_data(const char* filename);

#endif // TEXTURE_H