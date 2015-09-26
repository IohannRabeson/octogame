#ifndef LEVELZERO_HPP
# define LEVELZERO_HPP

# include <AbstractState.hpp>
# include <SFML/Graphics.hpp>
# include "SpaceShip.hpp"
# include "StarSystem.hpp"

class LevelZeroScreen : public octo::AbstractState,
						public octo::DefaultKeyboardListener
{
public:
	LevelZeroScreen(void);
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;

	virtual bool	onPressed(sf::Event::KeyEvent const & event);
	virtual bool	onReleased(sf::Event::KeyEvent const& event);

private:
	enum State
	{
		Flying,
		Falling
	};
	SpaceShip			m_spaceShip;
	StarSystem			m_stars;
	sf::Time			m_timer;
	sf::Time			m_timerMax;
	sf::Time			m_timerEnd;
	sf::Time			m_timerEndMax;
	State				m_state;
	float				m_offsetCamera;
	bool				m_keyUp;
	bool				m_keyDown;
};

#endif
