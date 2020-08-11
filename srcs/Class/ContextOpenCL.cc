/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ContextOpenCL.cc                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 15:37:27 by gperez            #+#    #+#             */
/*   Updated: 2020/06/26 18:46:28 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ContextOpenCL.hpp"

using namespace std;

ContextOpenCL::ContextOpenCL()
{
	this->value = NULL;
	this->loaded = false;
}

static char	*load_CLFile(string file)
{
	t_lst_str		*lst_str;
	char			*programFile;

	if (!(lst_str = ft_parse_file((char*)file.c_str())))
		return (NULL);
	if (stock_file(lst_str, &programFile, 1))
	{
		free_lst_str(lst_str);
		return (NULL);
	}
	free_lst_str(lst_str);
	return (programFile);
}


int			ContextOpenCL::addProgramString(string programFileName)
{
	char				*clFile;

	if (!(clFile = load_CLFile(programFileName)))
		return (1);
	this->programString.push_back(clFile);
	this->programLength.push_back(ft_strlen(clFile));
	return (0);
}

int			ContextOpenCL::initContext()
{
	cl_uint			nbPlat = 0;
	cl_int			err;

// Get Platforms
	clGetPlatformIDs(0, NULL, &nbPlat);
	cl_platform_id		platform[nbPlat];
	err = clGetPlatformIDs(nbPlat, platform, NULL);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "No platform detected for OpenCL\n" NA);
		return (1);
	}

	clGetPlatformInfo(platform[0], CL_PLATFORM_VERSION, 1024, this->info, NULL);
	ft_printf(GREEN "%s\n" NA, info);
	ft_bzero(&this->info, 1024);
// Get Devices
	cl_uint			nbDevices = 0;

	clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, 0, NULL, &nbDevices);
	cl_device_id		devices[nbDevices];
	err = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, nbDevices, devices, 0);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "No device detected for OpenCL\n" NA);
		return (1);
	}
	
	for (cl_uint i = 0; i < nbDevices; i++)
	{
		clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 1024, this->info, NULL);
		ft_printf(GREEN "%s\n" NA, this->info);
		ft_bzero(&this->info, 1024);
	}

// Create Context
	this->clContext = clCreateContext(NULL, nbDevices, devices, NULL, NULL, &err);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to create context for OpenCL\n" NA);
		return (1);
	}
// Create Queue
	this->clQueue = clCreateCommandQueue(this->clContext, devices[0], 0, &err);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to create queue for OpenCL\n" NA);
		return (1);
	}

// Check clFileString

	int	nbProgram;

	nbProgram = this->programString.size();
	if (nbProgram == 0)
	{
		nbProgram = this->addProgramString(KERNEL_FILE_NAME) == 1 ? 0 : 1;
		if (nbProgram == 0)
		{
			ft_printf(RED "There is no programString in this ContextOpenCL\n" NA);
			return (1);
		}
	}

// Create Program with the clFileString
	this->clProgram = clCreateProgramWithSource(this->clContext,
		(cl_uint)nbProgram, (const char **)&this->programString[0], (const size_t*)&this->programLength[0], &err);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to create program for OpenCL\n" NA);
		return (1);
	}

// Compile Program

	err = clBuildProgram(this->clProgram, nbDevices, devices, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to build program for OpenCL\n" NA);
		clGetProgramBuildInfo(this->clProgram, devices[0], CL_PROGRAM_BUILD_LOG, 1024, this->info, NULL);
		ft_printf(GREEN "%s\n" NA, this->info);
		ft_bzero(&this->info, 1024);
		return (1);
	}
	this->loaded = true;
	return (0);
}

int		ContextOpenCL::useKernel(void)
{
	cl_int			err;
	
	err = 0;
	if (!loaded)
		return (1);
	// Create Kernel
	this->kernel = clCreateKernel(this->clProgram, "test", &err);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to create kernel for OpenCL\n" NA);
		return (1);
	}

	// Allocate value
	this->value = clCreateBuffer(this->clContext, CL_MEM_READ_WRITE, sizeof(int) * 42, NULL, &err);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to allocate value for OpenCL\n" NA);
		return (1);
	}

	// Set Arguments
	err = clSetKernelArg(this->kernel, 0, sizeof(cl_mem), &this->value);
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to set kernel's arguments for OpenCL\n" NA);
		return (1);
	}

	// Queuing kernel
	size_t glob[1];
	size_t local[1];

	glob[0] = 42;
	local[0] = 2;
	err = clEnqueueNDRangeKernel(this->clQueue, this->kernel, 1, NULL, glob, local,
	0, NULL, NULL);
	
	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to enqueuing kernel for OpenCL\n" NA);
		return (1);
	}

	// Get data from device
	int	test[42];

	err = clEnqueueReadBuffer(clQueue, this->value, CL_TRUE, 0, sizeof(int) * 42, test, 0, NULL, NULL);

	if (err != CL_SUCCESS)
	{
		ft_printf(RED "Failed to read kernel data for OpenCL\n" NA);
		return (1);
	}

	for (int i = 0; i < 42; i++)
	{
		ft_printf(GREEN "GOOD %d\n" NA, test[i]);
	}
	return (0);
}

ContextOpenCL::~ContextOpenCL()
{
	size_t	i;
	int		err[5];

	i = 0;
	while (i < this->programString.size())
	{
		free(programString[i]);
		i++;
	}
	// Il faut check les erreurs
	err[0] = clReleaseContext(this->clContext);
	err[1] = clReleaseCommandQueue(this->clQueue);
	err[2] = clReleaseProgram(this->clProgram);
	err[3] = clReleaseKernel(this->kernel);
	err[4] = clReleaseMemObject(this->value);

	for (i = 0; i < 5; i++)
	{
		if (err[i] != CL_SUCCESS)
			ft_printf(RED"Error in release %d (openCL)\n" NA, i);
	}
}
