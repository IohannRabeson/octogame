#include "Polygon.hpp"

Polygon::Polygon(std::size_t p_verticesCount, CollideType p_collideType) :
	m_recompute(true),
	m_recomputeCenter(true),
	m_center(),
	m_verticesCount(p_verticesCount),
	m_collideType(p_collideType)
{
	m_vertices.resize(p_verticesCount);
	m_edges.resize(p_verticesCount);
	m_normals.resize(p_verticesCount);
}

Polygon::~Polygon(void) { }

void Polygon::computeEdges(void)
{
	if (!m_recompute)
		return;
	m_recompute = false;
	for (std::size_t i = 0; i < m_verticesCount; i++)
	{
		m_edges[i].x = m_vertices[i].position.x - m_vertices[(i + 1) % m_verticesCount].position.x;
		m_edges[i].y = m_vertices[i].position.y - m_vertices[(i + 1) % m_verticesCount].position.y;
		m_normals[i].x = -m_edges[i].y;
		m_normals[i].y = m_edges[i].x;
	}
}

void Polygon::computeCenter(void)
{
	if (!m_recomputeCenter)
		return;
	m_recomputeCenter = false;
	m_center.x = 0.f;
	m_center.y = 0.f;
	for (std::size_t i = 0; i < m_verticesCount; i++)
	{
		m_center.x += m_vertices[i].position.x;
		m_center.y += m_vertices[i].position.y;
	}
	m_center.x /= m_verticesCount;
	m_center.y /= m_verticesCount;
}

sf::Vector2f const & Polygon::getVertex(std::size_t index) const
{
	return m_vertices[index].position;
}

sf::Vector2f const & Polygon::getCenter(void)
{
	computeCenter();
	return m_center;
}

sf::Vector2f const & Polygon::getEdge(std::size_t index)
{
	computeEdges();
	return m_edges[index];
}

sf::Vector2f const & Polygon::getNormal(std::size_t index)
{
	computeEdges();
	return m_normals[index];
}
