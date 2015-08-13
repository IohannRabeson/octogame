/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelGlitch.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:16:01 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 01:25:40 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIXELGLITCH_HPP
# define PIXELGLITCH_HPP
# include "FragmentGlitch.hpp"

/*!
 *	\brief Apply a pixel effect on the entire screen
 *
 */
class PixelGlitch : public FragmentGlitch
{
public:
	PixelGlitch(float pixelThreshold = 0.01f) :
		FragmentGlitch(PIXELATE_FRAG),
		m_pixelThreshold(pixelThreshold)
	{
	}

	void	setupShader(sf::Shader& shader)
	{
		shader.setParameter("pixel_threshold", m_pixelThreshold);
	}
private:
	float const	m_pixelThreshold;
};


#endif
