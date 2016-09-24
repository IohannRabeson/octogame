#ifndef ANPC_HPP
# define ANPC_HPP

# include <SFML/Graphics/Drawable.hpp>
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>
# include "AGameObject.hpp"
# include "ResourceDefinitions.hpp"
# include "BubbleText.hpp"
# include "IPlaceable.hpp"

class RectangleShape;
class CharacterOcto;

class ANpc : public sf::Drawable, public IPlaceable
{
public:
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
	virtual ~ANpc(void);

	RectangleShape * getBox(void);
	void setArea(sf::FloatRect const & area);
	void setPosition(sf::Vector2f const & position);
	void setOrigin(sf::Vector2f const & origin);
	void setSize(sf::Vector2f const & size);
	void setTexts(std::vector<std::wstring> const & texts, ABubble::Priority priority);
	void setCurrentText(int index);
	void setScale(float scale);
	void setDisplayText(bool displayText);
	void setNextEvent(Events event);
	void addMapOffset(float x, float y);
	void onTheFloor(void);
	sf::Vector2f const & getPosition(void) const;

	virtual void setup(void) = 0;
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void update(sf::Time frametime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void drawText(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual float getHeight(void) const;

protected:

	typedef std::pair<float, std::size_t>	FramePair;

	ANpc(ResourceKey const & npcId, bool isMeetable = true);

	void setType(GameObjectType const & type);
	void setTimer(sf::Time time);
	void setTimerMax(sf::Time timerMax);
	void setMachine(octo::FiniteStateMachine const & machine);
	void setActiveText(bool active);
	void setupBox(AGameObjectBase * gameObject, std::size_t type, std::size_t mask);
	void setTextOffset(sf::Vector2f const & offset);
	void setupIdleAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupWalkAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupSpecial1Animation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);
	void setupSpecial2Animation(std::initializer_list<FramePair> list, octo::LoopMode loopMode);

	float getScale(void) const;
	bool getCollideEventOcto(void) const;
	void addTimer(sf::Time time);
	sf::Time getTimer(void) const;
	sf::Time getTimerMax(void) const;
	sf::Vector2f const & getOrigin(void) const;
	sf::FloatRect const & getArea(void) const;
	std::vector<std::unique_ptr<BubbleText>> & getTexts(void);
	octo::CharacterSprite & getSprite(void);
	void reverseSprite(bool isReverse);
	bool isReverse(void) const;
	void setFollowOcto(bool isFollow);
	octo::CharacterAnimation & getIdleAnimation(void);
	octo::CharacterAnimation & getWalkAnimation(void);
	octo::CharacterAnimation & getSpecial1Animation(void);
	octo::CharacterAnimation & getSpecial2Animation(void);

	virtual bool isDoubleJump(void);
	virtual void resetVariables(void);

	virtual void setupMachine(void);
	virtual void updateState(void) = 0;
	virtual void updatePhysics(void) {};
	virtual void updateSprite(sf::Time frametime);
	virtual void updateText(sf::Time frametime);

	static void setupAnimation(octo::CharacterAnimation & animation, std::initializer_list<FramePair> list, octo::LoopMode loopMode);

private:
	GameObjectType								m_type;
	std::vector<std::unique_ptr<BubbleText>>	m_texts;
	octo::CharacterSprite						m_sprite;
	octo::CharacterAnimation					m_idleAnimation;
	octo::CharacterAnimation					m_walkAnimation;
	octo::CharacterAnimation					m_special1Animation;
	octo::CharacterAnimation					m_special2Animation;
	RectangleShape *							m_box;
	sf::FloatRect								m_area;
	sf::Vector2f								m_origin;
	sf::Vector2f								m_textOffset;
	sf::Time									m_timer;
	sf::Time									m_timerMax;
	int											m_currentText;
	float										m_scale;
	bool										m_displayText;
	bool										m_activeText;
	bool										m_collideOctoEvent;
	bool										m_isDoubleJump;
	bool										m_isMeetable;
	bool										m_isReverse;
	bool										m_isFollowOcto;

};

#endif
