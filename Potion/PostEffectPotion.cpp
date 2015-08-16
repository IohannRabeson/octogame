/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectPotion.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 12:42:31 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 13:01:32 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostEffectPotion.hpp"

# include <PostEffectManager.hpp>
# include <Application.hpp>
# include <ResourceManager.hpp>

PostEffectPotion::PostEffectPotion(std::string const& shaderResourceKey, sf::Time duration) :
	APotion(duration),
	m_shaderIndex(0u)
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	octo::ResourceManager& 		resources = octo::Application::getResourceManager();

	if (m_shader.loadFromMemory(resources.getText(shaderResourceKey), sf::Shader::Fragment))
	{
		m_shaderIndex = postEffect.addShader(m_shader);
	}
}

PostEffectPotion::~PostEffectPotion()
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	
	postEffect.removeShader(m_shaderIndex);
}

sf::Shader&	PostEffectPotion::getShader()
{
	return (m_shader);
}

sf::Shader const&	PostEffectPotion::getShader()const
{
	return (m_shader);
}

void	PostEffectPotion::updatePotion(sf::Time frameTime, float relativeTime)
{
	updateShader(frameTime, relativeTime, m_shader);
}

void	PostEffectPotion::startPotion()
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	
	setupShader(m_shader);
	postEffect.enableShader(m_shaderIndex, true);
}

void	PostEffectPotion::stopPotion()
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();

	postEffect.enableShader(m_shaderIndex, false);
}
