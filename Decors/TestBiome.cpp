/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestBiome.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 03:05:12 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 19:50:48 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestBiome.hpp"

#include <iostream>

TestBiome::TestBiome() :
	m_treeDepth(8u),
	m_treeSize(30.f, 200.f),
	m_treeLifeTime(sf::seconds(1)),
	m_treeColor(255, 105, 180),
	m_treeAngle(45.f),
	m_canCreateTree(false),
	m_canCreateLeaf(true),
	m_leafSize(150.f, 150.f),
	m_leafColor(0, 105, 180),
	m_crystalSize(10.f, 100.f),
	m_crystalPartCount(4u),
	m_crystalColor(255, 105, 180),
	m_canCreateCrystal(false),
	m_rockSize(300.f, 300.f),
	m_rockPartCount(5u),
	m_rockColor(255, 105, 180),
	m_canCreateRock(false),
	m_cloudSize(400.f, 200.f),
	m_cloudPartCount(5u),
	m_cloudColor(255, 105, 180),
	m_canCreateCloud(false),
	m_starColor(255, 105, 180),
	m_canCreateStar(false),
	m_sunSize(200.f, 200.f),
	m_sunPartCount(3u),
	m_sunColor(255, 105, 180),
	m_canCreateSun(false),
	m_moonColor(255, 105, 180),
	m_canCreateMoon(false)
{
}

void			TestBiome::setup(std::size_t seed)
{
	std::cout << "Test biome setup...\n";
	static_cast<void>(seed);
}

std::string		TestBiome::getName()const
{
	return ("Biome Test");
}

std::size_t	TestBiome::getTreeDepth()
{
	return (m_treeDepth);
}

sf::Vector2f	TestBiome::getTreeSize()
{
	return (m_treeSize);
}

sf::Time		TestBiome::getTreeLifeTime()
{
	return (m_treeLifeTime);
}

sf::Color		TestBiome::getTreeColor()
{
	return (m_treeColor);
}

float			TestBiome::getTreeAngle()
{
	return (m_treeAngle);
}

bool			TestBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			TestBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	TestBiome::getLeafSize()
{
	return (m_leafSize);
}

sf::Color		TestBiome::getLeafColor()
{
	return (m_leafColor);
}

sf::Vector2f	TestBiome::getCrystalSize()
{
	return (m_crystalSize);
}

std::size_t		TestBiome::getCrystalPartCount()
{
	return (m_crystalPartCount);
}

sf::Color		TestBiome::getCrystalColor()
{
	return (m_crystalColor);
}

bool			TestBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	TestBiome::getRockSize()
{
	return (m_rockSize);
}

std::size_t		TestBiome::getRockPartCount()
{
	return (m_rockPartCount);
}

sf::Color		TestBiome::getRockColor()
{
	return (m_rockColor);
}

bool			TestBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	TestBiome::getCloudSize()
{
	return (m_cloudSize);
}

std::size_t		TestBiome::getCloudPartCount()
{
	return (m_cloudPartCount);
}

sf::Time		TestBiome::getCloudLifeTime()
{
	return (m_cloudLifeTime);
}

sf::Color		TestBiome::getCloudColor()
{
	return (m_cloudColor);
}

bool			TestBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	TestBiome::getStarSize()
{
	return (m_starSize);
}

sf::Color		TestBiome::getStarColor()
{
	return (m_starColor);
}

sf::Time		TestBiome::getStarLifeTime()
{
	return (m_starLifeTime);
}

bool			TestBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	TestBiome::getSunSize()
{
	return (m_sunSize);
}

std::size_t		TestBiome::getSunPartCount()
{
	return (m_sunPartCount);
}

sf::Color		TestBiome::getSunColor()
{
	return (m_sunColor);
}

bool			TestBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	TestBiome::getMoonSize()
{
	return (m_moonSize);
}

sf::Color		TestBiome::getMoonColor()
{
	return (m_moonColor);
}

bool			TestBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}


void	TestBiome::setTreeDepth(std::size_t depth)
{
	m_treeDepth = depth;
}

void	TestBiome::setTreeSize(sf::Vector2f const& size)
{
	m_treeSize = size;
}

void	TestBiome::setTreeLifeTime(sf::Time lifeTime)
{
	m_treeLifeTime = lifeTime;
}

void	TestBiome::setTreeColor(sf::Color const& color)
{
	m_treeColor = color;
}

void	TestBiome::setTreeAngle(float const angle)
{
	m_treeAngle = angle;
}

void	TestBiome::setCanCreateTree(bool createTree)
{
	m_canCreateTree = createTree;
}

void	TestBiome::setCanCreateLeaf(bool createLeaf)
{
	m_canCreateLeaf = createLeaf;
}

void	TestBiome::setLeafSize(sf::Vector2f const& size)
{
	m_leafSize = size;
}

void	TestBiome::setLeafColor(sf::Color const& color)
{
	m_leafColor = color;
}

void	TestBiome::setCrystalSize(sf::Vector2f const& size)
{
	m_crystalSize = size;
}

void	TestBiome::setCrystalPartCount(std::size_t count)
{
	m_crystalPartCount = count;
}

void	TestBiome::setCrystalColor(sf::Color const& color)
{
	m_crystalColor = color;
}

void	TestBiome::setCanCreateCrystal(bool canCreate)
{
	m_canCreateCrystal = canCreate;
}

void	TestBiome::setRockSize(sf::Vector2f const& size)
{
	m_rockSize = size;
}

void	TestBiome::setRockPartCount(std::size_t count)
{
	m_rockPartCount = count;
}

void	TestBiome::setRockColor(sf::Color const& color)
{
	m_rockColor = color;
}

void	TestBiome::setCanCreateRock(bool canCreate)
{
	m_canCreateRock = canCreate;
}

void	TestBiome::setCloudSize(sf::Vector2f const& size)
{
	m_cloudSize = size;
}

void	TestBiome::setCloudPartCount(std::size_t count)
{
	m_cloudPartCount = count;
}

void	TestBiome::setCloudLifeTime(sf::Time time)
{
	m_cloudLifeTime = time;
}

void	TestBiome::setCloudColor(sf::Color const& color)
{
	m_cloudColor = color;
}

void	TestBiome::setCanCreateCloud(bool canCreate)
{
	m_canCreateCloud = canCreate;
}

void	TestBiome::setStarSize(sf::Vector2f const& size)
{
	m_starSize = size;
}

void	TestBiome::setStarColor(sf::Color const& color)
{
	m_starColor = color;
}

void	TestBiome::setStarLifeTime(sf::Time time)
{
	m_starLifeTime = time;
}

void	TestBiome::setCanCreateStar(bool canCreate)
{
	m_canCreateStar = canCreate;
}

void	TestBiome::setSunSize(sf::Vector2f const& size)
{
	m_sunSize = size;
}

void	TestBiome::setSunPartCount(std::size_t count)
{
	m_sunPartCount = count;
}

void	TestBiome::setSunColor(sf::Color const& color)
{
	m_sunColor = color;
}

void	TestBiome::setCanCreateSun(bool canCreate)
{
	m_canCreateSun = canCreate;
}

void	TestBiome::setMoonSize(sf::Vector2f const& size)
{
	m_moonSize = size;
}

void	TestBiome::setMoonColor(sf::Color const& color)
{
	m_moonColor = color;
}

void	TestBiome::setCanCreateMoon(bool canCreate)
{
	m_canCreateMoon = canCreate;
}
