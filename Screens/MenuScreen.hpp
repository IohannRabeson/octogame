#ifndef MENUSCREEN_HPP
# define MENUSCREEN_HPP
# include <AbstractState.hpp>

# include "Game.hpp"
# include "MainMenu.hpp"
# include "InputListener.hpp"

class MenuScreen : public octo::AbstractState, public InputListener
{
public:
	MenuScreen(void);

	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;
private:
	MainMenu				m_menu;
	std::unique_ptr<Game>	m_game;

	virtual bool onInputPressed(InputListener::OctoKeys const & key);
};

#endif
