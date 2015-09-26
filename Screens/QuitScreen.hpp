#ifndef QUITSCREEN_HPP
# define QUITSCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>
# include <DefaultGraphicsListeners.hpp>

# include <string>

# include <SFML/System/Time.hpp>
# include <SFML/Graphics/Texture.hpp>
# include <SFML/Graphics/Sprite.hpp>

class QuitScreen : public octo::AbstractState,
				   public octo::DefaultKeyboardListener
{
public:
	QuitScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

private:
	virtual bool onPressed(sf::Event::KeyEvent const & event);

	sf::Texture			m_startTexture;
	sf::Sprite			m_startSprite;
	sf::Time			m_timer;
};

#endif
