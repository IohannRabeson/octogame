#include "PolygonShape.hpp"
#include <limits>
#include <cassert>
#include <Math.hpp>

PolygonShape::PolygonShape(void) :
	AShape(),
	m_baryCenter(),
	m_globalBounds()
{
	m_initialVertices.reserve(MaxVertices);
	m_vertices.reserve(MaxVertices);
	m_edges.reserve(MaxVertices);
	m_normals.reserve(MaxVertices);
}

void PolygonShape::computeShape(void)
{
	// Compute rotation
	float cos = std::cos(getRotation());
	float sin = std::sin(getRotation());
	float x;
	float y;
	sf::Vector2f const & origin = getOrigin();
	for (std::size_t i = 0u; i < m_initialVertices.size(); i++)
	{
		x = m_initialVertices[i].x - origin.x;
		y = m_initialVertices[i].y - origin.y;
		m_vertices[i].x = origin.x + (x * cos - y * sin) + getPosition().x;
		m_vertices[i].y = origin.y + (x * sin + y * cos) + getPosition().y;
	}

	// Compute edges and normals
	for (std::size_t i = 0u; i < m_initialVertices.size(); i++)
	{
		m_edges[i].x = m_vertices[i].x - m_vertices[(i + 1u) % m_vertices.size()].x;
		m_edges[i].y = m_vertices[i].y - m_vertices[(i + 1u) % m_vertices.size()].y;
		m_normals[i].x = -m_edges[i].y;
		m_normals[i].y = m_edges[i].x;
	}

	// Compute bounding box
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();
	for (std::size_t i = 0u; i < m_initialVertices.size(); i++)
	{
		if (m_vertices[i].x < minX)
			minX = m_vertices[i].x;
		if (m_vertices[i].x > maxX)
			maxX = m_vertices[i].x;
		if (m_vertices[i].y < minY)
			minY = m_vertices[i].y;
		if (m_vertices[i].y > maxY)
			maxY = m_vertices[i].y;
	}
	m_globalBounds.left = minX;
	m_globalBounds.top = minY;
	m_globalBounds.width = maxX - minX;
	m_globalBounds.height = maxY - minY;

	// Compute bary center
	m_baryCenter = m_vertices[0u];
	for (std::size_t i = 1u; i < m_vertices.size(); i++)
		m_baryCenter += m_vertices[i];
	m_baryCenter.x /= static_cast<float>(m_vertices.size());
	m_baryCenter.y /= static_cast<float>(m_vertices.size());
}

sf::Vector2f const & PolygonShape::getSupportVertex(sf::Vector2f const & direction)
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

void PolygonShape::setVertexCount(std::size_t vertexCount)
{
	m_initialVertices.resize(vertexCount);
	m_vertices.resize(vertexCount);
	m_edges.resize(vertexCount);
	m_normals.resize(vertexCount);
}

void PolygonShape::setVertex(std::size_t index, sf::Vector2f const & vector)
{
	assert(index < MaxVertices);
	m_initialVertices[index] = vector;
	m_vertices[index] = vector;
	computeShape();
}

void PolygonShape::update(void)
{
	AShape::update();
	// We recompute all data once per frame
	computeShape();
}

void PolygonShape::debugDraw(sf::RenderTarget & render)
{
	sf::ConvexShape shape;
	shape.setPointCount(m_initialVertices.size());
	for (std::size_t i = 0u; i < m_initialVertices.size(); i++)
		shape.setPoint(i, getVertex(i));
	render.draw(shape);
	AShape::debugDraw(render);
}
