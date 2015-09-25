#include "NanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include "Tile.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <sstream>

NanoRobot::NanoRobot(sf::Vector2f const & position, std::string const & id, std::size_t nbFrames, int seed) :
	m_swarm(1),
	m_uniformPopulation(1234u, &octo::Application::getResourceManager().getPalette(FROM_SEA1_OPA),
						1.2f, 2.f, 6.f, 10.f, 32.f, 50.f,
						sf::Time::Zero, sf::Time::Zero),
	m_spawnMode(FireflySwarm::SpawnMode::Normal),
	m_positionBehavior(new FireflySwarm::CirclePositionBehavior(seed, 50.f)),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_textIndex(0u),
	m_state(Idle),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(7.f)),
	m_isTravelling(false),
	m_engine(std::time(0)),
	m_soundDistri(0u, 2u)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_box->setRadius(100.f);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
	m_box->setApplyGravity(false);

	m_swarm.setPositionBehavior(m_positionBehavior);
	m_swarm.setTexture(resources.getTexture(FIREFLY01_PNG));
	m_swarm.create(m_spawnMode, position, sf::Color::Magenta, 8.f, 32.f, 2.f);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(id));
	m_sprite.setScale(0.6f, 0.6f);

	octo::SpriteAnimation::FrameList	frames;
	for (std::size_t i = 0u; i < nbFrames; i++)
		frames.emplace_back(sf::seconds(0.2f), i);

	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setAnimation(m_animation);
	m_sprite.play();

	std::map<std::string, std::vector<std::wstring>>	npcTexts;
	std::wstringstream f(resources.getText(DIALOGS_TXT).toWideString());
	std::wstring wkey;
	std::wstring line;
	wchar_t delim = '=';
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		npcTexts[key].push_back(line);
	}
	for (std::size_t i = 0u; i < npcTexts[id].size(); i++)
	{
		std::unique_ptr<BubbleText> bubble;
		bubble.reset(new BubbleText());
		bubble->setup(npcTexts[id][i], sf::Color::White);
		bubble->setType(ABubble::Type::Speak);
		bubble->setActive(true);
		m_texts.push_back(std::move(bubble));
	}
}

NanoRobot::~NanoRobot(void)
{}

void NanoRobot::setup(AGameObjectBase * gameObject)
{
	m_box->setGameObject(gameObject);
	m_box->setCollisionType(static_cast<std::size_t>(gameObject->getObjectType()));
}

void NanoRobot::playSound(void)
{
	std::size_t soundChoose = m_soundDistri(m_engine);
	octo::AudioManager& audio = octo::Application::getAudioManager();
	octo::ResourceManager& resource = octo::Application::getResourceManager();

	switch (soundChoose)
	{
		case 0u:
			audio.playSound(resource.getSound(NANO_1_WAV), 0.6f, 1.f);
			break;
		case 1u:
			audio.playSound(resource.getSound(NANO_2_WAV), 0.6f, 1.f);
			break;
		case 2u:
			audio.playSound(resource.getSound(NANO_3_WAV), 0.6f, 1.f);
			break;
		default:
			break;
	}
}

void NanoRobot::addMapOffset(float x, float y)
{
	setPosition(sf::Vector2f(getPosition().x + x, getPosition().y + y));
}

void NanoRobot::transfertToOcto(void)
{
	PhysicsEngine::getInstance().unregisterShape(m_box);
	m_box = nullptr;
	m_positionBehavior->setRadius(300.f);
	m_swarm.getFirefly(0u).speed = 1.f;
	m_state = Speak;
	m_glowingEffect.onTransfer();
	Progress::getInstance().addNanoRobot();
	playSound();
}

void NanoRobot::setPosition(sf::Vector2f const & position)
{
	sf::Vector2f	pos = position;
	pos.y -= Tile::TripleTileSize * 2.f;
	if (std::abs(pos.x - m_swarm.getFirefly(0u).position.x) > 400.f)
		m_isTravelling = true;
	else
		m_isTravelling = false;
	m_swarm.setTarget(pos);
}

bool NanoRobot::isTravelling(void) const
{
	return m_isTravelling;
}

void NanoRobot::setState(NanoRobot::State state)
{
	m_state = state;
}

void NanoRobot::setTextIndex(std::size_t index)
{
	assert(index < m_texts.size());
	m_textIndex = index;
}

sf::Vector2f const & NanoRobot::getPosition(void) const
{
	return m_swarm.getFirefly(0u).position;
}

sf::Vector2f const & NanoRobot::getTargetPosition(void)
{
	if (std::abs(m_swarm.getTarget().x - m_swarm.getFirefly(0u).position.x) > 400.f)
		m_isTravelling = true;
	else
		m_isTravelling = false;
	return m_swarm.getTarget();
}

NanoRobot::State NanoRobot::getState(void) const
{
	return m_state;
}

void NanoRobot::update(sf::Time frametime)
{
	m_swarm.update(frametime);
	m_sprite.update(frametime);

	sf::Vector2f const & pos = m_swarm.getFirefly(0u).position;
	m_sprite.setPosition(pos - m_sprite.getGlobalSize() / 2.f);

	if (m_box)
	{
		m_box->setPosition(pos.x - m_box->getRadius(), pos.y - m_box->getRadius());
		m_box->update();
	}

	if (m_state == Speak)
	{
		m_timer += frametime;
		if (m_timer > m_timerMax)
			m_state = FollowOcto;
	}

	m_texts[m_textIndex]->setPosition(m_sprite.getPosition() - sf::Vector2f(0.f, 0.f));
	m_texts[m_textIndex]->update(frametime);
	m_glowingEffect.setPosition(pos);
	m_glowingEffect.update(frametime);
}

void NanoRobot::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	if (!m_isTravelling || m_state == FollowOcto || m_state == Speak)
	{
		render.draw(m_glowingEffect);
		render.draw(m_sprite);
	}
}

void NanoRobot::drawText(sf::RenderTarget& render, sf::RenderStates) const
{
	if (!m_isTravelling || m_state == FollowOcto || m_state == Speak)
	{
		if (m_state == Idle || m_state == Speak)
			m_texts[m_textIndex]->draw(render);
	}
}
