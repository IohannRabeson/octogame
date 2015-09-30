#ifndef TRANSITIONLEVELZEROSCREEN_HPP
# define TRANSITIONLEVELZEROSCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"

# include <memory>

class TransitionLevelZeroScreen : public octo::AbstractState
{
public:
	TransitionLevelZeroScreen();
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	void				playSound(std::size_t index);
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;

private:
	std::size_t						m_bubbleCount;
	std::unique_ptr<BubbleText[]>	m_bubble;
	sf::Time						m_time;
	std::vector<sf::Time>			m_timerMax;
	std::size_t						m_index;
	sf::Time						m_startTimer;
	sf::Time						m_startTimerMax;
	bool							m_soundPlayed1;
	bool							m_soundPlayed2;
};

#endif
