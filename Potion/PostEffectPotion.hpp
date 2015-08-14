/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectPotion.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 03:49:33 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 12:45:28 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTEFFECTPOTION_HPP
# define POSTEFFECTPOTION_HPP
# include "PotionEffectManager.hpp"

# include <PostEffectManager.hpp>
# include <Application.hpp>
# include <ResourceManager.hpp>

# include <SFML/Graphics/Shader.hpp>

class PostEffectPotion : public PotionEffectManager::APotion
{
public:
	explicit PostEffectPotion(std::string const& shaderResourceKey, sf::Time duration);

	~PostEffectPotion();
protected:
	sf::Shader&			getShader();
	sf::Shader const&	getShader()const;
private:
	virtual void		startPotion();
	virtual void		stopPotion();

	virtual void		updatePotion(sf::Time, float relativeTime) = 0;
	virtual void		setupShader(sf::Shader& shader) = 0;
private:
	sf::Shader	m_shader;
	std::size_t	m_shaderIndex;
};

#endif
