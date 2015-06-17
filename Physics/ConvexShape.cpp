#include "ConvexShape.hpp"

ConvexShape::ConvexShape(void) :
	PolygonShape()
{ }

void ConvexShape::setVertex(std::size_t index, sf::Vector2f const & vector)
{
	PolygonShape::setVertex(index, vector);
}

void ConvexShape::setVertexCount(std::size_t vertexCount)
{
	PolygonShape::setVertexCount(vertexCount);
}
