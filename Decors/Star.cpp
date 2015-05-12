#include "Star.hpp"
#include "../ResourceDefinitions.hpp"
#include <cmath>
#include <ResourceManager.hpp>
#include <Application.hpp>

Star::Star() :
	Decor(),
	mf_timer(0.0f),
	mf_mouvement(0.01f)
{
}

Star::~Star()
{
}

void Star::randomDecor(void) {}

void Star::randomStar(sf::ConvexShape * p_star)
{
	p_star->setPointCount(8);
	m_sizeHeart = sf::Vector2f(10.0f, 10.0f);
	m_sizeRay = sf::Vector2f(randomRange(minX, maxX), randomRange(minY, maxY));
	p_star->setPoint(0, sf::Vector2f(m_origin.x - m_sizeHeart.x / 2.0f - m_sizeRay.x, m_origin.y));
	p_star->setPoint(1, sf::Vector2f(m_origin.x - m_sizeHeart.x / 2.0f, m_origin.y - m_sizeHeart.y / 2.0f));
	p_star->setPoint(2, sf::Vector2f(m_origin.x, m_origin.y - m_sizeHeart.y / 2.0f - m_sizeRay.y));
	p_star->setPoint(3, sf::Vector2f(m_origin.x + m_sizeHeart.x / 2.0f, m_origin.y - m_sizeHeart.y / 2.0f));
	p_star->setPoint(4, sf::Vector2f(m_origin.x + m_sizeHeart.x / 2.0f + m_sizeRay.x, m_origin.y));
	p_star->setPoint(5, sf::Vector2f(m_origin.x + m_sizeHeart.x / 2.0f, m_origin.y + m_sizeHeart.y / 2.0f));
	p_star->setPoint(6, sf::Vector2f(m_origin.x, m_origin.y + m_sizeHeart.y / 2.0f + m_sizeRay.y));
	p_star->setPoint(7, sf::Vector2f(m_origin.x - m_sizeHeart.x / 2.0f, m_origin.y + m_sizeHeart.y / 2.0f));

	p_star->setOrigin(m_origin);
	p_star->setFillColor(sf::Color(255, 255, 255, 127));
	p_star->setPosition(m_origin);

	mf_mouvement = 0.01f;
}

void Star::randomLight(void)
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();

	float radius = m_sizeHeart.x + m_sizeRay.x;
	m_light.setTexture(&resources.getTexture(STARGRADIENT_PNG));
	m_light.setRadius(radius);
	m_light.setOrigin(sf::Vector2f(radius, radius));
	m_light.setPosition(m_origin);
}

void Star::init(Biome * p_biome)
{
	Decor::init(p_biome);
	m_origin = sf::Vector2f(1000.0f, 500.0f);
	m_origin = sf::Vector2f(300, 300);
	m_color = sf::Color(randomRange(20, 200), randomRange(20, 200), randomRange(20, 200));
	randomStar(&m_starFront);
	randomStar(&m_starBack);
	randomLight();
}

void Star::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	m_origin = sf::Vector2f(300, 300);

	mf_timer += pf_deltatime;
	if (mf_timer > 1.0f)
	{
		mf_timer = 0.0f;
		randomStar(&m_starFront);
		randomStar(&m_starBack);
		m_starBack.rotate(45.0f);
	}
}

void Star::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	Decor::draw(p_target, p_states);

	p_target.draw(m_light);
	p_target.draw(m_starFront);
	p_target.draw(m_starBack);

}

