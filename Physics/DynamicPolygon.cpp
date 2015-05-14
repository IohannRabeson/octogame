#include "DynamicPolygon.hpp"
#include <cmath>

DynamicPolygon::DynamicPolygon(std::size_t p_verticesCount, CollideType p_collideType, bool p_applyGravity) :
	Polygon(p_verticesCount, p_collideType),
	m_velocity(0.f, 0.f),
	m_applyGravity(p_applyGravity),
	m_collideMask(0)
{}

DynamicPolygon::~DynamicPolygon(void) { }

void DynamicPolygon::rotate(float angle)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float x;
	float y;
	sf::Vector2f center = Polygon::getCenter();
	// rotate vertices, and normals
	for (std::size_t i = 0; i < m_verticesCount; i++)
	{
		x = m_vertices[i].position.x - center.x;
		y = m_vertices[i].position.y - center.y;
		m_vertices[i].position.x = center.x + (x * cos - y * sin);
		m_vertices[i].position.y = center.y + (x * sin + y * cos);
	}
	m_recompute = true;
}

void DynamicPolygon::move(sf::Vector2f const &translation)
{
	// move vertices and edges
	for (std::size_t i = 0; i < m_verticesCount; i++)
	{
		m_vertices[i].position.x += translation.x;
		m_vertices[i].position.y += translation.y;
	}
	m_recomputeCenter = true;
}

sf::Rect<float> const & DynamicPolygon::getGlobalBounds(void)
{
	float minX = 99999999.f;
	float minY = 99999999.f;
	float maxX = -99999999.f;
	float maxY = -99999999.f;
	for (std::size_t i = 0; i < m_verticesCount; i++)
	{
		if (m_vertices[i].position.x < minX)
			minX = m_vertices[i].position.x;
		if (m_vertices[i].position.x > maxX)
			maxX = m_vertices[i].position.x;
		if (m_vertices[i].position.y < minY)
			minY = m_vertices[i].position.y;
		if (m_vertices[i].position.y > maxY)
			maxY = m_vertices[i].position.y;
	}
	m_globalBounds.left = minX;
	m_globalBounds.top = minY;
	m_globalBounds.width = maxX - minX;
	m_globalBounds.height = maxY - minY;
	return m_globalBounds;
}

void DynamicPolygon::applyTransform(void)
{
	move(m_velocity);
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
}
