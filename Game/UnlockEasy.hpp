#ifndef UNLOCKEASY_HPP
# define UNLOCKEASY_HPP

# include <DefaultGraphicsListeners.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/System/Time.hpp>

namespace sf
{
	class Shader;
}

class UnlockEasy : public octo::DefaultKeyboardListener, public sf::Drawable
{
public:
	UnlockEasy(void);
	virtual ~UnlockEasy(void);

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	std::size_t			m_index;

	bool onPressed(sf::Event::KeyEvent const & event);

};

#endif
