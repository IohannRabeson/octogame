/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClimatManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/18 00:15:51 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/18 01:38:12 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIMATMANAGER_HPP
# define CLIMATMANAGER_HPP
# include <VertexBuilder.hpp>

class AWheather
{
public:
	virtual ~AWheather() = default;

	virtual void		setIntensity(float intensity) = 0;
	virtual	void		update(sf::Time frameTime) = 0;
};

class ClimatManager
{
public:

private:
	// TODO: store AWheather instance
};

#endif
