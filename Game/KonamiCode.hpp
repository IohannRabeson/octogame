#ifndef KONAMICODE_HPP
# define KONAMICODE_HPP

# include <DefaultGraphicsListeners.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/System/Time.hpp>

namespace sf
{
	class Shader;
}

class KonamiCode : public octo::DefaultKeyboardListener, public sf::Drawable
{
public:
	KonamiCode(void);
	virtual ~KonamiCode(void);

	void update(sf::Time frameTime, sf::Vector2f const & position);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

	bool canStartEvent();

private:
	enum State
	{
		Start,
		Success,
		Fail,
		End
	};
	std::size_t			m_index;
	sf::Shader &		m_shader;
	State				m_state;
	sf::Time			m_timer;
	sf::Time			m_timerEnd;
	sf::Time			m_timerMax;

	bool onPressed(sf::Event::KeyEvent const & event);

};

#endif
