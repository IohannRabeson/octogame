#include "Pyramid.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include "CharacterOcto.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <ResourceManager.hpp>

Pyramid::Pyramid(sf::Vector2f const & scale, sf::Vector2f const & position, ABiome & biome) :
	InstanceDecor(PYRAMID_OSS, scale, position, 1u),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_particles(new BeamSystem()),
	m_octo(nullptr),
	m_octoCollide(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	sf::Vector2f const & particlePosition = position + sf::Vector2f(getSprite().getLocalSize().x / 2.f, getSprite().getLocalSize().y);
	m_particles->setWidth(100 * 16.f);
	m_particles->setHeight(200 * 16.f);
	m_particles->setBiome(biome);
	m_particles->setPosition(particlePosition);

	m_box->setGameObject(this);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Pyramid));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
	m_box->setRadius(2100.f);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setPosition(getSprite().getPosition() + getSprite().getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.1f), 0u);
	m_animationTop.setFrames(frames);
	m_animationTop.setLoop(octo::LoopMode::Loop);

	m_spriteTop.setSpriteSheet(resources.getSpriteSheet(PYRAMID_TOP_OSS));
	m_spriteTop.setScale(scale);
	m_spriteTop.setPosition(position.x + 600.f, position.y - 384.f);
	m_spriteTop.setAnimation(m_animationTop);
	m_spriteTop.play();

	octo::SpriteAnimation::FrameList	framesWater;
	framesWater.emplace_back(sf::seconds(0.1f), 0u);
	framesWater.emplace_back(sf::seconds(0.1f), 1u);
	m_animationWater.setFrames(framesWater);
	m_animationWater.setLoop(octo::LoopMode::Loop);

	m_spriteWater.setSpriteSheet(resources.getSpriteSheet(PYRAMID_WATER_OSS));
	m_spriteWater.setScale(scale);
	m_spriteWater.setPosition(position.x + 460.f, position.y - 32.f);
	m_spriteWater.setAnimation(m_animationWater);
	m_spriteWater.play();

	m_spriteBlocs.resize(18);
	m_animationBlocs.resize(18);
	std::size_t i = 0u;
	for (i = 0u; i < 11u; i++)
	{
		octo::SpriteAnimation::FrameList	frames;
		frames.emplace_back(sf::seconds(2.0f), 0u);
		frames.emplace_back(sf::seconds(0.1f), 1u);
		frames.emplace_back(sf::seconds(0.1f), 2u);
		frames.emplace_back(sf::seconds(0.1f), 3u);
		frames.emplace_back(sf::seconds(0.1f), 4u);
		frames.emplace_back(sf::seconds(0.1f), 5u);
		frames.emplace_back(sf::seconds(0.1f), 6u);
		frames.emplace_back(sf::seconds(0.1f), 7u);
		frames.emplace_back(sf::seconds(0.1f), 8u);
		frames.emplace_back(sf::seconds(0.1f), 9u);
		m_animationBlocs[i].setFrames(frames);
		m_animationBlocs[i].setLoop(octo::LoopMode::Loop);

		m_spriteBlocs[i].setSpriteSheet(resources.getSpriteSheet(PYRAMID_BLOC_1_OSS));
		m_spriteBlocs[i].setScale(sf::Vector2f(0.98f, 0.98f));
		m_spriteBlocs[i].setAnimation(m_animationBlocs[i]);
		m_spriteBlocs[i].play();
	}

	for (; i < 13u; i++)
	{
		octo::SpriteAnimation::FrameList	frames;
		frames.emplace_back(sf::seconds(2.1f), 0u);
		frames.emplace_back(sf::seconds(0.1f), 1u);
		frames.emplace_back(sf::seconds(0.1f), 2u);
		frames.emplace_back(sf::seconds(0.1f), 3u);
		frames.emplace_back(sf::seconds(0.1f), 4u);
		frames.emplace_back(sf::seconds(0.1f), 5u);
		frames.emplace_back(sf::seconds(0.1f), 6u);
		frames.emplace_back(sf::seconds(0.1f), 7u);
		m_animationBlocs[i].setFrames(frames);
		m_animationBlocs[i].setLoop(octo::LoopMode::Loop);

		m_spriteBlocs[i].setSpriteSheet(resources.getSpriteSheet(PYRAMID_BLOC_2_OSS));
		m_spriteBlocs[i].setScale(scale);
		m_spriteBlocs[i].setScale(sf::Vector2f(0.98f, 0.98f));
		m_spriteBlocs[i].setAnimation(m_animationBlocs[i]);
		m_spriteBlocs[i].play();
	}

	for (; i < 18u; i++)
	{
		octo::SpriteAnimation::FrameList	frames;
		frames.emplace_back(sf::seconds(0.1f), 4u);
		frames.emplace_back(sf::seconds(0.1f), 3u);
		frames.emplace_back(sf::seconds(0.1f), 2u);
		frames.emplace_back(sf::seconds(0.1f), 1u);
		frames.emplace_back(sf::seconds(2.1f), 0u);
		m_animationBlocs[i].setFrames(frames);
		m_animationBlocs[i].setLoop(octo::LoopMode::Loop);

		m_spriteBlocs[i].setSpriteSheet(resources.getSpriteSheet(PYRAMID_BLOC_3_OSS));
		m_spriteBlocs[i].setScale(scale);
		m_spriteBlocs[i].setScale(sf::Vector2f(0.98f, 0.98f));
		m_spriteBlocs[i].setAnimation(m_animationBlocs[i]);
		m_spriteBlocs[i].play();
	}

	m_spriteBlocs[0u].setPosition(position.x + 337.f, position.y + 257.f);
	m_spriteBlocs[1u].setPosition(position.x + 605.f, position.y + 321.f);
	m_spriteBlocs[2u].setPosition(position.x + 813.f, position.y + 513.f);
	m_spriteBlocs[3u].setPosition(position.x + 809.f, position.y + 705.f);
	m_spriteBlocs[4u].setPosition(position.x + 493.f, position.y + 961.f);
	m_spriteBlocs[5u].setPosition(position.x + 181.f, position.y + 961.f);
	m_spriteBlocs[6u].setPosition(position.x + 53.f, position.y + 961.f);
	m_spriteBlocs[7u].setPosition(position.x + 1213.f, position.y + 961.f);
	m_spriteBlocs[8u].setPosition(position.x + 1249.f, position.y + 577.f);
	m_spriteBlocs[9u].setPosition(position.x + 1185.f, position.y + 513.f);
	m_spriteBlocs[10u].setPosition(position.x + 901.f, position.y + 897.f);

	m_spriteBlocs[11u].setPosition(position.x + 445.f, position.y + 897.f);
	m_spriteBlocs[12u].setPosition(position.x + 1085.f, position.y + 961.f);

	m_spriteBlocs[13u].setPosition(position.x + 929.f, position.y + 1.f);
	m_spriteBlocs[14u].setPosition(position.x + 1073.f, position.y + 193.f);
	m_spriteBlocs[15u].setPosition(position.x + 1377.f, position.y + 897.f);
	m_spriteBlocs[16u].setPosition(position.x + 873.f, position.y + 641.f);
	m_spriteBlocs[17u].setPosition(position.x + 401.f, position.y + 193.f);
}

void Pyramid::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_spriteTop.setPosition(m_spriteTop.getPosition().x + x, m_spriteTop.getPosition().y + y);
	m_spriteWater.setPosition(m_spriteWater.getPosition().x + x, m_spriteWater.getPosition().y + y);
	for (std::size_t i = 0u; i < m_spriteBlocs.size(); i++)
		m_spriteBlocs[i].setPosition(m_spriteBlocs[i].getPosition().x + x, m_spriteBlocs[i].getPosition().y + y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
}

void Pyramid::setPosition(sf::Vector2f const & position)
{
	Pyramid::setPosition(position);
	m_spriteTop.setPosition(position);
	m_spriteWater.setPosition(position);
	for (std::size_t i = 0u; i < m_spriteBlocs.size(); i++)
		m_spriteBlocs[i].setPosition(position);
	m_box->setPosition(getSprite().getPosition() + getSprite().getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();
}

void Pyramid::collideOcto(CharacterOcto * octo)
{
	m_octo = octo;
	m_octoCollide = true;
}

void Pyramid::update(sf::Time frameTime)
{
	m_particles->update(frameTime);
	InstanceDecor::update(frameTime);
	m_spriteTop.update(frameTime);
	m_spriteWater.update(frameTime);
	for (std::size_t i = 0u; i < m_spriteBlocs.size(); i++)
		m_spriteBlocs[i].update(frameTime);

	if (m_octo)
		m_octo->enableCutscene(m_octoCollide, false);
	m_octoCollide = false;
}

bool Pyramid::isInScreen(void)
{
	octo::Camera const & camera = octo::Application::getCamera();
	sf::FloatRect rect = m_sprite.getGlobalBounds();
	rect.top -= 2000.f;
	rect.height += 2000.f;

	if (camera.getRectangle().intersects(rect))
		return true;
	return false;
}

void Pyramid::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	m_spriteTop.draw(render, states);
}

void Pyramid::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::draw(render, states);
	for (std::size_t i = 0u; i < m_spriteBlocs.size(); i++)
		m_spriteBlocs[i].draw(render, states);
	m_spriteWater.draw(render, states);
	m_particles->draw(render, states);
}
