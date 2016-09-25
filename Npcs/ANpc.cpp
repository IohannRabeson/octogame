#include "ANpc.hpp"
#include "RectangleShape.hpp"
#include "CircleShape.hpp"
#include "PhysicsEngine.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <sstream>
#include <cwchar>

ANpc::ANpc(ResourceKey const & npcId, bool isMeetable) :
	m_type(GameObjectType::None),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle()),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(5.f)),
	m_currentText(0),
	m_scale(1.f),
	m_displayText(false),
	m_activeText(true),
	m_collideOctoEvent(false),
	m_isDoubleJump(false),
	m_isMeetable(isMeetable),
	m_isReverse(false),
	m_isUpSideDown(false),
	m_isFollowOcto(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	TextManager & textManager = TextManager::getInstance();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(npcId));

	setTexts(textManager.getTextsNpc(npcId), textManager.getPriority(npcId));
	if (m_texts.size())
		m_displayText = true;
}

ANpc::~ANpc(void)
{
}

void ANpc::setType(GameObjectType const & type)
{
	m_type = type;
}

void ANpc::setupIdleAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	setupAnimation(m_idleAnimation, list, loopMode);
}

void ANpc::setupWalkAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	setupAnimation(m_walkAnimation, list, loopMode);
}

void ANpc::setupSpecial1Animation(std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	setupAnimation(m_special1Animation, list, loopMode);
}

void ANpc::setupSpecial2Animation(std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	setupAnimation(m_special2Animation, list, loopMode);
}

void ANpc::setupAnimation(octo::CharacterAnimation & animation, std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	typedef octo::CharacterAnimation::Frame		Frame;
	std::vector<Frame>							frames;

	for (auto & frame : list)
		frames.push_back(Frame(sf::seconds(frame.first), { frame.second, sf::FloatRect(), sf::Vector2f() }));
	animation.setFrames(frames);
	animation.setLoop(loopMode);
}

void ANpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					walkLeftState;
	StatePtr					walkRightState;

	idleState = std::make_shared<State>("0", m_idleAnimation, m_sprite);
	walkLeftState = std::make_shared<State>("1", m_walkAnimation, m_sprite);
	walkRightState = std::make_shared<State>("2", m_walkAnimation, m_sprite);

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, walkLeftState, idleState);
	machine.addTransition(Idle, walkRightState, idleState);

	machine.addTransition(Left, idleState, walkLeftState);
	machine.addTransition(Left, walkLeftState, walkLeftState);
	machine.addTransition(Left, walkRightState, walkLeftState);

	machine.addTransition(Right, idleState, walkRightState);
	machine.addTransition(Right, walkLeftState, walkRightState);
	machine.addTransition(Right, walkRightState, walkRightState);

	setMachine(machine);
	m_sprite.setNextEvent(Idle);

}

void ANpc::setTimer(sf::Time time)
{
	m_timer = time;
}

void ANpc::setTimerMax(sf::Time timerMax)
{
	m_timerMax = timerMax;
}

void ANpc::setArea(sf::FloatRect const & area)
{
	m_area = area;
}

void ANpc::setOrigin(sf::Vector2f const & origin)
{
	m_origin = origin;
	m_sprite.setOrigin(origin);
}

void ANpc::setScale(float scale)
{
	m_scale = scale;
	m_sprite.setScale(scale, scale);
	m_box->setSize(m_box->getSize() * scale);
}

void ANpc::setPosition(sf::Vector2f const & position)
{
	m_box->setPosition(position.x, position.y - m_box->getSize().y);
	m_box->update();
}

void ANpc::setNextEvent(Events event)
{
	m_sprite.setNextEvent(event);
}

void ANpc::setMachine(octo::FiniteStateMachine const & machine)
{
	m_sprite.setMachine(machine);
	m_sprite.restart();
}

void ANpc::setupBox(AGameObjectBase * gameObject, std::size_t type, std::size_t mask)
{
	Progress & progress = Progress::getInstance();

	m_box->setGameObject(gameObject);
	m_box->setCollisionType(type);
	m_box->setCollisionMask(mask);

	if (m_isMeetable)
		progress.registerNpc(m_type);
}

void ANpc::setTextOffset(sf::Vector2f const & offset)
{
	m_textOffset = offset;
}

void ANpc::setSize(sf::Vector2f const & size)
{
	m_box->setSize(size);
}

void ANpc::setTexts(std::vector<std::wstring> const & texts, ABubble::Priority priority)
{
	for (std::size_t i = 0u; i < texts.size(); i++)
	{
		std::unique_ptr<BubbleText> bubble;
		bubble.reset(new BubbleText());
		bubble->setup(texts[i], sf::Color::White);
		bubble->setType(ABubble::Type::Speak);
		bubble->setPriority(priority);
		m_texts.push_back(std::move(bubble));
	}
}

void ANpc::setCurrentText(int index)
{
	assert(index < static_cast<int>(m_texts.size()));
	m_currentText = index;
}

void ANpc::setDisplayText(bool displayText)
{
	m_displayText = displayText;
}

void ANpc::setActiveText(bool active)
{
	for (auto & text : m_texts)
		text->setActive(active);
	m_activeText = active;
}

float ANpc::getScale(void) const
{
	return m_scale;
}

bool ANpc::getCollideEventOcto(void) const
{
	return m_collideOctoEvent;
}

void ANpc::addTimer(sf::Time time)
{
	m_timer += time;
}

sf::Time ANpc::getTimer(void) const
{
	return m_timer;
}

sf::Time ANpc::getTimerMax(void) const
{
	return m_timerMax;
}

sf::Vector2f const & ANpc::getOrigin(void) const
{
	return m_origin;
}

sf::FloatRect const & ANpc::getArea(void) const
{
	return m_area;
}

RectangleShape * ANpc::getBox(void)
{
	return m_box;
}

std::vector<std::unique_ptr<BubbleText>> & ANpc::getTexts(void)
{
	return m_texts;
}

octo::CharacterSprite & ANpc::getSprite(void)
{
	return m_sprite;
}

void ANpc::reverseSprite(bool isReverse)
{
	m_isReverse = isReverse;
}

bool ANpc::isReverse(void) const
{
	return m_isReverse;
}

void ANpc::upSideDownSprite(bool isUpSideDown)
{
	m_isUpSideDown = isUpSideDown;
}

bool ANpc::isUpSideDown(void) const
{
	return m_isUpSideDown;
}

void ANpc::setFollowOcto(bool isFollow)
{
	m_isFollowOcto = isFollow;
}

octo::CharacterAnimation & ANpc::getIdleAnimation(void)
{
	return m_idleAnimation;
}

octo::CharacterAnimation & ANpc::getWalkAnimation(void)
{
	return m_walkAnimation;
}

octo::CharacterAnimation & ANpc::getSpecial1Animation(void)
{
	return m_special1Animation;
}

octo::CharacterAnimation & ANpc::getSpecial2Animation(void)
{
	return m_special2Animation;
}

void ANpc::addMapOffset(float x, float y)
{
	m_box->setPosition(m_box->getPosition().x + x, m_box->getPosition().y + y);
	m_box->update(); // We must update ourselves because the box is out of the screen, and the engine didn't update shape out of the screen
	m_area.left += x;
	m_area.top += y;
}

void ANpc::onTheFloor(void)
{
	m_box->setType(AShape::Type::e_trigger);
	m_box->setApplyGravity(false);
}

float ANpc::getHeight(void) const
{
	return 100.f;
}

sf::Vector2f const & ANpc::getPosition(void) const
{
	return m_box->getPosition();
}

void ANpc::resetVariables(void)
{
	m_collideOctoEvent = false;
}

bool ANpc::isDoubleJump(void)
{
	return m_isDoubleJump;
}

void ANpc::update(sf::Time frametime)
{
	updateState();
	updatePhysics();

	m_timer += frametime;
	updateSprite(frametime);
	updateText(frametime);
	resetVariables();
}

void ANpc::collideOctoEvent(CharacterOcto * octo)
{
	m_collideOctoEvent = true;
	if (octo->getDoubleJump())
		m_isDoubleJump = true;
	else
		m_isDoubleJump = false;
	if (m_isMeetable && Progress::getInstance().meetNpc(m_type))
		octo->meetNpc(true);
}

void ANpc::updateSprite(sf::Time frametime)
{
	sf::FloatRect const & bounds = m_box->getGlobalBounds();
	sf::Vector2f origin = getOrigin();
	sf::Vector2f scale = sf::Vector2f(getScale(), getScale());

	m_sprite.update(frametime);
	m_sprite.setPosition(bounds.left, bounds.top);

	if (m_isFollowOcto)
	{
		if (Progress::getInstance().getOctoPos().x < getPosition().x)
			reverseSprite(true);
		else
			reverseSprite(false);
	}

	if (m_isReverse)
	{
		origin.x = m_sprite.getLocalSize().x - getOrigin().x;
		scale.x = -scale.x;
	}

	if (m_isUpSideDown)
	{
		origin.y = m_sprite.getLocalSize().y - getOrigin().y;
		scale.y = -scale.y;
	}

	m_sprite.setOrigin(origin);
	m_sprite.setScale(scale);
}

void ANpc::updateText(sf::Time frametime)
{
	if (m_displayText)
	{
		int index = m_currentText;
		if (m_isDoubleJump)
			index = m_texts.size() - 1;
		if (m_collideOctoEvent && m_activeText)
			m_texts[index]->setActive(true);
		else
			m_texts[index]->setActive(false);
		m_texts[index]->setPosition(m_box->getPosition() + m_textOffset);
		m_texts[index]->update(frametime);
	}
}

void ANpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	m_sprite.draw(render, states);
}

void ANpc::drawText(sf::RenderTarget & render, sf::RenderStates) const
{
	Progress const & progress = Progress::getInstance();

	if (m_displayText && progress.isBubbleNpc()
		&& (progress.getCurrentDestination() != Level::WaterC || m_type == GameObjectType::CedricStartNpc || m_type == GameObjectType::CedricEndNpc))
	{
		if (!m_isDoubleJump)
		{
			m_texts[m_currentText]->draw(render);
		}
		else
			m_texts[m_texts.size() - 1]->draw(render);
	}
}
