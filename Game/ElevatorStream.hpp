/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStream.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 03:53:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/13 22:27:34 by irabeson         ###   ########.fr       */
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

# include <array>
# include <cmath>
# include <iostream>

# include "AGameObject.hpp"

class ABiome;

class ElevatorStream : public AGameObject<GameObjectType::Elevator>
{
	class BeamParticle;
public:
	ElevatorStream();

	void	setPosX(float x);
	void	setPosY(float y);
	void	setHeight(float height);
	void	setWidth(float width);
	void	setRotationFactor(float factor);
	void	setBiome(ABiome & biome);

	void	update(sf::Time frameTime);
	void	draw(sf::RenderTarget& render)const;
private:
	std::shared_ptr<BeamParticle>	m_particles;
	sf::Shader						m_shader;
	octo::VertexBuilder				m_builder;
	sf::Time						m_waveCycle;
	sf::Time						m_waveCycleDuration;
};

#endif
