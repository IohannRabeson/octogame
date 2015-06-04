#include "Decor.hpp"
#include "Map.hpp"
#include <cmath>

Decor::Decor(void) :
	GameObject(),
	mn_countTriangle(0u),
	me_currentState(e_state_sleep),
	m_size(50.f, 50.f),
	m_color(sf::Color(0, 0, 0)),
	mf_dieTimer(0.f),
	mf_liveTime(0.f),
	mf_mouvement(0.f)
{
	mf_timer = randomRange(0, 100) / 100.f;
	//TODO: put it somewhere else
	srand(time(NULL));
}

Decor::~Decor(void)
{
}

void Decor::rotateVec(sf::Vector2f *p_point, float p_cosAngle, float p_sinAngle)
{
	sf::Vector2f tmp;
	tmp.x = p_point->x * p_cosAngle - p_point->y * p_sinAngle;
	tmp.y = p_point->y * p_cosAngle + p_point->x * p_sinAngle;
	*p_point = tmp;
}

float Decor::randomRange(int pn_min, int pn_max)
{
	if (pn_max - pn_min == 0)
		return pn_max;
	return static_cast<float>(random() % (pn_max - pn_min) + pn_min);
}

float Decor::randomRangeTile(int pn_min, int pn_max)
{
	int tmp = randomRange(pn_min, pn_max);
	tmp -= tmp % 16;
	return static_cast<float>(tmp);
}

void Decor::createVertex(sf::Vector2f p_pos, sf::Color const & p_color, int * pn_count)
{
	m_triangle[*pn_count].position = p_pos;
	m_triangle[*pn_count].color = p_color;
	(*pn_count)++;
}

void Decor::createTriangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_origin, sf::Color const & p_color)
{
	createVertex(p_origin + p_a, p_color, &mn_countTriangle);
	createVertex(p_origin + p_b, p_color, &mn_countTriangle);
	createVertex(p_origin + p_c, p_color, &mn_countTriangle);
}

void Decor::createRectangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_d, sf::Vector2f const & p_origin, sf::Color const & p_color)
{
	createTriangle(p_a, p_b, p_c, p_origin, p_color);
	createTriangle(p_c, p_d, p_a, p_origin, p_color);
}

void Decor::computeStates(float pf_deltatime)
{
	switch (me_currentState)
	{
		case e_state_grow:
		{
			mf_mouvement += pf_deltatime / 4;
			if (mf_mouvement >= 1.0f)
				me_currentState = e_state_sleep;
			break;
		}
		case e_state_die:
		{
			mf_mouvement -= pf_deltatime / 8;
			if (mf_mouvement <= 0.0f)
				randomDecor();
			mf_dieTimer = 0.f;
			break;
		}
		case e_state_sleep:
		{
			mf_timer += pf_deltatime;
			mf_dieTimer += pf_deltatime;
			if (mf_timer > 4.0f)
				mf_timer = 0.0f;
			if (mf_dieTimer >= mf_liveTime && mf_liveTime != 0.f)
				me_currentState = e_state_die;
			else if (mf_timer > 2.0f)
				mf_mouvement += pf_deltatime / (20 + (mf_timer - 2.0f) * 10);
				else
			mf_mouvement -= pf_deltatime / (20 + mf_timer * 10);
			break;
		}
		default:
			break;
	}
}

sf::Vertex * Decor::getTriangle(void)
{
	return m_triangle.get();
}

int Decor::getCount(void)
{
	return mn_countTriangle;
}

void Decor::randomDecor(void)
{
	mf_mouvement = 0.f;
	me_currentState = e_state_grow;
}

void Decor::init(Biome * p_biome)
{
	m_biome = p_biome;
}

void Decor::update(float pf_deltatime)
{
	computeStates(pf_deltatime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		randomDecor();
	m_color = m_vertexPosition->color;
	m_origin.x = m_vertexPosition->position.x;
	m_origin.y = m_vertexPosition->position.y;
}

void Decor::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	p_target.draw(m_triangle.get(), mn_countTriangle, sf::Triangles, p_states);
}
