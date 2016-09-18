#ifndef GAMESCREEN_HPP
# define GAMESCREEN_HPP
# include <AbstractState.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"
# include "InputListener.hpp"

class GameScreen : public octo::AbstractState, public InputListener
{
public:
	GameScreen(void);

	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;
private:
	MainMenu				m_menu;
	std::unique_ptr<Game>	m_game;
	bool					m_doSave;
	sf::Clock				m_timePlayed;
	sf::Time				m_timerRed;
	sf::Time				m_timerBlue;
	sf::Time				m_timerRedBlueMax;

	virtual bool onInputPressed(InputListener::OctoKeys const & key);
};

#endif
