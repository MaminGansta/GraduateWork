

	void PrintPixel(float8  pixel)
	{
		printf("x=%0.2f, y=%0.2f, r=%0.2f, g=%0.2f, b=%0.2f", pixel.x, pixel.y, pixel[2], pixel[3], pixel[4]);
	}

	float PixelDistance(float8 a, float8 b)
	{
		float res = 0;
		//res += pow(a.x - b.x, 2);
		//res += pow(a.y - b.y, 2);
		res += fabs(a.x - b.x) * 2;
		res += fabs(a.y - b.y) * 2;

		float brightness1 = 0;
		float brightness2 = 0;
	
		for (int i = 2; i < 5; i++)
		{
			res += pow(a[i] - b[i], 2) * 10;
			brightness1 += a[i];
			brightness2 += a[i];
		}
		res += pow(brightness2 - brightness1, 2) * 5;
		return sqrt(res);
	}

	float Distance(float8 a, float8 b)
	{
		a -= b;
		a = pow(a, 2);

		float res = 0;
		for (int i = 0; i < 8; i++)
		{
			res += a[i];
		}

		return sqrt(res);
	}


__kernel void MeanShift(__global float8* input, __global float8* output, int buffer_size, float radius)
{
	float8 active_pixel = input[get_global_id(0)];
	float8 mean_pixel = (float8)(0);
	float total = 0;
	
	for (int i = 0; i < buffer_size; i++)
	{
		float8 pixel = input[i];
		float distance = PixelDistance(active_pixel, pixel);
	
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