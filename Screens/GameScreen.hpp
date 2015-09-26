#ifndef GAMESCREEN_HPP
# define GAMESCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"

class GameScreen : public octo::AbstractState, public octo::DefaultKeyboardListener
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

	virtual bool onPressed(sf::Event::KeyEvent const & event);
};

#endif
