/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectPotion.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 03:49:33 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 03:51:15 by irabeson         ###   ########.fr       */
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
	explicit PostEffectPotion(std::string const& shaderResourceKey, sf::Time duration) :
		APotion(duration),
		m_shaderIndex(0u)
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
		octo::ResourceManager& 		resources = octo::Application::getResourceManager();
		sf::Shader					shader;

		if (m_shader.loadFromMemory(resources.getText(shaderResourceKey), sf::Shader::Fragment))
		{
			m_shaderIndex = postEffect.addShader(m_shader);
		}
	}

	~PostEffectPotion()
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
		
		postEffect.removeShader(m_shaderIndex);
	}
protected:
	sf::Shader&	getShader()
	{
		return (m_shader);
	}

	sf::Shader const&	getShader()const
	{
		return (m_shader);
	}
private:
	virtual void	startPotion()
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
		
		setupShader(m_shader);
		postEffect.enableShader(m_shaderIndex, true);
	}

	virtual void	stopPotion()
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();

		postEffect.enableShader(m_shaderIndex, false);
	}

	virtual void	updatePotion(sf::Time, float relativeTime) = 0;
	virtual void	setupShader(sf::Shader& shader) = 0;
private:
	sf::Shader	m_shader;
	std::size_t	m_shaderIndex;
};

#endif
