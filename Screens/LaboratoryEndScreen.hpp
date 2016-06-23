#ifndef LABORATORYENDSCREEN_HPP
# define LABORATORYENDSCREEN_HPP

# include <AbstractState.hpp>
# include <SFML/Graphics/Sprite.hpp>
# include "InputListener.hpp"
# include "ScientistNpc.hpp"
# include <memory>

class LaboratoryEndScreen : public octo::AbstractState,
							public InputListener
{
public:
	enum State
	{
		Appear,
		Dialogs,
		CedricPutPotion,
		VisualEffect,
		Disappear
	};

	LaboratoryEndScreen(void);
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

	virtual bool	onInputPressed(InputListener::OctoKeys const & key);
	virtual bool	onInputReleased(InputListener::OctoKeys const & key);

private:
	std::vector<std::unique_ptr<ScientistNpc>>	m_npcs;
	State										m_state;
	sf::Time									m_timer;
	sf::Time									m_timeBeforeNextText;
	sf::Time									m_appearDuration;
	sf::Sprite									m_background;
	std::size_t									m_textIndex;
	std::size_t									m_lastTextIndex;

};

#endif
