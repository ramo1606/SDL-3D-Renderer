#include "texture.h"

int tex_width = 64;
int tex_height = 64;

upng_t* png_texture = NULL;
uint32_t* mesh_texture = NULL;

void load_png_texture_data(const char* filename)
{
    png_texture = upng_new_from_file(filename);
	if (png_texture != NULL)
	{
		upng_decode(png_texture);
		if (upng_get_error(png_texture) == UPNG_EOK)
		{
			tex_width = upng_get_width(png_texture);
			tex_height = upng_get_height(png_texture);
			mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
		}
	}
}
