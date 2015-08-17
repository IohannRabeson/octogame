/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectPotion.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 12:42:31 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/16 15:52:07 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostEffectPotion.hpp"

#include <PostEffectManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffect.hpp>

#include <limits>

std::size_t const	PostEffectPotion::NullEffectIndex = std::numeric_limits<std::size_t>::max();

PostEffectPotion::PostEffectPotion(std::string const& shaderResourceKey, sf::Time duration) :
	APotion(duration),
	m_effectIndex(NullEffectIndex)
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	octo::ResourceManager& 		resources = octo::Application::getResourceManager();

	if (m_shader.loadFromMemory(resources.getText(shaderResourceKey), sf::Shader::Fragment))
	{
		m_effectIndex = postEffect.addEffect(octo::PostEffect(m_shader, false));
	}
}

PostEffectPotion::~PostEffectPotion()
{
	octo::PostEffectManager&	postEffectManager = octo::Application::getPostEffectManager();
	
	if (m_effectIndex != NullEffectIndex)
	{
		postEffectManager.removeEffect(m_effectIndex);
	}
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
	if (m_effectIndex != NullEffectIndex)
	{
		updateShader(frameTime, relativeTime, m_shader);
	}
}

void	PostEffectPotion::startPotion()
{
	octo::PostEffectManager&	postEffectManager = octo::Application::getPostEffectManager();
	
	if (m_effectIndex != NullEffectIndex)
	{
		setupShader(m_shader);
		postEffectManager.enableEffect(m_effectIndex, true);
	}
}

void	PostEffectPotion::stopPotion()
{
	octo::PostEffectManager&	postEffectManager = octo::Application::getPostEffectManager();

	if (m_effectIndex != NullEffectIndex)
	{
		postEffectManager.enableEffect(m_effectIndex, false);
	}
}
