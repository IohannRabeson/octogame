#include "Decor.hpp"
#include "Map.hpp"
#include <cmath>

Decor::Decor(void) :
	GameObject(),
	mn_countVertex(0u),
	mn_maxTriangle(0u),
	b_isIce(false),
	me_currentState(e_state_sleep),
	m_size(50.f, 50.f),
	m_color(sf::Color(0, 0, 0)),
	m_biome(nullptr),
	mf_timer(0.f),
	mf_maxTimer(4.f),
	mf_dieTimer(0.f),
	mf_liveTime(0.f),
	mf_mouvement(0.f),
	mf_growSpeed(0.25f),
	mf_dieSpeed(0.125f)
{
	mf_timer = randomRange(0, 100) / 100.f;
	//TODO: put it somewhere else
	srand(time(NULL));
	m_triangle = new sf::VertexArray(sf::Triangles);
}

Decor::~Decor(void)
{
	m_triangle->clear();
}

void Decor::rotateVec(sf::Vector2f *p_vector, float const & p_cosAngle, float const & p_sinAngle)
{
	float x = p_vector->x * p_cosAngle - p_vector->y * p_sinAngle;
	p_vector->y = p_vector->y * p_cosAngle + p_vector->x * p_sinAngle;
	p_vector->x = x;
}

float Decor::randomRange(int pn_min, int pn_max)
{
	if (pn_max - pn_min == 0)
		return pn_max;
	std::uniform_int_distribution<int> distribution(pn_min, pn_max);
	std::random_device rd;
	std::mt19937 engine(rd());

	return distribution(engine);
	//return static_cast<float>(random() % (pn_max - pn_min) + pn_min);
}

void Decor::createVertex(sf::Vector2f p_pos, sf::Color const & p_color, int * pn_count)
{
	(*m_triangle)[*pn_count].position = p_pos;
	(*m_triangle)[*pn_count].color = p_color;
	(*pn_count)++;
}

void Decor::createTriangle(sf::Vector2f const & p_a, sf::Vector2f const & p_b, sf::Vector2f const & p_c, sf::Vector2f const & p_origin, sf::Color const & p_color)
{
	createVertex(p_origin + p_a, p_color, &mn_countVertex);
	createVertex(p_origin + p_b, p_color, &mn_countVertex);
	createVertex(p_origin + p_c, p_color, &mn_countVertex);
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
				mf_mouvement += pf_deltatime * mf_growSpeed;
				if (mf_mouvement >= 1.0f)
					me_currentState = e_state_sleep;
				break;
			}
		case e_state_die:
			{
				mf_mouvement -= pf_deltatime * mf_dieSpeed;
				if (mf_mouvement <= 0.0f)
					randomDecor();
				mf_dieTimer = 0.f;
				break;
			}
		case e_state_sleep:
			{
				mf_timer += pf_deltatime;
				mf_dieTimer += pf_deltatime;
				if (mf_timer > mf_maxTimer)
					mf_timer = 0.0f;
				if (mf_dieTimer >= mf_liveTime && mf_liveTime != 0.f)
					me_currentState = e_state_die;
				else if (mf_timer > mf_maxTimer / 2.f)
					mf_mouvement += pf_deltatime / (20 + (mf_timer - 2.0f) * 10);
				else
					mf_mouvement -= pf_deltatime / (20 + mf_timer * 10);
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

void Decor::init(Biome * p_biome)
{
	m_biome = p_biome;
}

void Decor::allocateVertex(int pn_count)
{
	//m_triangle.reset(new sf::Vertex[pn_count]);
	m_triangle->resize(pn_count);
}

void Decor::randomDecor(void)
{
	mf_mouvement = 0.f;
	me_currentState = e_state_grow;
}

float Decor::getOriginX(void) const
{
	return m_origin.x;
}

void Decor::putOnMap(void)
{
	m_color = m_vertexPosition->color;
	m_origin.x = m_vertexPosition->position.x;
	m_origin.y = m_vertexPosition->position.y;
}

void Decor::updateOrigin(float pf_deltatime)
{
	computeStates(pf_deltatime);
	m_origin.x = m_vertexPosition->position.x;
	//	m_origin.y = m_vertexPosition->position.y;
}

void Decor::iceDecor(void)
{
	if (b_isIce == true)
		b_isIce = false;
	else if (b_isIce == false)
		b_isIce = true;
}

void Decor::update(float pf_deltatime)
{
	computeStates(pf_deltatime);
}

void Decor::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
	p_target.draw(*m_triangle, p_states);
}
