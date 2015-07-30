#ifndef CHARACTERBASICNPC_HPP
# define CHARACTERBASICNPC_HPP
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include <DefaultGraphicsListeners.hpp>
# include "AGameObject.hpp"
# include "RectangleShape.hpp"

# include <SFML/Graphics/Drawable.hpp>

# include <array>

class CharacterBasicNpc : public AGameObject<GameObjectType::Npc>,
					  public sf::Drawable
{
	enum Events
	{
		Idle
	};
public:
	CharacterBasicNpc();

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;

private:
	bool		canWalk();

	float		m_x;
	sf::Clock	m_clock;
	bool		m_idle;
	sf::FloatRect m_rect;
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
};

#endif
