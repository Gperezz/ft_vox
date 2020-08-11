__kernel void	test( __global int *ret){

	unsigned int id = get_global_id(0);
	unsigned int size = get_global_size(0);

	if (id < size)
		ret[id] = 3 + id;
	printf("Ceci est ecrit grace a OPENCL ! ID: %d SIZE: %d\n", id, size);
}