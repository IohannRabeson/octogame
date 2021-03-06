#ifndef TRANSITIONLEVELZEROSCREEN_HPP
# define TRANSITIONLEVELZEROSCREEN_HPP
# include <AbstractState.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"
# include "InputListener.hpp"
# include <AnimatedSprite.hpp>

# include <memory>

class TransitionLevelZeroScreen : public octo::AbstractState, public InputListener
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
	void				setupText(void);
	void				setupSprite(void);

	std::size_t						m_bubbleCount;
	std::unique_ptr<BubbleText[]>	m_bubble;
	sf::Time						m_time;
	sf::Time						m_timeCircleRainbow;
	std::vector<sf::Time>			m_timerMax;
	std::size_t						m_index;
	sf::Time						m_startTimer;
	sf::Time						m_startTimerMax;
	sf::Time						m_timerShaderRocket;
	sf::Time						m_timerShaderRocketMax;
	bool							m_soundPlayed1;
	bool							m_soundPlayed2;

	octo::AnimatedSprite			m_sprite;
	octo::SpriteAnimation			m_animation;

	virtual bool onInputPressed(InputListener::OctoKeys const & key);
};

#endif
