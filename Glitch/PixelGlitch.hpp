/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelGlitch.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:16:01 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 16:09:55 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIXELGLITCH_HPP
# define PIXELGLITCH_HPP
# include "PostEffectGlitch.hpp"

/*!
 *	\brief Apply a pixel effect on the entire screen
 *
 */
class PixelGlitch : public PostEffectGlitch
{
public:
	explicit	PixelGlitch(float pixelThreshold = 0.01f);

	void		setupShader(sf::Shader& shader);
private:
	float const	m_pixelThreshold;
};


#endif
