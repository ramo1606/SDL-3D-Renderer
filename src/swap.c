#include "swap.h"

/* Function to swap two integers */
void int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

/* Function to swap two floats */
void float_swap(float* a, float* b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}