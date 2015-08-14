/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PotionEffectManager.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:35:42 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 03:46:40 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POTIONMANAGER_HPP
# define POTIONMANAGER_HPP
# include <SFML/System/Time.hpp>
# include <SFML/Graphics/RenderTarget.hpp>

# include <random>
# include <map>
# include <memory>

class PotionEffectManager
{
	static float	clamp(float value, float min, float max)
	{
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}
		return (value);
	}

	public:
		class APotion
		{
		public:
			APotion(sf::Time duration) :
				m_duration(duration)
			{
			}

			virtual 		~APotion()
			{
			}

			void			start()
			{
				m_elapsedTime = sf::Time::Zero;
				startPotion();
			}

			void			stop()
			{
				stopPotion();
			}

			void			update(sf::Time frameTime)
			{
				m_elapsedTime += frameTime;
				updatePotion(frameTime, clamp(m_elapsedTime / m_duration, 0.f, 1.f));
			}

			bool			isActive()const
			{
				return (m_elapsedTime < m_duration);
			}
		private:
			virtual void	startPotion() = 0;
			virtual void	updatePotion(sf::Time, float relativeTime) = 0;
			virtual void	stopPotion() = 0;
		private:
			sf::Time	m_duration;
			sf::Time	m_elapsedTime;
		};

	PotionEffectManager();

	void		addPotion(std::string const& key, std::unique_ptr<APotion>&& potion);
	void		startPotion(std::string const& potionKey);
	void		update(sf::Time frameTime);
private:
	typedef std::map<std::string, std::unique_ptr<APotion>>	PrototypeBank;

	std::vector<APotion*>	m_potions;
	PrototypeBank			m_prototypes;
};

#endif
