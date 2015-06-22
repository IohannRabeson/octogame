#ifndef ENGINESCREEN_HPP
# define ENGINESCREEN_HPP

# include <AbstractState.hpp>
# include "PhysicsEngine.hpp"

class RectangleShape;
class ConvexShape;
class CircleShape;

enum Type
{
	e_npc = 1u,
	e_tile = 2u,
	e_rock = 4u,
	e_player = 8u
};

class EngineScreen : public octo::AbstractState
{
public:
	EngineScreen(void);
	virtual ~EngineScreen(void) = default;

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time deltatime);
	virtual void	draw(sf::RenderTarget & render) const;

private:
	PhysicsEngine &			m_engine;

	// TODO:remove test var
	RectangleShape *		m_shape;
	ConvexShape *			m_shapeA;
	ConvexShape *			m_shapeB;
	ConvexShape *			m_ground;
	CircleShape *			m_circle;
	CircleShape *			m_circleA;

};

#endif
