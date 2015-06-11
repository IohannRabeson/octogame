#ifndef CONVEXSHAPE_HPP
# define CONVEXSHAPE_HPP

# include "PolygonShape.hpp"

class ConvexShape : public PolygonShape
{
public:
	ConvexShape(void);
	virtual ~ConvexShape(void) = default;

	/*! Set the vertex at a given index
	 * If index > vertexCount, behavior is undefined
	 * The vertex position is relative to the position of the shape
	 *
	 * \param index The given index
	 * \param vector The new vector
	 */
	virtual void setVertex(std::size_t index, sf::Vector2f const & vector);

	/*! Set the vertex count
	 * This method will resize the vectors that contain the vertices
	 *
	 * \param vertexCount The vertex count
	 */
	virtual void setVertexCount(std::size_t vertexCount);

private:

};

#endif
