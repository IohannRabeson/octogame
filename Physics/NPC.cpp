#include "NPC.hpp"

NPC::NPC(void) :
	DynamicPolygon(4u, CollideType::e_pnj, true),
	m_state(NPCState::e_waiting),
	m_speed(500.f),
	m_timerAcceleration(0.f),
	m_timerMaxAcceleration(0.3f),
	m_timerJump(0.f),
	m_timerJumpMax(1.f),
	m_jumpSpeed(1500.f)
{
	m_collideMask = CollideType::e_tile;
	sf::Vertex ver;
	ver.position = sf::Vector2f(700.f, 760.f);
	ver.texCoords = sf::Vector2f(20.f, 0.f);
	setVertex(0, ver);
	ver.position = sf::Vector2f(760.f, 760.f);
	ver.texCoords = sf::Vector2f(595.f, 0.f);
	setVertex(1, ver);
	ver.position = sf::Vector2f(760.f, 850.f);
	ver.texCoords = sf::Vector2f(595.f, 785.f);
	setVertex(2, ver);
	ver.position = sf::Vector2f(700.f, 850.f);
	ver.texCoords = sf::Vector2f(20.f, 785.f);
	setVertex(3, ver);
}

NPC::~NPC(void)
{
	
}

void NPC::update(float pf_deltatime)
{
	int move = 0;

	// Movement left/right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		move = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		move = 1;

	// Jump
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && m_state != e_jumping && m_state != e_falling)
	{
		m_timerJump = m_timerJumpMax;
		m_state = e_jumping;
	}

	if (m_state == e_jumping)
	{
		m_timerJump -= pf_deltatime;
		if (m_timerJump <= 0.f)
		{
			m_timerJump = 0.f;
			m_state = e_falling;
		}
	}

	if (move)
	{
		m_timerAcceleration += pf_deltatime;
		if (m_timerAcceleration >= m_timerMaxAcceleration)
			m_timerAcceleration = m_timerMaxAcceleration;
	}
	else
		m_timerAcceleration = 0.f;
	m_velocity.x = m_speed * m_timerAcceleration / m_timerMaxAcceleration * static_cast<float>(move) * pf_deltatime;
	m_velocity.y = -m_jumpSpeed * m_timerJump / m_timerJumpMax * pf_deltatime;
}

void NPC::onCollision(Polygon * p_polygon)
{
	if (dynamic_cast<Tile*>(p_polygon))
	{
		if (m_state == e_falling)
			m_state = e_waiting;
	}
}
