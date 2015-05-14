#include "Player.hpp"

Player::Player(void) :
	DynamicPolygon(4u)
{
	sf::Vertex ver;
	ver.position = sf::Vector2f(300.f, 360.f);
	ver.texCoords = sf::Vector2f(20.f, 0.f);
	setVertex(0, ver);
	ver.position = sf::Vector2f(360.f, 360.f);
	ver.texCoords = sf::Vector2f(595.f, 0.f);
	setVertex(1, ver);
	ver.position = sf::Vector2f(360.f, 450.f);
	ver.texCoords = sf::Vector2f(595.f, 785.f);
	setVertex(2, ver);
	ver.position = sf::Vector2f(300.f, 450.f);
	ver.texCoords = sf::Vector2f(20.f, 785.f);
	setVertex(3, ver);
}

Player::~Player(void)
{
	
}

void Player::updateVelocity(float pf_deltatime)
{
	float speed = 300.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
		m_velocity.y = -speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		m_velocity.y = speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		m_velocity.x = -speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		m_velocity.x = speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		rotate(3.14f * pf_deltatime);

	// Gravity
	//m_velocity.x += speed / 2.f;
	m_velocity.y += speed * 2.f;
	m_velocity *= pf_deltatime;
}
