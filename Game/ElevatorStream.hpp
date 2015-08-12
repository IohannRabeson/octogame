/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStream.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 03:53:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/12 18:29:33 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TELEPORTBEAM_HPP
# define TELEPORTBEAM_HPP
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <array>

# include <VertexBuilder.hpp>
# include <ParticleSystem.hpp>
# include <Math.hpp>

# include <cmath>
# include <iostream>

class ElevatorStream
{
	class BeamParticle;
public:
	ElevatorStream();

	void	setPoints(sf::Vector2f const& top, sf::Vector2f const& bottom);
	void	setWidth(float width);
	void	setColor(sf::Color const& color);

	void	update(sf::Time frameTime);
	void	draw(sf::RenderTarget& render)const;
private:
	std::shared_ptr<BeamParticle>	m_particles;
	sf::Shader						m_shaders;
	octo::VertexBuilder				m_builder;
	sf::Color						m_color;
	sf::Time						m_waveCycle;
	sf::Time						m_waveCycleDuration;
};

#endif
