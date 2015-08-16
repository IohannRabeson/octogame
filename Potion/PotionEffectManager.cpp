/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PotionEffectManager.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:54:52 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 14:57:01 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PotionEffectManager.hpp"
#include <algorithm>

//
// class PotionEffectManager::APotion
//

PotionEffectManager::APotion::APotion(sf::Time duration) :
	m_duration(duration)
{
}

PotionEffectManager::APotion::~APotion()
{
}

void	PotionEffectManager::APotion::start()
{
	m_elapsedTime = sf::Time::Zero;
	startPotion();
}

void	PotionEffectManager::APotion::stop()
{
	stopPotion();
}

void	PotionEffectManager::APotion::update(sf::Time frameTime)
{
	m_elapsedTime += frameTime;
	updatePotion(frameTime, clamp(m_elapsedTime / m_duration, 0.f, 1.f));
}

bool	PotionEffectManager::APotion::isActive()const
{
	return (m_elapsedTime <= m_duration);
}

//
// class PotionEffectManager
//
PotionEffectManager::PotionEffectManager()
{
}

void	PotionEffectManager::addPotion(std::string const& key, std::unique_ptr<APotion>&& potion)
{
	m_prototypes.emplace(key, std::move(potion));
}

void		PotionEffectManager::startPotion(std::string const& key)
{
	auto	it = m_prototypes.find(key);

	if (it != m_prototypes.end())
	{
		it->second->start();
		m_potions.push_back(it->second.get());
	}
}

void		PotionEffectManager::update(sf::Time frameTime)
{
	for (auto* potion : m_potions)
	{
		potion->update(frameTime);
	}

	auto it = std::remove_if(m_potions.begin(), m_potions.end(),
						     [](APotion* potion){return (potion->isActive() == false);});

	std::for_each(it, m_potions.end(), [](APotion* potion){potion->stop();});
	m_potions.erase(it, m_potions.end());
}
