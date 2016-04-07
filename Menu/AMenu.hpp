#ifndef AMENU_HPP
# define AMENU_HPP

# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>
# include <SFML/System/Time.hpp>
# include <map>
# include <cwchar>

class AMenu : public sf::Drawable,
			  public sf::Transformable
{
public:
	enum State
	{
		Hide,
		Active,
		Draw
	};

	AMenu(void);
	virtual ~AMenu(void) = default;

	virtual void						setup(AMenu * menu);
	virtual void						setup(void) = 0;
	virtual void						update(sf::Time frameTime, sf::Vector2f const & position) = 0;
	virtual void						draw(sf::RenderTarget & render, sf::RenderStates states) const = 0;

	void								setState(State state);
	State								getState(void) const;
	AMenu *								getBackMenu(void);
	void								initTexts(void);
	std::wstring const &				getText(std::string const & text);

	std::map<std::string, std::wstring>	m_menuTexts;

private:
	State								m_currentState;
	AMenu *								m_backMenu;
};

#endif
