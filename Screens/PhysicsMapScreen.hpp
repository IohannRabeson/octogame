#ifndef PHYSICSMAPSCREEN_HPP
# define PHYSICSMAPSCREEN_HPP

# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>
# include "PhysicsEngine.hpp"
# include "MapManager.hpp"

class ConvexShape;
class CircleShape;

class PhysicsMapScreen : public octo::AbstractState, public octo::IKeyboardListener
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
	MapManager	m_mapManager;
	PhysicsEngine &	m_engine;
	ConvexShape *	m_shape;
	CircleShape *	m_circle;
	bool		m_frameByFrame;

	virtual bool onPressed(sf::Event::KeyEvent const & event);
	virtual bool onReleased(sf::Event::KeyEvent const & ) {return false;}

};

#endif
