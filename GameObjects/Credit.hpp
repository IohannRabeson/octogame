#ifndef CREDIT_HPP
# define CREDIT_HPP

# include "ResourceDefinitions.hpp"
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Text.hpp>
# include <SFML/System/Time.hpp>
# include <map>

class Credit : public sf::Drawable
{
public:
	Credit(sf::Vector2f const & position);
	virtual ~Credit(void) = default;

	void update(sf::Time frameTime);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

	bool isFinished(void) const;

private:
	sf::Font					m_font;
	std::map<int, sf::Text>		m_texts;
	sf::Vector2f				m_speed;
	sf::Vector2f				m_position;

	void appendNames(ResourceKey key);
	void appendCategories(ResourceKey key);

};

#endif
