/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlitchManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/13 23:41:12 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 00:51:23 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLITCHMANAGER_HPP
# define GLITCHMANAGER_HPP
# include <SFML/System/Time.hpp>
# include <memory>
# include <string>
# include <vector>
# include <random>

# include <ResourceManager.hpp>

class GlitchManager
{
public:
	class AGlitch
	{
	public:
		virtual ~AGlitch()
		{
		}

		virtual void	start() = 0;
		virtual void	stop() = 0;
	};

	GlitchManager();

	std::size_t	addGlitch(std::unique_ptr<AGlitch>&& glitch);
	void		startRandomGlitch(sf::Time duration);
	void		update(sf::Time frameTime);
private:
	typedef std::vector<std::unique_ptr<AGlitch>>		PrototypeBank;
	typedef std::uniform_int_distribution<std::size_t>	RandomKey;

	std::mt19937	m_rnd;
	RandomKey		m_randomKey;
	AGlitch*		m_glitch;
	PrototypeBank	m_prototypes;
	sf::Time	 	m_glitchDuration;
	sf::Time	 	m_elapsedTime;
};

#endif
