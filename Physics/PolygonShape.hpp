#ifndef POLYGONSHAPE_HPP
# define POLYGONSHAPE_HPP

# include "AShape.hpp"
# include <vector>

class PolygonShape : public AShape
{
public:
	PolygonShape(void);
	virtual ~PolygonShape(void) = default;

	/*! Get the vertex count */
	inline std::size_t getVertexCount(void) const { return m_vertices.size(); }

	/*! Get the edge at a given index
	 * If index > vertexCount, behavior is undefined
	 *
	 * \param index The index
	 */
	sf::Vector2f const & getEdge(std::size_t index);

	/*! Get the normal at a given index
	 * If index > vertexCount, behavior is undefined
	 *
	 * \param index The index
	 */
	sf::Vector2f const & getNormal(std::size_t index);

	/*! Get the vertex at a given index
	 * If index > vertexCount, behavior is undefined
	 *
	 * \param index The index
	 */
	sf::Vector2f const & getVertex(std::size_t index);

	/*! Use to draw debug information
	 */
	virtual void debugDraw(sf::RenderTarget & render);

	/*! Get the global bounds
	 * The AABB is recomputed at each rotation
	 *
	 * \see rotate
	 */
	virtual sf::Rect<float> const & getGlobalBounds(void);

	/*! Get the center of the object
	 * The center is recomputed at each rotation
	 *
	 * \see rotate
	 */
	virtual sf::Vector2f const & getCenter(void);

protected:
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
	std::vector<sf::Vector2f>	m_vertices;
	std::vector<sf::Vector2f>	m_initialVertices;
	std::vector<sf::Vector2f>	m_edges;
	std::vector<sf::Vector2f>	m_normals;
	sf::Vector2f			m_center;
	sf::Rect<float>			m_globalBounds;

	void computeRotation(void);
	void computeEdges(void);

};

#endif
