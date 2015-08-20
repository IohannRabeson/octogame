/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PersistancePotion.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 02:45:15 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 13:02:17 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTANCEPOTION_HPP
# define PERSISTANCEPOTION_HPP
# include "PotionEffectManager.hpp"
# include "PostEffectPotion.hpp"

# include <SFML/Graphics/Shader.hpp>

class PersistancePotion : public PostEffectPotion
{
public:
	PersistancePotion(sf::Time duration = sf::seconds(30.f), float cycle = 15.f, float factor = 0.03f);

private:
	void	updateShader(sf::Time frameTime, float relativeTime, sf::Shader& shader);
	void	setupShader(sf::Shader& shader);
private:
	float		m_duration;
	float		m_cycle;
	float		m_factor;
	float		m_intensity;
};

#endif
