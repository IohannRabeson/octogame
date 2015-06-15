#include "PolygonShape.hpp"
#include <limits>
#include <Math.hpp>

PolygonShape::PolygonShape(void) :
	AShape(),
	m_center(),
	m_globalBounds()
{
	// TODO: ask iohann what is the best
	m_vertices.reserve(1000u);
	m_initialVertices.reserve(1000u);
	m_edges.reserve(1000u);
	m_normals.reserve(1000u);
}

void PolygonShape::computeEdges(void)
{
	//m_recomputeEdges = false;
	for (std::size_t i = 0; i < m_vertices.size(); i++)
		m_initialVertices[i] = m_vertices[i] + getPosition();
	computeRotation();
	for (std::size_t i = 0; i < m_vertices.size(); i++)
	{
		m_edges[i].x = m_initialVertices[i].x - m_initialVertices[(i + 1) % m_initialVertices.size()].x;
		m_edges[i].y = m_initialVertices[i].y - m_initialVertices[(i + 1) % m_initialVertices.size()].y;
		m_normals[i].x = -m_edges[i].y;
		m_normals[i].y = m_edges[i].x;
	}
}

sf::Rect<float> const & PolygonShape::getGlobalBounds(void)
{
	//TODO: ask io
	//if (m_recomputeGlobalBounds)
	{
		//m_recomputeGlobalBounds = false;
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = -std::numeric_limits<float>::max();
		float maxY = -std::numeric_limits<float>::max();
		for (std::size_t i = 0; i < m_vertices.size(); i++)
		{
			if (m_initialVertices[i].x < minX)
				minX = m_initialVertices[i].x;
			if (m_initialVertices[i].x > maxX)
				maxX = m_initialVertices[i].x;
			if (m_initialVertices[i].y < minY)
				minY = m_initialVertices[i].y;
			if (m_initialVertices[i].y > maxY)
				maxY = m_initialVertices[i].y;
		}
		m_globalBounds.left = minX;
		m_globalBounds.top = minY;
		m_globalBounds.width = maxX - minX;
		m_globalBounds.height = maxY - minY;
	}
	return m_globalBounds;
}

sf::Vector2f const & PolygonShape::getEdge(std::size_t index)
{
	//if (m_recomputeEdges)
		computeEdges();
	return m_edges[index];
}

sf::Vector2f const & PolygonShape::getNormal(std::size_t index)
{
	//if (m_recomputeEdges)
		computeEdges();
	return m_normals[index];
}

sf::Vector2f const & PolygonShape::getVertex(std::size_t index)
{
	//TODO: recompute
	for (std::size_t i = 0; i < m_vertices.size(); i++)
		m_initialVertices[i] = m_vertices[i] + getPosition();
	if (needRecompute())
		computeRotation();
	return m_initialVertices[index];
}

sf::Vector2f const & PolygonShape::getCenter(void)
{
	//TODO: recompute
	m_center.x = 0.f;
	m_center.y = 0.f;
	for (std::size_t i = 0; i < m_initialVertices.size(); i++)
		m_center += m_initialVertices[i];
	m_center.x /= static_cast<float>(m_initialVertices.size());
	m_center.y /= static_cast<float>(m_initialVertices.size());
	return m_center;
}

sf::Vector2f const & PolygonShape::getNearest(sf::Vector2f const & vertex)
{
	sf::Vector2f const * nearest;
	float mag = 100000000000.f;
	for (std::size_t i = 0u; i < getVertexCount(); i++)
	{
		float m = octo::magnitude(vertex - getVertex(i));
		if (m < mag)
		{
			mag = m;
			nearest = &getVertex(i);
		}
	}
	return *nearest;
}

void PolygonShape::setVertexCount(std::size_t vertexCount)
{
	m_vertices.resize(vertexCount);
	m_initialVertices.resize(vertexCount);
	m_edges.resize(vertexCount);
	m_normals.resize(vertexCount);
}

void PolygonShape::setVertex(std::size_t index, sf::Vector2f const & vector)
{
	//m_recomputeEdges = true;
	m_vertices[index] = vector;
	m_initialVertices[index] = vector;
}

void PolygonShape::computeRotation(void)
{
	float cos = std::cos(getRotation());
	float sin = std::sin(getRotation());
	float x;
	float y;
	sf::Vector2f const & origin = getOrigin();
	for (std::size_t i = 0; i < m_vertices.size(); i++)
	{
		x = m_vertices[i].x - origin.x;
		y = m_vertices[i].y - origin.y;
		m_initialVertices[i].x = origin.x + (x * cos - y * sin) + getPosition().x;
		m_initialVertices[i].y = origin.y + (x * sin + y * cos) + getPosition().y;
	}
	//m_recomputeEdges = true;
	//m_recomputeGlobalBounds = true;
}

void PolygonShape::debugDraw(sf::RenderTarget & render)
{
	sf::ConvexShape shape;
	shape.setPointCount(m_vertices.size());
	for (std::size_t i = 0; i < m_vertices.size(); i++)
		shape.setPoint(i, getVertex(i));
	render.draw(shape);
	AShape::debugDraw(render);
}
