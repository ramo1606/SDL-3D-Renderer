#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include "texture.h"

/* Constants for frame rate */
#define FPS 60 
#define FRAME_TARGET_TIME (1000 / FPS)

enum culling_mode
{
	CULLING_BACKFACE,
	CULLING_NONE
} culling_mode;

enum render_mode
{
	RENDER_WIRE,
	RENDER_WIRE_VERTEX,
	RENDER_FILL,
	RENDER_FILL_WIRE,
	RENDER_TEXTURED,
	RENDER_TEXTURED_WIRE
} render_mode;

/* External declarations for global variables */
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern float* depth_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

/* Function to initialize the window */
bool initialize_window(void);

/* Function to draw a grid */
void draw_grid(void);

/* Function to draw a single pixel */
void draw_pixel(int x, int y, uint32_t color);

/* Function to draw a line */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

/* Function to draw a triangle */
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

/* Function to draw a filled triangle */
void draw_filled_triangle(
	int x0, int y0, float z0, float w0, 
	int x1, int y1, float z1, float w1, 
	int x2, int y2, float z2, float w2, 
	uint32_t color);

/* Function to draw a textured triangle */
void draw_textured_triangle(
	int x0, int y0, float z0, float w0, float u0, float v0, 
	int x1, int y1, float z1, float w1, float u1, float v1, 
	int x2, int y2, float z2, float w2, float u2, float v2, 
	uint32_t* texture);

/* Function to draw a rectangle */
void draw_rect(int x, int y, int width, int height, uint32_t color);

/* Function to render the color buffer */
void render_color_buffer(void);

/* Function to clear the color buffer */
void clear_color_buffer(uint32_t color);

/* Function to clear the depth buffer */
void clear_depth_buffer(void);

/* Function to destroy the window */
void destroy_window(void);

#endif /* DISPLAY_H */