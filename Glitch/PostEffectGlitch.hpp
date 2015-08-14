/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostEffectGlitch.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 01:14:02 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/16 18:43:23 by irabeson         ###   ########.fr       */
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
	static std::size_t const	NullShaderIndex;

	PostEffectGlitch(std::string const& shaderResourceKey);
	~PostEffectGlitch();

	virtual void	start();
	virtual void	stop();
protected:
	void					setBlendMode(std::size_t effectIndex, sf::BlendMode const& blendMode);
	sf::BlendMode const&	getBlendMode(std::size_t effectIndex)const;
private:
	virtual void			setupShader(sf::Shader& shader) = 0;
private:
	sf::Shader			m_shader;
	std::string const	m_shaderResourceKey;
	std::size_t			m_shaderIndex;
};

#endif
