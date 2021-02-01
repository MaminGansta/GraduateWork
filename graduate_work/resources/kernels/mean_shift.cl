

	float PixelDistance(float8 a,
						float8 b,
						int distance_coef,
						int color_coef,
						int brightness_coef)
	{
		float res = 0;
		float brightness1 = 0;
		float brightness2 = 0;
		float* a_raw = &a;
		float* b_raw = &b;

		res += pow(a.x - b.x, 2) * distance_coef;
		res += pow(a.y - b.y, 2) * distance_coef;
		
		for (int i = 2; i < 5; i++)
		{
			res += pow(a_raw[i] - b_raw[i], 2) * color_coef;
			
			brightness1 += a_raw[i];
			brightness2 += b_raw[i];
		}
		res += pow(brightness2 - brightness1, 2) * brightness_coef;
		return sqrt(res);
	}
	
__kernel void MeanShift(__global float8* input,
						__global float8* output,
						int buffer_size,
						int radius,
						int distance_coef,
						int color_coef,
						int brightness_coef)
{
	float8 active_pixel = input[get_global_id(0)];
	float8 mean_pixel = (float8)(0, 0, 0, 0, 0, 0, 0, 0);
	float total = 0;
	
	for (int i = 0; i < buffer_size; i++)
	{
		float8 pixel = input[i];
		float distance = PixelDistance(active_pixel, pixel, distance_coef, color_coef, brightness_coef);
	
		if (distance < radius * 3)
		{
			float influance = exp(-(distance * distance) / (2.0f * radius * radius));
			mean_pixel += pixel * influance;
			total += influance;
		}
	}
	mean_pixel /= total;
	output[get_global_id(0)] = mean_pixel;
}