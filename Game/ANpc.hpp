#ifndef ANPC_HPP
# define ANPC_HPP

# include <SFML/Graphics/Drawable.hpp>
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include "AGameObject.hpp"
# include "ResourceDefinitions.hpp"
# include "BubbleText.hpp"

class RectangleShape;

class ANpc : public AGameObject<GameObjectType::Npc>, sf::Drawable
{
public:
	virtual ~ANpc(void);

	void setArea(sf::FloatRect const & area);
	void setPosition(sf::Vector2f const & position);
	void setOrigin(sf::Vector2f const & origin);
	void setSize(sf::Vector2f const & size);
	void setTexts(std::vector<std::string> const & texts);
	void addMapOffset(float x, float y);
	sf::Vector2f const & getPosition(void) const;

	virtual void setup(void) = 0;
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void doSpecialAction(void);

protected:
	enum Events
	{
		Left,
		Idle,
		Right,
		Jump,
		DoubleJump,
		Special
	};

	typedef std::pair<float, std::size_t>	FramePair;

	ANpc(ResourceKey const & npcId);

	void setVelocity(float velocity);
	void setBoxCollision(std::size_t type, std::size_t mask);
	void setCurrentText(std::size_t index);
	void setupIdleAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupWalkAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);

	virtual bool canWalk(void) const;
	virtual bool canJump(void) const;
	virtual bool canDoubleJump(void) const;
	virtual bool canDance(void) const;
	virtual bool canSpecial(void) const;

	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);

private:
	std::vector<std::unique_ptr<BubbleText>>	m_texts;
	octo::CharacterSprite						m_sprite;
	octo::CharacterAnimation					m_idleAnimation;
	octo::CharacterAnimation					m_walkAnimation;
	RectangleShape *							m_box;
	sf::FloatRect								m_area;
	sf::Vector2f								m_origin;
	float										m_velocity;
	std::size_t									m_currentText;

	static void setupAnimation(octo::CharacterAnimation & animation, std::initializer_list<FramePair> list, octo::LoopMode loopMode);

};

#endif
