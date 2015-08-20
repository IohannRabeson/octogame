/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PersistanceGlitch.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:16:01 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 16:09:55 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTANCEGLITCH_HPP
# define PERSISTANCEGLITCH_HPP
# include "PostEffectGlitch.hpp"

/*!
 *	\brief Apply a persistance effect on the entire screen
 *
 */
class PersistanceGlitch : public PostEffectGlitch
{
public:
	explicit	PersistanceGlitch(float intensity = 0.05f);

	void		setupShader(sf::Shader& shader);
private:
	float const	m_intensity;
};


#endif
