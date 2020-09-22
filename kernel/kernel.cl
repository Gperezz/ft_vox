__kernel void	test( __global char *buffer, __constant char *tabTxt, short offset, short lenTxt){

	unsigned int gid = get_global_id(0);
	unsigned int gsize = get_global_size(0);

	if (gid < gsize)
	{
		unsigned int	i;
		unsigned int	iX;
		unsigned int	iY;

		iY = 0;
		printf("Debut %d\n", gid);
		while (iY < 16)
		{
			iX = 0;
			while (iX < 16)
			{
				i = 0;
				while (i < 4)
				{
					buffer[gid * lenTxt + iY * 16 * 4 + iX * 4 + i] = tabTxt[gid * (offset + lenTxt) + offset + (iY * 16 * 4 + iX * 4 + i)];
					i++;
				}
				iX++;
			}
			iY++;
		}
		printf("Fin %d\n", gid);
	}
}