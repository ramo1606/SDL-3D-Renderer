#include "camera.h"

camera_t camera = {
	.position = { 0.f, 0.f, 0.f },
	.direction = { 0.f, 0.f, 1.f },
	.forward_velocity = { 0.f, 0.f, 0.f },
	.yaw = 0.f
};