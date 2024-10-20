#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include "upng.h"
#include "array.h"
#include "display.h"
#include "vector.h"
#include "matrix.h"
#include "light.h"
#include "mesh.h"

/* Array of triangles that should be rendered frame by frame */
triangle_t* triangles_to_render = NULL;

/* Global variables for execution status and game loop */
bool is_running = false;
int previous_frame_time = 0;

vec3_t camera_position = { 0.f, 0.f, 0.f };
mat4_t projection_matrix;

int triangle_avg_depth_compare(const void* a, const void* b) 
{
    const triangle_t* triangleA = (const triangle_t*)a;
    const triangle_t* triangleB = (const triangle_t*)b;

    if (triangleA->avg_depth < triangleB->avg_depth) return -1;
    if (triangleA->avg_depth > triangleB->avg_depth) return 1;
    return 0;
}

/* Setup function to initialize variables and game objects */
void setup(void)
{
	render_mode = RENDER_WIRE;
	culling_mode = CULLING_BACKFACE;

    /* Allocate the required memory in bytes to hold the color buffer */
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    /* Creating a SDL texture that is used to display the color buffer */
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

	// Initialize the perspective projection matrix
	float fov = M_PI / 3.0f;  // 60 degrees or 180/3 degrees
	float aspect = (float)window_height / (float)window_width;
	float near = 0.1f;
	float far = 100.f;
	projection_matrix = mat4_make_perspective(fov, aspect, near, far);

	// Manually load the hardcoded texture data from the static array
    //mesh_texture = (uint32_t*)REDBRICK_TEXTURE;

    /* Loads the vertex and face values for the mesh data structure */
	//load_cube_mesh_data();
	load_png_texture_data("../assets/textures/crab.png");
    load_obj_file_data("../assets/obj/crab.obj");
}

/* Poll system events and handle keyboard input */
void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        if (event.key.keysym.sym == SDLK_1)
            render_mode = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_2)
		    render_mode = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3)
			render_mode = RENDER_FILL;
		if (event.key.keysym.sym == SDLK_4)
			render_mode = RENDER_FILL_WIRE;
		if (event.key.keysym.sym == SDLK_5)
			render_mode = RENDER_TEXTURED;
		if (event.key.keysym.sym == SDLK_6)
			render_mode = RENDER_TEXTURED_WIRE;
		if (event.key.keysym.sym == SDLK_c)
			culling_mode = CULLING_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			culling_mode = CULLING_NONE;
        break;
    }
}

/* Update function frame by frame with a fixed time step */
void update(void)
{
    /* Wait some time until the reach the target frame time in milliseconds */
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    /* Only delay execution if we are running too fast */
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    /* Initialize the array of triangles to render */
    triangles_to_render = NULL;

	// Change the mesh scale, rotation, and translation values per animation frame
    //mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.01;
    //mesh.rotation.z += 0.01;

	//mesh.scale.x += 0.002f;
	//mesh.scale.y += 0.002f;
	//mesh.scale.z += 0.002f;

	//mesh.translation.x += 0.01f;
	//mesh.translation.y += 0.01f;
	mesh.translation.z = 5.f;

	// Create scale, rotation, and translation matrices that will be applied to the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

    /* Loop all triangle faces of our mesh */
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++)
    {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a];
        face_vertices[1] = mesh.vertices[mesh_face.b];
        face_vertices[2] = mesh.vertices[mesh_face.c];

        vec4_t transformed_vertices[3];

        /* Loop all three vertices of this current face and apply transformations */
        for (int j = 0; j < 3; j++)
        {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			// Create a World matrix to apply scale, rotation, and translation to the mesh
			mat4_t world_matrix = mat4_identity();

			// Order of transformations: Scale -> Rotation -> Translation
            // [T]*[R]*[S]*v
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			// Apply the world matrix transformation to the vertex
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            /* Save transformed vertex in the array of transformed vertices */
            transformed_vertices[j] = transformed_vertex;
        }
		
        /* Check backface culling */
        vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*   A   */
        vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
        vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C---B */

        /* Get the vector subtraction of B-A and C-A */
        vec3_t vector_ab = vec3_sub(vector_b, vector_a);
        vec3_t vector_ac = vec3_sub(vector_c, vector_a);
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ac);

        /* Compute the face normal (using cross product to find perpendicular) */
        vec3_t normal = vec3_cross(vector_ab, vector_ac);
        vec3_normalize(&normal);

        /* Find the vector between vertex A in the triangle and the camera origin */
        vec3_t camera_ray = vec3_sub(camera_position, vector_a);

        /* Calculate how aligned the camera ray is with the face normal (using dot product) */
        float dot_normal_camera = vec3_dot(normal, camera_ray);

        if (culling_mode == CULLING_BACKFACE)
        {
            /* Bypass the triangles that are looking away from the camera */
            if (dot_normal_camera < 0)
            {
                continue;
            }
        }

		vec4_t projected_points[3];

        /* Loop all three vertices to perform projection */
        for (int j = 0; j < 3; j++)
        {
            /* Project the current vertex */
            projected_points[j] = mat4_mul_vec4_project(projection_matrix, transformed_vertices[j]);

            // Scale into the view
            projected_points[j].x *= 0.5f * window_width;
            projected_points[j].y *= 0.5f * window_height;

            /* Invert the y-axis to have the origin on the top-left corner */
            projected_points[j].y *= -1;

            /* Translate the projected points to the middle of the screen */
            projected_points[j].x += 0.5f * window_width;
            projected_points[j].y += 0.5f * window_height;
        }

		// Calculate the shade intensity based on how alligned the normal is with the inverse of the light direction
		float light_intensity_factor = -vec3_dot(normal, light.direction);
		
		// Calculate the triangle color based on the light direction
		uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

        triangle_t projected_triangle = {
            .points = {
                { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
				{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
				{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w },
			},
            .texcoords = { { mesh_face.a_uv.u, mesh_face.a_uv.v },
                           { mesh_face.b_uv.u, mesh_face.b_uv.v },
                           { mesh_face.c_uv.u, mesh_face.c_uv.v } 
            },
			.color = triangle_color,
			.avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.f        // average depth of the triangle
		};

        /* Save the projected triangle in the array of triangles to render */
        array_push(triangles_to_render, projected_triangle);
    }

	/* Sort the triangles to render by average depth */
	//qsort(triangles_to_render, array_length(triangles_to_render), sizeof(triangle_t), triangle_avg_depth_compare);

    // Sort the triangles to render by their avg_depth
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++) 
    {
        for (int j = i; j < num_triangles; j++) 
        {
            if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) 
            {
                // Swap the triangles positions in the array
                triangle_t temp = triangles_to_render[i];
                triangles_to_render[i] = triangles_to_render[j];
                triangles_to_render[j] = temp;
            }
        }
    }
}

/* Render function to draw objects on the display */
void render(void)
{
    draw_grid();

    /* Loop all projected triangles and render them */
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++)
    {
        triangle_t triangle = triangles_to_render[i];

        if (render_mode == RENDER_FILL || render_mode == RENDER_FILL_WIRE) 
        {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y, /* vertex A */
                triangle.points[1].x, triangle.points[1].y, /* vertex B */
                triangle.points[2].x, triangle.points[2].y, /* vertex C */
                triangle.color
            );
        }

		if (render_mode == RENDER_TEXTURED || render_mode == RENDER_TEXTURED_WIRE)
		{
			/* Draw textured triangle */
			draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,
                mesh_texture
			);
		}
        
		if (render_mode == RENDER_WIRE || render_mode == RENDER_FILL_WIRE || render_mode == RENDER_WIRE_VERTEX || render_mode == RENDER_TEXTURED_WIRE)
		{
			/* Draw wireframe triangle */
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y, /* vertex A */
				triangle.points[1].x, triangle.points[1].y, /* vertex B */
				triangle.points[2].x, triangle.points[2].y, /* vertex C */
				0xFFFFFFFF /* white color */
			);
		}

		if (render_mode == RENDER_WIRE_VERTEX)
		{
			/* Draw vertices of the triangle */
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000);
			draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000);
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000);
		}
    }

    /* Clear the array of triangles to render every frame loop */
    array_free(triangles_to_render);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

/* Free the memory that was dynamically allocated by the program */
void free_resources(void)
{
    free(color_buffer);
    upng_free(png_texture);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}

/* Main function */
int main(void)
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();

    return 0;
}