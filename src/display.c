#include "display.h"

/* Global variables */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

/* Function to initialize the window */
bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    /* Set width and height of the SDL window with the max screen resolution */
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    /* Create a SDL Window */
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    /* Create a SDL renderer */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

/* Function to draw a grid */
void draw_grid(void)
{
    for (int y = 0; y < window_height; y += 10)
    {
        for (int x = 0; x < window_width; x += 10)
        {
            color_buffer[(window_width * y) + x] = 0xFF444444;
        }
    }
}

/* Function to draw a single pixel */
void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < window_width && y >= 0 && y < window_height)
    {
        color_buffer[(window_width * y) + x] = color;
    }
}

/* Function to draw a line */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;
    for (int i = 0; i <= longest_side_length; i++)
    {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

/* Function to draw a triangle */
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

/* Function to swap two integers */
void int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Calculate the inverse slopes
	float inv_slope_1 = (float)(x1 - x0) / (float)(y1 - y0);
	float inv_slope_2 = (float)(x2 - x0) / (float)(y2 - y0);

    // Start x_start anmd x_end from the top vertex (x0, y0)
	float x_start = x0;
	float x_end = x0;

    // Loop through the scanlines from top to bottom
	for (int scanline_y = y0; scanline_y <= y1; scanline_y++)
	{
		draw_line(round(x_start), scanline_y, round(x_end), scanline_y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
	}
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// Calculate the inverse slopes
	float inv_slope_1 = (float)(x2 - x0) / (float)(y2 - y0);
	float inv_slope_2 = (float)(x2 - x1) / (float)(y2 - y1);

    // Start x_start anmd x_end from the bottom vertex (x2, y2)
	float x_start = x2;
	float x_end = x2;

    // Loop through the scanlines from bottom to top
	for (int scanline_y = y2; scanline_y > y0; scanline_y--)
	{
		draw_line(round(x_start), scanline_y, round(x_end), scanline_y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
	}
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	/* Sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) */
	if (y0 > y1)
	{
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
	}

	if (y1 > y2)
	{
        int_swap(&y2, &y1);
        int_swap(&x2, &x1);
	}

	if (y0 > y1)
	{
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
	}

    if(y1 == y2)
	{
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if(y0 == y1)
	{
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
    else 
    {
        // Calculate the new vertex (Mx, My) using triangle similarity
        int Mx = x0 + ((float)(y1 - y0) / (float)(y2 - y0)) * (x2 - x0);
        int My = y1;

        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
    }
}

/* Function to draw a rectangle */
void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

/* Function to render the color buffer */
void render_color_buffer(void)
{
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

/* Function to clear the color buffer */
void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

/* Function to destroy the window */
void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}