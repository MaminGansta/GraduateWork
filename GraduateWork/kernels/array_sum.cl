

__kernel void ArraySum(	__global int* values_per_item,
						__global int* data,
						__global int* res)
{
	int localId = get_local_id(0);
	int groupId = get_group_id(0);
	int groupSize = get_local_size(0);

	
	int sum = 0;
	for (int i = 0; i < *(values_per_item); i++)
	{
		sum += data[groupId * groupSize * *(values_per_item) + i * groupSize + localId];
	}

	atomic_add(res, sum);
}


//__kernel void ArraySum(	__global int* values_per_item,
//						__global int* data, 
//						__global int* res)
//{
//	int id = get_global_id(0);
//
//	int sum = 0;
//	for (int i = 0; i < *values_per_item; i++)
//	{
//		sum += data[id * *(values_per_item) + i];
//	}
//
//	atomic_add(res, sum);
//}