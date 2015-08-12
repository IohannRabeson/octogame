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
		Right
	};
	public:
	CharacterNpc();
	void			setup(sf::Vector2f pos, sf::FloatRect rect);
	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;

	private:
	bool		canWalk();
	void		setupAnimation();
	void		setupMachine();
	void		updateState();
	void		updatePhysics(sf::Time frameTime);
	void		commitPhysicsToGraphics();

	private:
	RectangleShape*				m_box;
	sf::Clock					m_clock;
	sf::FloatRect				m_area;
	float						m_pixelSecondWalk;
	bool						m_originMove;
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
	octo::CharacterAnimation	m_walkAnimation;
};

#endif
