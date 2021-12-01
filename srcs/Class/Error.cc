/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 16:01:12 by gperez            #+#    #+#             */
/*   Updated: 2021/03/12 16:20:07 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Error.hpp"

Error::Error(int nb, std::string const& str, int lvl) throw()
	:errNb(nb),errStr(str),errLvl(lvl)
{}

const char*	Error::what(void) const throw()
{
	return (this->errStr.c_str());
}

int			Error::getNb() const throw()
{
	return (this->errNb);
}

int			Error::getLvl() const throw()
{
	return (this->errLvl);
}

Error::~Error() throw()
{
	
}