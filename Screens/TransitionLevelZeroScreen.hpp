#ifndef TRANSITIONLEVELZEROSCREEN_HPP
# define TRANSITIONLEVELZEROSCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"
# include <AnimatedSprite.hpp>

# include <memory>

class TransitionLevelZeroScreen : public octo::AbstractState, public octo::DefaultKeyboardListener
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
	bool							m_isStatePush;

	octo::AnimatedSprite			m_sprite;
	octo::SpriteAnimation			m_animation;

	virtual bool onPressed(sf::Event::KeyEvent const & event);
};

#endif
