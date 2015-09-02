#ifndef AMENU_HPP
# define AMENU_HPP

# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>
# include <SFML/System/Time.hpp>

class AMenu : public sf::Drawable,
			  public sf::Transformable
{
public:
	AMenu(void);
	~AMenu(void) = default;

	virtual void	setup(void) = 0;
	virtual void	update(sf::Time frameTime, sf::Vector2f const & position) = 0;
	virtual void	draw(sf::RenderTarget & render, sf::RenderStates states) const = 0;

	bool			isActive(void) const;
	void			setActive(bool isActive);

private:
	bool				m_isActive;
};

#endif
