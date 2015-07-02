#ifndef PHYSICSMAPSCREEN_HPP
# define PHYSICSMAPSCREEN_HPP

# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>
# include <Camera.hpp>
# include "PhysicsEngine.hpp"
# include "GroundManager.hpp"
# include "TestBiome.hpp"

class ConvexShape;
class RectangleShape;
class CircleShape;

class PhysicsMapScreen : public octo::AbstractState, public octo::DefaultKeyboardListener
{
public:
	PhysicsMapScreen(void);

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

private:
	PhysicsEngine &		m_engine;
	octo::Camera &		m_camera;
	ConvexShape *		m_shape;
	RectangleShape *	m_shapes[10];
	GroundManager		m_groundManager;
	TestBiome			m_biome;

	virtual bool onPressed(sf::Event::KeyEvent const & event);

};

#endif
