#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>

/* Constants for frame rate */
#define FPS 60 
#define FRAME_TARGET_TIME (1000 / FPS)

/* External declarations for global variables */
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
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
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

/* Function to draw a rectangle */
void draw_rect(int x, int y, int width, int height, uint32_t color);

/* Function to render the color buffer */
void render_color_buffer(void);

/* Function to clear the color buffer */
void clear_color_buffer(uint32_t color);

/* Function to destroy the window */
void destroy_window(void);

#endif /* DISPLAY_H */