/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PotionEffectManager.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:54:52 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 03:36:23 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PotionEffectManager.hpp"
#include <algorithm>

PotionEffectManager::PotionEffectManager()
{
}

void	PotionEffectManager::addPotion(std::string const& key, std::unique_ptr<APotion>&& potion)
{
	m_prototypes.emplace(key, std::move(potion));
}

void		PotionEffectManager::startPotion(std::string const& key)
{
	m_potions.push_back(m_prototypes.at(key).get());
	m_potions.back()->start();
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
