


	typedef struct
	{
		float x, y;
		float color[4];
	} Pixel;
	
	Pixel CreatePixel() 
	{ 
		Pixel pixel = { 0, 0, 0, 0, 0, 0 };
		return pixel;
	}
	
	Pixel PixelPlus(Pixel a, Pixel b)
	{
		a.x += b.x;
		a.y += b.y;
	
		for (int channel = 0; channel < 3; channel++)
		{
			a.color[channel] += b.color[channel];
		}
		return a;
	}
	
	Pixel PixelDev(Pixel pixel, float value)
	{
		pixel.x /= value;
		pixel.y /= value;
		for (int channel = 0; channel < 4; channel++)
		{
			pixel.color[channel] /= value;
		}
		return pixel;
	}
	
	Pixel PixelMul(Pixel pixel, float val)
	{
		pixel.x *= val;
		pixel.y *= val;
		for (int channel = 0; channel < 3; channel++)
		{
			pixel.color[channel] *= val;
		}
		return pixel;
	}
	
	float PixelDistance(Pixel a, Pixel b)
	{
		float res = 0;
		res += pow(a.x - b.x, 2);
		res += pow(a.y - b.y, 2);
	
		float brightness1 = 0;
		float brightness2 = 0;
	
		for (int i = 0; i < 3; i++)
		{
			res += pow(a.color[i] - b.color[i], 2);
			brightness1 += a.color[i];
			brightness2 += b.color[i];
		}
		//res += pow(brightness2 - brightness1, 2);

		return sqrt(res);
	}

	void PrintPixel(Pixel pixel)
	{
		printf("x=%0.2f, y=%0.2f, r=%0.2f, g=%0.2f, b=%0.2f", pixel.x, pixel.y, pixel.color[0], pixel.color[1], pixel.color[2]);
	}


__kernel void HelloWorld(__global Pixel* input, __global Pixel* output, int buffer_size, float radius)
{
	Pixel active_pixel = input[get_global_id(0)];
	Pixel mean_pixel = CreatePixel();
	float total = 0;
	
	for (int i = 0; i < buffer_size; i++)
	{
		Pixel pixel = input[i];
		float distance = PixelDistance(active_pixel, pixel);

		if (distance < radius * 3)
		{
			float influance = exp(-(distance * distance) / (2.0f * radius * radius));
			mean_pixel = PixelPlus(mean_pixel, PixelMul(pixel, influance)); 
			total += influance;
		}
	}
	
	mean_pixel = PixelDev(mean_pixel, total);
	output[get_global_id(0)] = mean_pixel;
}