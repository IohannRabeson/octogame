/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStream.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 03:53:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/12 19:28:39 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TELEPORTBEAM_HPP
# define TELEPORTBEAM_HPP
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <VertexBuilder.hpp>
# include <ParticleSystem.hpp>
# include <Math.hpp>

# include "RectangleShape.hpp"
# include <array>
# include <cmath>
# include <iostream>

# include "AGameObject.hpp"

class ElevatorStream : public AGameObject<GameObjectType::Elevator>
{
	class BeamParticle;
public:
	ElevatorStream();

	void	setPosBox(sf::Vector2f pos);
	void	setSizeBox(sf::Vector2f size);
	void	setPosX(float x);
	void	setPosY(float y);
	void	setHeight(float height);
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
	RectangleShape*				m_box;
};

#endif
