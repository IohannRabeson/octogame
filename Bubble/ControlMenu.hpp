#ifndef CONTROLMENU_HPP
# define CONTROLMENU_HPP

# include "AMenu.hpp"
# include "BubbleMenu.hpp"
# include <GraphicsManager.hpp>
# include <DefaultGraphicsListeners.hpp>

class ControlMenu : public AMenu,
					public octo::DefaultKeyboardListener

{
public:
	ControlMenu(void);
	~ControlMenu(void) = default;

	virtual void	setup(void);
	virtual void	update(sf::Time frameTime, sf::Vector2f const & position);
	virtual void	draw(sf::RenderTarget & render, sf::RenderStates states) const;

	virtual bool	onPressed(sf::Event::KeyEvent const & event);

private:
	BubbleMenu					m_bubble;
	bool						m_isKeyboardListening;
};

#endif
