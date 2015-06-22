#ifndef PHYSICSMAPSCREEN_HPP
# define PHYSICSMAPSCREEN_HPP

# include <AbstractState.hpp>

# include "PhysicsEngine.hpp"
# include "MapManager.hpp"

class PhysicsMapScreen : public octo::AbstractState
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

};

#endif
