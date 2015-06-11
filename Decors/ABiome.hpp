/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABiome.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 02:17:18 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 19:42:39 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABIOME_HPP
# define ABIOME_HPP
# include <NonCopyable.hpp>

# include <SFML/System/Time.hpp>
# include <SFML/Graphics/Color.hpp>
# include <SFML/System/Vector2.hpp>

# include <cstddef>
# include <string>

class ABiome : public octo::NonCopyable
{
public:
	virtual ~ABiome();

	virtual void			setup(std::size_t seed) = 0;
	virtual std::string		getName()const = 0;
	virtual unsigned int	getTreeDepth()= 0;
	virtual sf::Time		getTreeLifeTime() = 0;
	virtual sf::Color		getTreeColor() = 0;
	virtual bool			canCreateTree() = 0;

	virtual sf::Vector2f	getCrystalSize() = 0;
	virtual unsigned int	getCrystalPartCount() = 0;
	virtual sf::Color		getCrystalColor() = 0;
	virtual bool			canCreateCrystal() = 0;

	virtual sf::Vector2f	getRockSize() = 0;
	virtual unsigned int	getRockPartCount() = 0;
	virtual sf::Color		getRockColor() = 0;
	virtual bool			canCreateRock() = 0;

	virtual sf::Vector2f	getCloudSize() = 0;
	virtual unsigned int	getCloudPartCount() = 0;
	virtual sf::Time		getCloudLifeTime() = 0;
	virtual sf::Color		getCloudColor() = 0;
	virtual bool			canCreateCloud() = 0;

	virtual sf::Vector2f	getStarSize() = 0;
	virtual sf::Color		getStarColor() = 0;
	virtual sf::Time		getStarLifeTime() = 0;
	virtual bool			canCreateStar() = 0;

	virtual sf::Vector2f 	getSunSize() = 0;
	virtual sf::Color		getSunColor() = 0;
	virtual bool			canCreateSun() = 0;

	virtual sf::Vector2f 	getMoonSize() = 0;
	virtual sf::Color		getMoonColor() = 0;
	virtual bool			canCreateMoon() = 0;
};

#endif
