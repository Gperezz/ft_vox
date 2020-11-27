/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeMs.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 11:20:39 by gperez            #+#    #+#             */
/*   Updated: 2020/10/16 22:00:07 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TimeMs.hpp"

TimeMs::TimeMs()
{
	TimeMs::timeSet = 0;
}

static long long	get_time(struct timespec time)
{
	return (time.tv_sec * 1e3 + time.tv_nsec / 1e6);
}

void				TimeMs::setTime(void)
{
	clock_gettime(CLOCK_REALTIME, &(TimeMs::time));
	TimeMs::timeSet = get_time(TimeMs::time);
}

long long			TimeMs::getTime(void)
{
	long long	timeMs;

	clock_gettime(CLOCK_REALTIME, &(TimeMs::time));
	timeMs = get_time(TimeMs::time) - timeSet;
	return (timeMs);
}

float				TimeMs::getTimeSeconds(void)
{
	return (TimeMs::getTime() / (float)1e3);
}

TimeMs::~TimeMs()
{
}