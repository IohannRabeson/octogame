#include "GameObject.hpp"

GameObject::GameObject(void)  :
	GameObject(300.f, 300.f, nullptr)
{}

GameObject::GameObject(float x, float y, sf::Vertex * p_position) :
	m_origin(x, y),
	m_initialPosition(x, y),
	m_vertexPosition(p_position)
{}

GameObject::~GameObject(void) {}

void GameObject::setPosition(sf::Vector2f const & p_pos)
{
	m_initialPosition = p_pos;
}

sf::Vector2f const & GameObject::getPosition(void) const
{
	return m_initialPosition;
}

void GameObject::updatePosition(sf::Vertex * p_pos)
{
	m_vertexPosition = p_pos;
}
