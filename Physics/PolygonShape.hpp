#ifndef POLYGONSHAPE_HPP
# define POLYGONSHAPE_HPP

# include "AShape.hpp"
# include <vector>

class PolygonShape : public AShape
{
public:
	virtual ~PolygonShape(void) = default;

	/*! Get the vertex count */
	inline std::size_t getVertexCount(void) const { return m_vertices.size(); }

	/*! Get the edge at a given index
	 * If index > vertexCount, behavior is undefined
	 *
	 * \param index The index
	 */
	inline sf::Vector2f const & getEdge(std::size_t index) const { return m_edges[index]; }

	/*! Get the normal at a given index
	 * If index > vertexCount, behavior is undefined
	 *
	 * \param index The index
	 */
	inline sf::Vector2f const & getNormal(std::size_t index) const { return m_normals[index]; }

	/*! Get the vertex at a given index
	 * If index > vertexCount, behavior is undefined
	 *
	 * \param index The index
	 */
	inline sf::Vector2f const & getVertex(std::size_t index) const { return m_vertices[index]; }

	/*! Get the global bounds
	 * The AABB is recomputed at each rotation
	 *
	 * \see rotate
	 */
	inline virtual sf::Rect<float> const & getGlobalBounds(void) const { return m_globalBounds; }

	/*! Get the center of the object
	 * The center is recomputed at each rotation
	 *
	 * \see rotate
	 */
	inline virtual sf::Vector2f const & getBaryCenter(void) const { return m_baryCenter; }

	/*! Get the support vertex
	 * The support vertex is the farthest vzertex of the polygon along the direction
	 *
	 * \param direction The axis we used to search the farthest vertex
	 */
	sf::Vector2f const & getSupportVertex(sf::Vector2f const & direction);

	/*! Get the vertex count needed to compute the sat */
	inline virtual std::size_t getEfficientVertexCount(void) const { return m_vertices.size(); }

	/*! Use to draw debug information */
	virtual void debugDraw(sf::RenderTarget & render);

protected:
	PolygonShape(void);

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

	/*! Compute the shape once per frame */
	virtual void computeShape(void);

private:
	static constexpr std::size_t MaxVertices = 100u;
	std::vector<sf::Vector2f>	m_vertices;
	std::vector<sf::Vector2f>	m_initialVertices;
	std::vector<sf::Vector2f>	m_edges;
	std::vector<sf::Vector2f>	m_normals;
	sf::Vector2f				m_baryCenter;
	sf::Rect<float>				m_globalBounds;

};

#endif
