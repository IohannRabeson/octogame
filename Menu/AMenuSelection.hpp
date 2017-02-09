#ifndef AMENUSELECTION_HPP
# define AMENUSELECTION_HPP

# include "AMenu.hpp"
# include "BubbleMenu.hpp"
# include "InputListener.hpp"
# include "RandomGenerator.hpp"
# include <GraphicsManager.hpp>
# include <SFML/Graphics/CircleShape.hpp>

class AMenuSelection : public AMenu, public InputListener
{
public:
	AMenuSelection(void);
	virtual ~AMenuSelection(void);

	void				setup(void);
	void				setupBubble(void);
	void				setKeyboard(bool isKeyboard);
	virtual void		update(sf::Time frameTime, sf::Vector2f const & position);
	void				draw(sf::RenderTarget & render, sf::RenderStates states) const;

	void				addMenu(std::wstring const & name, std::unique_ptr<AMenu>&& menu);
	virtual bool		onInputPressed(InputListener::OctoKeys const & key);
	virtual bool		onInputReleased(InputListener::OctoKeys const & key);

	virtual void		createMenus(void) = 0;
	virtual void		onSelection(void);

	void				setCharacterSize(std::size_t size);
	void				setBubbleType(ABubble::Type type);
	void				setIndexCursor(std::size_t index);

	std::size_t			getIndexCursor(void) const;
	void				setIsFontSelect(bool isFontSelect);
	void				setCursorAtEnd(void);

	static sf::Vector2f							m_currentMenuPosition;

private:
	sf::Vector2f								m_deltaMenu;

	RandomGenerator								m_generator;
	BubbleMenu									m_bubble;
	ABubble::Type								m_type;
	std::size_t									m_characterSize;
	std::vector<std::wstring>					m_menuTitles;
	std::vector<std::unique_ptr<AMenu>>			m_menus;

	std::vector<sf::Vector2f>					m_cursorPosition;
	std::size_t									m_indexLastCursor;
	std::size_t									m_indexCursor;
	std::size_t									m_indexSave;
	bool										m_isKeyboard;
	sf::Time									m_timerMoveCursor;
	sf::Time									m_timerMoveCursorMax;

	sf::CircleShape								m_cursor;
	sf::Time									m_inputDelay;
	sf::Time									m_inputTimer;
};

#endif
