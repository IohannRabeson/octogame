#include "TileShape.hpp"
#include <limits>
#include <cassert>
#include <Math.hpp>

TileShape::TileShape(void) :
	AShape(),
	m_vertex(nullptr),
	m_baryCenter(),
	m_globalBounds()
{
	m_edges.resize(4u);
	m_normals.resize(4u);
}

void TileShape::computeShape(void)
{
	assert(m_vertex);

	// Compute edges and normals
	m_edges[0u] = m_vertex[0u].position - m_vertex[1u].position;
	//m_edges[1u] = m_vertex[1u].position - m_vertex[2u].position;
	//m_edges[2u] = m_vertex[2u].position - m_vertex[3u].position;
	//m_edges[3u] = m_vertex[3u].position - m_vertex[0u].position;

	m_vertices[1u] = m_vertex[0u].position + sf::Vector2f(0.f, 128.f);
	m_vertices[0u] = m_vertex[1u].position + sf::Vector2f(0.f, 128.f);

	m_normals[0u] = sf::Vector2f(-m_edges[0u].y, m_edges[0u].x);
	m_normals[1u] = sf::Vector2f(1.f, 0.f);
	m_normals[2u] = sf::Vector2f(0.f, 1.f);
	m_normals[3u] = sf::Vector2f(-1.f, 0.f);

	//TODO: finalize
	/*
	for (std::size_t i = 0u; i < 4u; i++)
	{
		m_normals[i].x = -m_edges[i].y;
		m_normals[i].y = m_edges[i].x;
	}*/

	// Compute global bounds
	m_globalBounds.left = m_vertex[0u].position.x;
	m_globalBounds.top = m_vertex[0u].position.y < m_vertex[1u].position.y ? m_vertex[0u].position.y : m_vertex[1u].position.y;
	m_globalBounds.width = 16.f;
	m_globalBounds.height = 144.f;

	// Compute bary center
	m_baryCenter = m_vertex[0u].position;
	for (std::size_t i = 1u; i < 4u; i++)
		m_baryCenter += m_vertex[i].position;
	m_baryCenter /= 4.f;
}

sf::Vector2f const & TileShape::getVertex(std::size_t index) const
{
	if (index > 1u)
		return m_vertices[index - 2u];
	return m_vertex[index].position;
}

sf::Vector2f const & TileShape::getSupportVertex(sf::Vector2f const & direction)
{
	float bestProjection = -std::numeric_limits<float>::max();
	std::size_t index = 0u;
	for (std::size_t i = 0u; i < getVertexCount(); i++)
	{
		float projection = octo::dotProduct(getVertex(i), direction);
		if (projection > bestProjection)
		{
			index = i;
			bestProjection = projection;
		}
	}
	return getVertex(index);
}

void TileShape::debugDraw(sf::RenderTarget & render)
{
	sf::ConvexShape shape;
	shape.setPointCount(4u);
	for (std::size_t i = 0u; i < 4u; i++)
		shape.setPoint(i, getVertex(i));
	render.draw(shape);
	AShape::debugDraw(render);
}
