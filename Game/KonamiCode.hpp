#ifndef KONAMICODE_HPP
# define KONAMICODE_HPP

# include <SFML/Graphics.hpp>
# include <DefaultGraphicsListeners.hpp>

class KonamiCode : public octo::DefaultKeyboardListener, public sf::Drawable
{
public:
	KonamiCode(void);
	virtual ~KonamiCode(void);

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	enum State
	{
		Success,
		Fail
	};
	std::size_t			m_index;
	sf::Shader			m_shader;
	std::size_t			m_shaderIndex;
	State				m_state;
	sf::Time			m_timer;
	sf::Time			m_timerMax;
	sf::Vector2f		m_position;

	bool onPressed(sf::Event::KeyEvent const & event);

};

#endif
