/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragmentGlitch.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:14:02 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 01:25:04 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAGMENTGLITCH_HPP
# define FRAGMENTGLITCH_HPP
# include "GlitchManager.hpp"
# include <Application.hpp>
# include <PostEffectManager.hpp>
# include <ResourceDefinitions.hpp>

# include <SFML/Graphics/Shader.hpp>

/*!
 *	\brief Base for glitch using a fragment shader as post effect
 *	Constructor load a shader file from a resource key and setupShader()
 *	is the place to set the shader parameters.
 *
 *	\see PixelGlitch
 */
class FragmentGlitch : public GlitchManager::AGlitch
{
public:
	FragmentGlitch(std::string const& shaderResourceKey) :
		m_shaderResourceKey(shaderResourceKey),
		m_shaderKey(std::numeric_limits<std::size_t>::max())
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
		octo::ResourceManager& 		resources = octo::Application::getResourceManager();
		sf::Shader					shader;

		if (m_shader.loadFromMemory(resources.getText(shaderResourceKey), sf::Shader::Fragment))
		{
			m_shaderKey = postEffect.addShader(m_shader);
		}
	}

	~FragmentGlitch()
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
		
		postEffect.removeShader(m_shaderKey);
	}

	virtual void	start()
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
		
		setupShader(m_shader);
		postEffect.enableShader(m_shaderKey, true);
	}

	virtual void	stop()
	{
		octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();

		postEffect.enableShader(m_shaderKey, false);
	}
private:
	virtual void	setupShader(sf::Shader& shader) = 0;
private:
	sf::Shader			m_shader;
	std::string const	m_shaderResourceKey;
	std::size_t			m_shaderKey;
};

#endif
