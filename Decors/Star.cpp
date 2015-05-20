#include "Star.hpp"
#include "Map.hpp"
#include "../ResourceDefinitions.hpp"
#include <cmath>
#include <ResourceManager.hpp>
#include <Application.hpp>

Star::Star() :
	Decor()
{
}

Star::~Star()
{
}

void Star::createOneStar(sf::Vector2f p_size, sf::Vector2f p_sizeHeart, float p_angle, sf::Color p_color)
{
	mn_countTriangle = 0u;

	sf::Vector2f left(-p_size.x / 2.f - p_sizeHeart.x, 0.f);
	sf::Vector2f right(p_size.x / 2.f + p_sizeHeart.x, 0.f);
	sf::Vector2f up(0.f, -p_size.y / 2.f - p_sizeHeart.y);
	sf::Vector2f down(0.f, p_size.y / 2.f + p_sizeHeart.y);
	sf::Vector2f heartLeftUp(-p_sizeHeart.x, -p_sizeHeart.y);
	sf::Vector2f heartRightUp(p_sizeHeart.x, -p_sizeHeart.y);
	sf::Vector2f heartLeftDown(-p_sizeHeart.x, p_sizeHeart.y);
	sf::Vector2f heartRightDown(p_sizeHeart.x, p_sizeHeart.y);

	float angle = p_angle * PI / 180.f;
	float cosA = cos(angle);
	float sinA = sin(angle);

	rotateVec(&left, cosA, sinA);
	rotateVec(&right, cosA, sinA);
	rotateVec(&up, cosA, sinA);
	rotateVec(&down, cosA, sinA);
	rotateVec(&heartLeftUp, cosA, sinA);
	rotateVec(&heartRightUp, cosA, sinA);
	rotateVec(&heartLeftDown, cosA, sinA);
	rotateVec(&heartRightDown, cosA, sinA);

	createTriangle(up, heartLeftUp, heartRightUp, m_origin, p_color);
	createTriangle(down, heartLeftDown, heartRightDown, m_origin, p_color);
	createTriangle(left, heartLeftUp, heartLeftDown, m_origin, p_color);
	createTriangle(right, heartRightUp, heartRightDown, m_origin, p_color);
	createRectangle(heartLeftUp, heartRightUp, heartRightDown, heartLeftDown, m_origin, p_color);

	createLight(p_size);
}

void Star::createLight(sf::Vector2f p_size)
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();

	float radius = p_size.x / 3.f;
	m_light.setTexture(&resources.getTexture(FIREFLY01_PNG));
	m_light.setRadius(radius);
	m_light.setOrigin(sf::Vector2f(radius, radius));
	m_light.setPosition(m_origin);
}

void Star::randomDecor(void)
{
	Decor::randomDecor();
	me_currentState = e_state_stop;

	mn_maxTriangle = 6;
	mf_liveTime = 0.01;

	allocateVertex(mn_maxTriangle * 3u);

	m_size = sf::Vector2f(randomRange(m_biome->m_star.mn_minSizeX, m_biome->m_star.mn_maxSizeX), randomRange(m_biome->m_star.mn_minSizeY, m_biome->m_star.mn_maxSizeY));
	m_sizeHeart = sf::Vector2f(m_size.x / 50.f, m_size.x / 50.f);
}

void Star::setOrigin(sf::Vector2f p_origin)
{
	m_origin = p_origin;
}

void Star::init(Biome * p_biome)
{
	Decor::init(p_biome);
	mf_growSpeed = 2.f;
	mf_dieSpeed = 2.f;
	randomDecor();
	createOneStar(m_size, m_sizeHeart, 0.0f, m_color);
}

void Star::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	//TODO: Find an other solution (this one could depend on fps, not good)
	if (me_currentState == e_state_die && mf_mouvement <= 0.01f)
		me_currentState = e_state_stop;
	if (me_currentState == e_state_stop && randomRange(1, 1000) >= 998)
		me_currentState = e_state_grow;

	createOneStar(sf::Vector2f(m_size.x * mf_mouvement, m_size.y * mf_mouvement), sf::Vector2f(m_sizeHeart.x * mf_mouvement, m_sizeHeart.y * mf_mouvement), 0.f, sf::Color(255,255,255, 50));
}

void Star::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	Decor::draw(p_target, p_states);
	p_target.draw(m_light);
}

