/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStream.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 03:53:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/18 11:38:02 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TELEPORTBEAM_HPP
# define TELEPORTBEAM_HPP
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Shader.hpp>
# include <SFML/System/Time.hpp>

# include <AnimatedSprite.hpp>
# include <VertexBuilder.hpp>
# include <ParticleSystem.hpp>
# include <Math.hpp>

# include <array>
# include <cmath>
# include <iostream>

# include "AGameObject.hpp"

class ABiome;
class RectangleShape;

class ElevatorStream : public AGameObject<GameObjectType::Elevator>
{
	class BeamParticle;
public:
	ElevatorStream();

	void		setupSprite(void);
	void		setPosX(float x);
	void		setPosY(float y);
	void		setHeight(float height);
	void		setTopY(float topY);
	void		setWidth(float width);
	void		setRotationFactor(float factor);
	void		setBiome(ABiome & biome);

	float		getHeight(void) const;
	float		getWidth(void) const;
	float		getPosY(void) const;
	float		getTopY(void) const;

	void		createRay(void);

	void		update(sf::Time frameTime);
	void		draw(sf::RenderTarget& render)const;
private:
	std::shared_ptr<BeamParticle>	m_particles;
	sf::Shader						m_shader;
	octo::VertexBuilder				m_builder;
	sf::Time						m_waveCycle;
	sf::Time						m_waveCycleDuration;
	RectangleShape*					m_box;
	float							m_topY;
	std::size_t						m_rayCountVertex;
	std::unique_ptr<sf::Vertex[]>	m_ray;
	sf::Color						m_borderColor;
	sf::Color						m_centerColor;
	sf::Color						m_upColor;

	octo::SpriteAnimation			m_animation;
	octo::AnimatedSprite			m_spriteBottomFront;
	octo::AnimatedSprite			m_spriteBottomBack;
	octo::AnimatedSprite			m_spriteTopFront;
	octo::AnimatedSprite			m_spriteTopBack;
};

#endif
