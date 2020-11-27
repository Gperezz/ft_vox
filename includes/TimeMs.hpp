/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeMs.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:12:06 by gperez            #+#    #+#             */
/*   Updated: 2020/10/16 21:58:30 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMEMS_HPP
# define TIMEMS_HPP

extern "C"
{
	#include "time.h"
}

class TimeMs
{
	public:
		TimeMs();
		~TimeMs();
		void		setTime(void);
		long long	getTime(void);
		float		getTimeSeconds(void);
	private:
		long long	timeSet;
		struct		timespec time;
};

#endif