/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrayPotion.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 15:09:25 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 15:47:26 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAYPOTION_HPP
# define GRAYPOTION_HPP
# include "PostEffectPotion.hpp"

class GrayPotion : public PostEffectPotion
{
public:
	GrayPotion();
private:
	void	updateShader(sf::Time frameTime, float relativeTime, sf::Shader& shader);
	void	setupShader(sf::Shader&);
};

#endif
