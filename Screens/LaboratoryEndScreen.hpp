#ifndef LABORATORYENDSCREEN_HPP
# define LABORATORYENDSCREEN_HPP

# include <AbstractState.hpp>
# include <SFML/Graphics/Sprite.hpp>
# include "InputListener.hpp"
# include "ScientistJu.hpp"
# include "ScientistLu.hpp"
# include "ScientistFran.hpp"
# include "ScientistCedric.hpp"

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
	State				m_state;
	sf::Time			m_timer;
	sf::Time			m_timeBeforeNextText;
	sf::Time			m_appearDuration;
	sf::Sprite			m_background;
	ScientistJu			m_ju;
	ScientistLu			m_lu;
	ScientistFran		m_fran;
	ScientistCedric		m_cedric;

};

#endif
