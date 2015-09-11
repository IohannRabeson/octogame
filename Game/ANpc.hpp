#ifndef ANPC_HPP
# define ANPC_HPP

# include <SFML/Graphics/Drawable.hpp>
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include "AGameObject.hpp"
# include "ResourceDefinitions.hpp"

class RectangleShape;

class ANpc : public AGameObject<GameObjectType::Npc>, sf::Drawable
{
public:
	virtual ~ANpc(void);

	void setArea(sf::FloatRect const & area);
	void setPosition(sf::Vector2f const & position);
	void setOrigin(sf::Vector2f const & origin);
	void setSize(sf::Vector2f const & size);
	void setScale(float scale);
	void addMapOffset(float x, float y);
	void activatePhysics(bool active);
	sf::Vector2f const & getPosition(void) const;

	virtual void setup(void) = 0;
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void doSpecialAction(void);
	virtual float getHeight(void) const;

protected:
	enum Events
	{
		Left,
		Idle,
		Right,
		Jump,
		DoubleJump,
		Special1,
		Special2
	};

	typedef std::pair<float, std::size_t>	FramePair;

	ANpc(ResourceKey const & npcId);

	void setNextEvent(Events event);
	void setMachine(octo::FiniteStateMachine const & machine);
	void setVelocity(float velocity);
	void setBoxCollision(std::size_t type, std::size_t mask);
	void setupIdleAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupWalkAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupSpecial1Animation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupSpecial2Animation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);

	float getScale(void) const;
	float getVelocity(void) const;
	sf::Vector2f const & getOrigin(void) const;
	sf::FloatRect const & getArea(void) const;
	RectangleShape * getBox(void);
	octo::CharacterSprite & getSprite(void);
	octo::CharacterAnimation & getIdleAnimation(void);
	octo::CharacterAnimation & getWalkAnimation(void);
	octo::CharacterAnimation & getSpecial1Animation(void);
	octo::CharacterAnimation & getSpecial2Animation(void);

	virtual bool canWalk(void) const;
	virtual bool canJump(void) const;
	virtual bool canDoubleJump(void) const;
	virtual bool canDance(void) const;
	virtual bool canSpecial(void) const;

	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);

	static void setupAnimation(octo::CharacterAnimation & animation, std::initializer_list<FramePair> list, octo::LoopMode loopMode);

private:
	octo::CharacterSprite		m_sprite;
	octo::CharacterAnimation	m_idleAnimation;
	octo::CharacterAnimation	m_walkAnimation;
	octo::CharacterAnimation	m_special1Animation;
	octo::CharacterAnimation	m_special2Animation;
	RectangleShape *			m_box;
	sf::FloatRect				m_area;
	sf::Vector2f				m_origin;
	float						m_velocity;
	float						m_scale;

};

#endif
