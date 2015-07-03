#ifndef TILESHAPE_HPP
# define TILESHAPE_HPP

# include "AShape.hpp"
# include <vector>

/*!
 * \ingroup Physic
 * \class AShape
 * Class representing a tile shape
 * It can't be moved or rotated
 */
class TileShape : public AShape
{
public:
	TileShape(void);
	virtual ~TileShape(void) = default;

	/*! Get the vertex count */
	inline std::size_t getVertexCount(void) const { return 4u; }

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
	sf::Vector2f const & getVertex(std::size_t index) const;

	/*! Set the up left vertex
	 * Other vertices are accessible at index between 0 && 4
	 * m_vertices[0] = up left
	 * m_vertices[1] = up right
	 * m_vertices[2] = down right
	 * m_vertices[3] = down left
	 *
	 * \param vertex The new vertex pointer
	 */
	inline void setVertex(sf::Vertex * vertex) { m_vertex = vertex; }

	inline void setHeight(float height) { m_height = height; }

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
	inline virtual std::size_t getEfficientVertexCount(void) const { return 4u; }

	/*! Use to draw debug information */
	virtual void debugDraw(sf::RenderTarget & render);

protected:
	/*! Compute the shape once per frame */
	virtual void computeShape(void);

private:
	sf::Vertex *				m_vertex; /// Pointer to the up left vertex of the tile
	sf::Vector2f				m_vertices[2];
	std::vector<sf::Vector2f>	m_edges;
	std::vector<sf::Vector2f>	m_normals;
	sf::Vector2f				m_baryCenter;
	sf::FloatRect				m_globalBounds;
	float						m_height;

};

#endif
