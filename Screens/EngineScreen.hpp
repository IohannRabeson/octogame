#ifndef ENGINESCREEN_HPP
# define ENGINESCREEN_HPP

# include <AbstractState.hpp>
# include "PhysicsEngine.hpp"

class Polygon;
class Circle;

enum Type
{
	e_npc = 1u << 0u,
	e_tile = 1u << 1u,
	e_rock = 1u << 2u,
	e_player = 1u << 3u
};

class EngineScreen : public octo::AbstractState
{
public:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time deltatime);
	virtual void	draw(sf::RenderTarget & render) const;

private:
	PhysicsEngine		m_engine;

	// TODO:remove test var
	Polygon *			m_shape;
	Polygon *			m_shapeB;
	Polygon *			m_ground;
	Circle *			m_circle;

};

#endif
