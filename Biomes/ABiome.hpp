/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABiome.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 02:17:18 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/22 11:01:06 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABIOME_HPP
# define ABIOME_HPP
# include <NonCopyable.hpp>

# include <SFML/System/Time.hpp>
# include <SFML/Graphics/Color.hpp>
# include <SFML/System/Vector2.hpp>

# include "ParallaxScrolling.hpp"
# include "Map.hpp"

# include <cstddef>
# include <string>
# include <map>
# include <vector>

class ABiome : public octo::NonCopyable
{
public:
	virtual ~ABiome();

	virtual void			setup(std::size_t seed) = 0;
	virtual std::string		getName()const = 0;

	virtual sf::Vector2u	getMapSize() = 0;
	virtual sf::Vector2f	getMapSizeFloat() = 0;
	virtual float			getTransitionDuration() = 0;
	virtual int				getBossInstancePosX() = 0;

	virtual std::map<std::size_t, std::string>	const & getInstances() = 0;
	virtual std::vector<ParallaxScrolling::ALayer *> getLayers() = 0;
	virtual Map::MapSurfaceGenerator getMapSurfaceGenerator() = 0;
	virtual Map::TileColorGenerator getTileColorGenerator() = 0;

	virtual sf::Time		getDayDuration() = 0;
	virtual sf::Color		getSkyDayColor() = 0;
	virtual sf::Color		getSkyNightColor() = 0;
	virtual sf::Color		getNightLightColor() = 0;
	virtual sf::Color		getSunsetLightColor() = 0;
	virtual float			getWind() = 0;

	virtual std::size_t		getRockCount() = 0;
	virtual std::size_t		getTreeCount() = 0;
	virtual std::size_t		getMushroomCount() = 0;
	virtual std::size_t		getCrystalCount() = 0;
	virtual std::size_t		getCloudCount() = 0;
	virtual std::size_t		getStarCount() = 0;
	virtual std::size_t		getSunCount() = 0;
	virtual std::size_t		getMoonCount() = 0;
	virtual std::size_t		getRainbowCount() = 0;
	virtual std::size_t		getGroundRockCount() = 0;

	virtual sf::Vector2f	getRockSize() = 0;
	virtual std::size_t		getRockPartCount() = 0;
	virtual sf::Color		getRockColor() = 0;
	virtual bool			canCreateRock() = 0;

	virtual std::size_t		getTreeDepth() = 0;
	virtual sf::Vector2f	getTreeSize() = 0;
	virtual sf::Time		getTreeLifeTime() = 0;
	virtual sf::Color		getTreeColor() = 0;
	virtual float			getTreeAngle() = 0;
	virtual bool			getTreeIsMoving() = 0;
	virtual bool			canCreateTree() = 0;
	virtual bool			canCreateLeaf() = 0;
	virtual sf::Vector2f	getLeafSize() = 0;
	virtual sf::Color		getLeafColor() = 0;

	virtual sf::Vector2f	getMushroomSize() = 0;
	virtual sf::Color		getMushroomColor() = 0;
	virtual sf::Time		getMushroomLifeTime() = 0;
	virtual bool			canCreateMushroom() = 0;

	virtual sf::Vector2f	getCrystalSize() = 0;
	virtual std::size_t		getCrystalPartCount() = 0;
	virtual sf::Color		getCrystalColor() = 0;
	virtual bool			canCreateCrystal() = 0;
	virtual sf::Vector2f	getShineEffectSize() = 0;
	virtual sf::Color		getShineEffectColor() = 0;
	virtual float			getShineEffectRotateAngle() = 0;
	virtual int				getCrystalPosX() = 0;
	virtual bool			canCreateShineEffect() = 0;

	virtual sf::Vector2f	getCloudSize() = 0;
	virtual std::size_t		getCloudPartCount() = 0;
	virtual sf::Time		getCloudLifeTime() = 0;
	virtual sf::Color		getCloudColor() = 0;
	virtual bool			canCreateCloud() = 0;

	virtual sf::Vector2f	getStarSize() = 0;
	virtual sf::Color		getStarColor() = 0;
	virtual sf::Time		getStarLifeTime() = 0;
	virtual bool			canCreateStar() = 0;

	virtual sf::Vector2f 	getSunSize() = 0;
	virtual std::size_t		getSunPartCount() = 0;
	virtual sf::Color		getSunColor() = 0;
	virtual bool			canCreateSun() = 0;

	virtual sf::Vector2f 	getMoonSize() = 0;
	virtual sf::Color		getMoonColor() = 0;
	virtual sf::Time		getMoonLifeTime() = 0;
	virtual bool			canCreateMoon() = 0;

	virtual float			getRainbowThickness() = 0;
	virtual float			getRainbowPartSize() = 0;
	virtual std::size_t		getRainbowLoopCount() = 0;
	virtual sf::Time		getRainbowGrowTime() = 0;
	virtual bool			canCreateRainbow() = 0;

	virtual float			randomFloat(float min, float max) = 0;
	virtual int				randomInt(int min, int max) = 0;
	virtual bool			randomBool(float percent) = 0;

};

#endif
