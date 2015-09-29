#ifndef TRANSITIONLEVELZEROSCREEN_HPP
# define TRANSITIONLEVELZEROSCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"

class TransitionLevelZeroScreen : public octo::AbstractState
{
public:
	TransitionLevelZeroScreen();
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;

private:
	BubbleText			bubble;
	sf::Time			m_time;
};

#endif
