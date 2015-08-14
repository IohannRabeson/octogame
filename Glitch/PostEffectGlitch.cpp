/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectGlitch.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 16:03:43 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 17:26:28 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostEffectGlitch.hpp"

#include <Application.hpp>
#include <PostEffectManager.hpp>
#include <ResourceDefinitions.hpp>

#include <limits>

std::size_t const	PostEffectGlitch::NullShaderIndex = std::numeric_limits<std::size_t>::max();

PostEffectGlitch::PostEffectGlitch(std::string const& shaderResourceKey) :
	m_shaderResourceKey(shaderResourceKey),
	m_shaderIndex(NullShaderIndex)
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	octo::ResourceManager& 		resources = octo::Application::getResourceManager();
	sf::Shader					shader;

	if (m_shader.loadFromMemory(resources.getText(shaderResourceKey), sf::Shader::Fragment))
	{
		m_shaderIndex = postEffect.addShader(m_shader);
	}
}

PostEffectGlitch::~PostEffectGlitch()
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	
	if (m_shaderIndex != NullShaderIndex)
	{
		postEffect.removeShader(m_shaderIndex);
	}
}

void	PostEffectGlitch::start()
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	
	if (m_shaderIndex != NullShaderIndex)
	{
		setupShader(m_shader);
		postEffect.enableShader(m_shaderIndex, true);
	}
}

void	PostEffectGlitch::stop()
{
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();

	if (m_shaderIndex != NullShaderIndex)
	{
		postEffect.enableShader(m_shaderIndex, false);
	}
}
