

__kernel void ProccessArray(__global int* data, __global int* out)
{
	size_t id = get_global_id(0);
	out[id] = 2 * data[id];
}