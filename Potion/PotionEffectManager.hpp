/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PotionEffectManager.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:35:42 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 16:31:11 by irabeson         ###   ########.fr       */
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
public:
	/*!
	 *	\brief Abstract potion
	 */
	class APotion
	{
	public:
		explicit		APotion(sf::Time duration);
		virtual 		~APotion();

		void			start();
		void			stop();
		void			update(sf::Time frameTime);
		bool			isActive()const;
	private:
		/*!	Called when the potion is started */
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

	// TODO: replace by a map to check if the potions is already used
	std::vector<APotion*>	m_potions;
	PrototypeBank			m_prototypes;
};

#endif
