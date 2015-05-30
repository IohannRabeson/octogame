#include "Star.hpp"
#include "Map.hpp"
#include "../ResourceDefinitions.hpp"
#include <cmath>
#include <ResourceManager.hpp>
#include <Application.hpp>

Star::Star() :
	Decor(),
	mf_shineTimer(0.f),
	mf_maxShineTime(0.f)
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

	// Create light
	float radius = p_size.x / 3.f;
	m_light.setRadius(radius);
	m_light.setOrigin(sf::Vector2f(radius, radius));
	m_light.setPosition(m_origin);
}

void Star::randomDecor(void)
{
	Decor::randomDecor();
	me_currentState = e_state_grow;

	mn_maxTriangle = 6;
	//TODO: Add this parameter im the biome
	mf_maxShineTime = randomRange(1, 20);

	allocateVertex(mn_maxTriangle * 3u);

	m_size = sf::Vector2f(randomRange(m_biome->m_star.mn_minSizeX, m_biome->m_star.mn_maxSizeX), randomRange(m_biome->m_star.mn_minSizeY, m_biome->m_star.mn_maxSizeY));
	m_sizeHeart = sf::Vector2f(m_size.x / 50.f, m_size.x / 50.f);

	// Init light
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	m_light.setTexture(&resources.getTexture(FIREFLY01_PNG));
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

void Star::computeStates(float pf_deltatime)
{
	switch (me_currentState)
	{
		case e_state_grow:
		{
			mf_mouvement += pf_deltatime * mf_growSpeed;
			if (mf_mouvement >= 1.0f)
				me_currentState = e_state_die;
			break;
		}
		case e_state_die:
		{
			mf_mouvement -= pf_deltatime * mf_dieSpeed;
			mf_dieTimer = 0.f;
			break;
		}
		case e_state_stop:
		{
			mf_mouvement = 0.f;
			break;
		}
		default:
			break;
	}
}

void Star::shine(float pf_deltatime)
{
	computeStates(pf_deltatime);
	mf_shineTimer += pf_deltatime;
	//TODO: Find an other solution (this one could depend on fps, not good)
	if (me_currentState == e_state_die && mf_mouvement <= 0.f)
		me_currentState = e_state_stop;
	if (me_currentState == e_state_stop && mf_shineTimer >= mf_maxShineTime)
	{
		randomDecor();
		mf_shineTimer = 0.f;
	}
	if (me_currentState != e_state_stop)
		createOneStar(sf::Vector2f(m_size.x * mf_mouvement, m_size.y * mf_mouvement), sf::Vector2f(m_sizeHeart.x * mf_mouvement, m_sizeHeart.y * mf_mouvement), 0.f, sf::Color(255,255,255, 50));
}

void Star::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);

	if (me_currentState != e_state_stop)
		createOneStar(sf::Vector2f(m_size.x * mf_mouvement, m_size.y * mf_mouvement), sf::Vector2f(m_sizeHeart.x * mf_mouvement, m_sizeHeart.y * mf_mouvement), 0.f, sf::Color(255,255,255, 50));
}

void Star::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	if (me_currentState != e_state_stop)
	{
		Decor::draw(p_target, p_states);
		p_target.draw(m_light);
	}
}
