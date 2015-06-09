#include "OBB.hpp"

OBB::OBB(void) :
	Polygon()
{
	setVertexCount(4u);
}

void OBB::setVertexCount(std::size_t vertexCount)
{
	Polygon::setVertexCount(vertexCount);
}

void OBB::setSize(sf::Vector2f const & size)
{
	m_size = size;
}
