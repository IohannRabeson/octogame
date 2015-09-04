#ifndef AMENUSELECTION_HPP
# define AMENUSELECTION_HPP

# include "AMenu.hpp"
# include "BubbleMenu.hpp"
# include <GraphicsManager.hpp>
# include <DefaultGraphicsListeners.hpp>
# include <SFML/Graphics/CircleShape.hpp>

class AMenuSelection : public AMenu,
					   public octo::DefaultKeyboardListener
{
public:
	AMenuSelection(void);
	~AMenuSelection(void) = default;

	void				setup(void);
	void				setKeyboard(bool isKeyboard);
	void				update(sf::Time frameTime, sf::Vector2f const & position);
	void				draw(sf::RenderTarget & render, sf::RenderStates states) const;

	void				addMenu(std::string const & name, AMenu * menu);
	virtual bool		onPressed(sf::Event::KeyEvent const & event);

	virtual void		createMenus(void) = 0;
	virtual void		onSelection(void);

	void				setCharacterSize(std::size_t size);
	void				setBubbleType(ABubble::Type type);
	void				setIndexCursor(std::size_t index);

	std::size_t			getIndexCursor(void);

private:
	BubbleMenu					m_bubble;
	ABubble::Type				m_type;
	std::size_t					m_characterSize;
	std::vector<std::string>	m_menuTitles;
	std::vector<AMenu *>		m_menus;

	std::vector<sf::Vector2f>	m_cursorPosition;
	std::size_t					m_indexCursor;
	bool						m_isKeyboard;

	sf::CircleShape				m_cursor;
};

#endif
