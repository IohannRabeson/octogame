#include "Polygon.hpp"
#include <limits>

Polygon::Polygon(void) :
	AShape(),
	m_recomputeEdges(true),
	m_recomputeGlobalBounds(true)
{
	// TODO: ask iohann what is the best
	m_vertices.reserve(1000u);
	m_edges.reserve(1000u);
	m_normals.reserve(1000u);
}

void Polygon::setVertexCount(std::size_t vertexCount)
{
	m_vertices.resize(vertexCount);
	m_edges.resize(vertexCount);
	m_normals.resize(vertexCount);
}

void Polygon::computeEdges(void)
{
	m_recomputeEdges = false;
	for (std::size_t i = 0; i < m_vertices.size(); i++)
	{
		m_edges[i].x = m_vertices[i].x - m_vertices[(i + 1) % m_vertices.size()].x;
		m_edges[i].y = m_vertices[i].y - m_vertices[(i + 1) % m_vertices.size()].y;
		m_normals[i].x = -m_edges[i].y;
		m_normals[i].y = m_edges[i].x;
	}
}

sf::Rect<float> const & Polygon::getGlobalBounds(void)
{
	//TODO: ask io
	//if (m_recomputeGlobalBounds)
	{
		m_recomputeGlobalBounds = false;
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = -std::numeric_limits<float>::max();
		float maxY = -std::numeric_limits<float>::max();
		sf::Rect<float> globalBounds;
		for (std::size_t i = 0; i < m_vertices.size(); i++)
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
		globalBounds.left = minX + getPosition().x;
		globalBounds.top = minY + getPosition().y;
		globalBounds.width = maxX - minX;
		globalBounds.height = maxY - minY;
		setGlobalBounds(globalBounds);
	}
	return AShape::getGlobalBounds();
}

sf::Vector2f const & Polygon::getEdge(std::size_t index)
{
	if (m_recomputeEdges)
		computeEdges();
	return m_edges[index];
}

sf::Vector2f const & Polygon::getNormal(std::size_t index)
{
	if (m_recomputeEdges)
		computeEdges();
	return m_normals[index];
}

sf::Vector2f const & Polygon::getCenter(void)
{
	sf::Vector2f center;
	//recompute
	center.x = 0.f;
	center.y = 0.f;
	for (std::size_t i = 0; i < m_vertices.size(); i++)
		center += m_vertices[i];
	center.x /= static_cast<float>(m_vertices.size());
	center.y /= static_cast<float>(m_vertices.size());
	center += getPosition();
	setCenter(center);
	return AShape::getCenter();
}

void Polygon::rotate(float angle)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float x;
	float y;
	sf::Vector2f const & origin = getOrigin();
	for (std::size_t i = 0; i < m_vertices.size(); i++)
	{
		x = m_vertices[i].x - origin.x;
		y = m_vertices[i].y - origin.y;
		m_vertices[i].x = origin.x + (x * cos - y * sin);
		m_vertices[i].y = origin.y + (x * sin + y * cos);
	}
	m_recomputeEdges = true;
	m_recomputeGlobalBounds = true;
}

void Polygon::debugDraw(sf::RenderTarget & render)
{
	sf::ConvexShape shape;
	shape.setPointCount(m_vertices.size());
	for (std::size_t i = 0; i < m_vertices.size(); i++)
		shape.setPoint(i, getVertex(i));
	render.draw(shape);
	AShape::debugDraw(render);
}
