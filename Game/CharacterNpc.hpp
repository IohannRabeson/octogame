#ifndef CHARACTERNPC_HPP
# define CHARACTERNPC_HPP
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include <DefaultGraphicsListeners.hpp>
# include "AGameObject.hpp"
# include "RectangleShape.hpp"

# include <SFML/Graphics/Drawable.hpp>

# include <array>

class CharacterNpc : public AGameObject<GameObjectType::Npc>,
					  public sf::Drawable
{
	enum Events
	{
		Left,
		Idle,
		Jump,
		Right
	};
public:
	CharacterNpc(sf::Vector2f position, sf::FloatRect area);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;

private:
	bool		canWalk();

	float		m_velocity;
	sf::Clock	m_clock;
	bool		m_idle;
	sf::FloatRect				m_area;
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
	octo::CharacterAnimation	m_walkAnimation;
	octo::CharacterAnimation	m_jumpAnimation;
};

#endif
