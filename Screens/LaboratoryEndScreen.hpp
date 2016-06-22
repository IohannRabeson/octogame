#ifndef LABORATORYENDSCREEN_HPP
# define LABORATORYENDSCREEN_HPP

# include <AbstractState.hpp>
# include <SFML/Graphics/Texture.hpp>
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
	sf::Texture			m_background;
	sf::Sprite			m_backgroundSprite;
	ScientistJu			m_ju;
	ScientistLu			m_lu;
	ScientistFran		m_fran;
	ScientistCedric		m_cedric;

};

#endif
