#include "RepairNanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Math.hpp>

RepairNanoRobot::RepairNanoRobot(void) :
	NanoRobot(sf::Vector2f(200.f, 800.f), NANO_REPAIR_OSS, 8, 12542),
	m_ray(new sf::Vertex[16]),
	m_texture(nullptr),
	m_state(None)
{
	setup(this);
	m_texture = &octo::Application::getResourceManager().getTexture(STARGRADIENT_PNG);

	sf::Color color = sf::Color::Red;

	for (std::size_t i = 0u; i < 16u; i++)
		m_ray[i].color = color;

	m_ray[1].color.a = 0;
	m_ray[2].color.a = 0;
	m_ray[4].color.a = 0;
	m_ray[7].color.a = 0;

	m_ray[12].color = sf::Color(255, 165, 0);
	m_ray[13].color = sf::Color(255, 165, 0);
	m_ray[14].color = sf::Color(255, 165, 0);
	m_ray[15].color = sf::Color(255, 165, 0);

	sf::Vector2f texSize(m_texture->getSize());
	m_ray[8].texCoords = sf::Vector2f(0.f, 0.f);
	m_ray[9].texCoords = sf::Vector2f(texSize.x, 0.f);
	m_ray[10].texCoords = texSize;
	m_ray[11].texCoords = sf::Vector2f(0.f, texSize.y);

	m_ray[12].texCoords = sf::Vector2f(0.f, 0.f);
	m_ray[13].texCoords = sf::Vector2f(texSize.x, 0.f);
	m_ray[14].texCoords = texSize;
	m_ray[15].texCoords = sf::Vector2f(0.f, texSize.y);

	m_particles.setColor(color);
}

void RepairNanoRobot::setTarget(sf::Vector2f const & target)
{
	m_target = target;
}

void RepairNanoRobot::setState(RepairNanoRobot::State state)
{
	m_state = state;
}

void RepairNanoRobot::makeLaser(sf::Vertex* vertices, sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness)
{
	static float const size = 8.f;
	sf::Vector2f	p(-(p1.y - p0.y), p1.x - p0.x);
	float			halfThickness = thickness * .5f;

	octo::normalize(p);
	p *= halfThickness;

	vertices[0].position = p0;
	vertices[1].position = p0 + p;
	vertices[2].position = p1 + p;
	vertices[3].position = p1;

	vertices[4].position = p0 - p;
	vertices[5].position = p0;
	vertices[6].position = p1;
	vertices[7].position = p1 - p;

	vertices[8].position = p0 + sf::Vector2f(-size, -size);
	vertices[9].position = p0 + sf::Vector2f(size, -size);
	vertices[10].position = p0 + sf::Vector2f(size, size);
	vertices[11].position = p0 + sf::Vector2f(-size, size);

	vertices[12].position = p1 + sf::Vector2f(-size, -size);
	vertices[13].position = p1 + sf::Vector2f(size, -size);
	vertices[14].position = p1 + sf::Vector2f(size, size);
	vertices[15].position = p1 + sf::Vector2f(-size, size);
}

void RepairNanoRobot::update(sf::Time frameTime)
{
	NanoRobot::update(frameTime);

	m_particles.canEmit(false);
	if (m_state == Repair)
	{
		makeLaser(m_ray.get(), getPosition() + sf::Vector2f(-2.f, 16.f), m_target, 4.f);
		m_particles.canEmit(true);
		m_particles.setPosition(m_target);
	}
	m_particles.update(frameTime);
}

void RepairNanoRobot::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	NanoRobot::draw(render, states);
	if ((m_state == Repair) && (!isTravelling() || getState() == FollowOcto || getState() == Speak))
	{
		render.draw(m_ray.get(), 8, sf::Quads);
		render.draw(m_ray.get() + 8, 8u, sf::Quads, m_texture);
	}
	m_particles.draw(render);
}
