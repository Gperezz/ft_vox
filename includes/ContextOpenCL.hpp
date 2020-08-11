/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ContextOpenCL.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 15:32:35 by gperez            #+#    #+#             */
/*   Updated: 2020/06/26 18:41:39 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXTOPENCL_HPP
# define CONTEXTOPENCL_HPP

# include <iostream>
# include <cstdio>
# include <vector>
# include <map>

extern "C"
{
	# include "ft_printf.h"
	# define CL_MINIMUM_OPENCL_VERSION 120
	# define CL_TARGET_OPENCL_VERSION 120
	# include "opencl.h"
}

# define KERNEL_FILE_NAME "kernel/kernel.cl"

// typedef int	(*errorPointer)(void*);

// std::map<std::string, errorPointer>	g_error_info_opencl = {
// 	{"context", &clReleaseContext},
// 	{}
// };

class ContextOpenCL
{
	private:
		cl_context			clContext;
		cl_command_queue	clQueue;
		cl_program			clProgram;
		cl_kernel			kernel;
		cl_mem				value;
		char				info[1024];
		std::vector<char*>	programString;
		std::vector<size_t>	programLength;
		bool				loaded;
	public:
		ContextOpenCL();
		int		addProgramString(std::string programFileName);
		int		initContext();
		int		useKernel(void);
		~ContextOpenCL();
};

#endif