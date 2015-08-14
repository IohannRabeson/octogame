/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelPotion.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 02:45:15 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 04:04:50 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIXELPOTION_HPP
# define PIXELPOTION_HPP
# include "PotionEffectManager.hpp"
# include "PostEffectPotion.hpp"

# include <SFML/Graphics/Shader.hpp>

class PixelPotion : public PostEffectPotion
{
public:
	PixelPotion(sf::Time duration = sf::seconds(20.f), float cycle = 16.f, float factor = 0.01f);

private:
	void	updatePotion(sf::Time frameTime, float relativeTime);
	void	setupShader(sf::Shader& shader);
private:
	float 	m_cycle;
	float	m_factor;
};

#endif
