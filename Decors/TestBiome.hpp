/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestBiome.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 03:03:00 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 19:45:17 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTBIOME_HPP
# define TESTBIOME_HPP

# include "ABiome.hpp"

# include <cstddef>

# include <SFML/System/Vector2.hpp>

class TestBiome : public ABiome
{
public:
	TestBiome();

	virtual void			setup(std::size_t seed);
	virtual std::string		getName()const;

	virtual std::size_t		getTreeDepth();
	virtual sf::Vector2f	getTreeSize();
	virtual sf::Time		getTreeLifeTime();
	virtual sf::Color		getTreeColor();
	virtual float			getTreeAngle();
	virtual bool			canCreateTree();
	virtual bool			canCreateLeaf();
	virtual sf::Vector2f	getLeafSize();
	virtual sf::Color		getLeafColor();

	virtual sf::Vector2f	getCrystalSize();
	virtual std::size_t		getCrystalPartCount();
	virtual sf::Color		getCrystalColor();
	virtual bool			canCreateCrystal();
	virtual sf::Vector2f	getShineEffectSize();
	virtual sf::Color		getShineEffectColor();
	virtual float			getShineEffectRotateAngle();
	virtual bool			canCreateShineEffect();

	virtual sf::Vector2f	getRockSize();
	virtual std::size_t		getRockPartCount();
	virtual sf::Color		getRockColor();
	virtual bool			canCreateRock();

	virtual sf::Vector2f	getCloudSize();
	virtual std::size_t		getCloudPartCount();
	virtual sf::Time		getCloudLifeTime();
	virtual sf::Color		getCloudColor();
	virtual bool			canCreateCloud();

	virtual sf::Vector2f	getStarSize();
	virtual sf::Color		getStarColor();
	virtual sf::Time		getStarLifeTime();
	virtual bool			canCreateStar();

	virtual sf::Vector2f 	getSunSize();
	virtual std::size_t		getSunPartCount();
	virtual sf::Color		getSunColor();
	virtual bool			canCreateSun();

	virtual sf::Vector2f 	getMoonSize();
	virtual sf::Color		getMoonColor();
	virtual bool			canCreateMoon();

	void					setTreeDepth(std::size_t depth);
	void					setTreeSize(sf::Vector2f const& treeSize);
	void					setTreeLifeTime(sf::Time lifeTime);
	void					setTreeColor(sf::Color const& color);
	void					setTreeAngle(float const angle);
	void					setCanCreateTree(bool createTree);
	void					setCanCreateLeaf(bool createLeaf);
	void					setLeafSize(sf::Vector2f const& size);
	void					setLeafColor(sf::Color const& color);

	void					setCrystalSize(sf::Vector2f const& crystalSize);
	void					setCrystalPartCount(std::size_t count);
	void					setCrystalColor(sf::Color const& color);
	void					setCanCreateCrystal(bool canCreate);

	void					setRockSize(sf::Vector2f const& rockSize);
	void					setRockPartCount(std::size_t count);
	void					setRockColor(sf::Color const& color);
	void					setCanCreateRock(bool canCreate);

	void					setCloudSize(sf::Vector2f const& rockSize);
	void					setCloudPartCount(std::size_t count);
	void					setCloudLifeTime(sf::Time time);
	void					setCloudColor(sf::Color const& color);
	void					setCanCreateCloud(bool canCreate);

	void					setStarSize(sf::Vector2f const& size);
	void					setStarColor(sf::Color const& color);
	void					setStarLifeTime(sf::Time tile);
	void					setCanCreateStar(bool canCreate);

	void					setSunSize(sf::Vector2f const& size);
	void					setSunPartCount(std::size_t count);
	void					setSunColor(sf::Color const& color);
	void					setCanCreateSun(bool canCreate);

	void					setMoonSize(sf::Vector2f const& size);
	void					setMoonColor(sf::Color const& color);
	void					setCanCreateMoon(bool canCreate);
private:
	std::size_t				m_treeDepth;
	sf::Vector2f			m_treeSize;
	sf::Time				m_treeLifeTime;
	sf::Color				m_treeColor;
	float					m_treeAngle;
	bool					m_canCreateTree;
	bool					m_canCreateLeaf;
	sf::Vector2f			m_leafSize;
	sf::Color				m_leafColor;

	sf::Vector2f			m_crystalSize;
	std::size_t				m_crystalPartCount;
	sf::Color				m_crystalColor;
	bool					m_canCreateCrystal;

	sf::Vector2f			m_shineEffectSize;
	sf::Color				m_shineEffectColor;
	float					m_shineEffectRotateAngle;
	bool					m_canCreateShineEffect;

	sf::Vector2f			m_rockSize;
	std::size_t				m_rockPartCount;
	sf::Color				m_rockColor;
	bool					m_canCreateRock;

	sf::Vector2f			m_cloudSize;
	std::size_t				m_cloudPartCount;
	sf::Time				m_cloudLifeTime;
	sf::Color				m_cloudColor;
	bool					m_canCreateCloud;

	sf::Vector2f			m_starSize;
	sf::Color				m_starColor;
	sf::Time				m_starLifeTime;
	bool					m_canCreateStar;

	sf::Vector2f			m_sunSize;
	std::size_t				m_sunPartCount;
	sf::Color				m_sunColor;
	bool					m_canCreateSun;

	sf::Vector2f			m_moonSize;
	sf::Color				m_moonColor;
	bool					m_canCreateMoon;
};

#endif
