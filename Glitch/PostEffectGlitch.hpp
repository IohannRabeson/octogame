/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectGlitch.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:14:02 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 16:10:17 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTEFFECTGLITCH_HPP
# define POSTEFFECTGLITCH_HPP
# include "GlitchEffectManager.hpp"

# include <SFML/Graphics/Shader.hpp>

/*!
 *	\brief Base for glitch using a fragment shader as post effect
 *	Constructor load a shader file from a resource key and setupShader()
 *	is the place to set the shader parameters.
 *
 *	\see PixelGlitch
 */
class PostEffectGlitch : public GlitchEffectManager::AGlitch
{
public:
	static std::size_t const	NullShaderKey;

	PostEffectGlitch(std::string const& shaderResourceKey);
	~PostEffectGlitch();

	virtual void	start();
	virtual void	stop();
private:
	virtual void	setupShader(sf::Shader& shader) = 0;
private:
	sf::Shader			m_shader;
	std::string const	m_shaderResourceKey;
	std::size_t			m_shaderKey;
};

#endif
