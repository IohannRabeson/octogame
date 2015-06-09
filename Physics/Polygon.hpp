#ifndef POLYGON_HPP
# define POLYGON_HPP

# include "AShape.hpp"
# include <vector>

class Polygon : public AShape
{
public:
	Polygon(void);
	virtual ~Polygon(void) = default;

	/*! Set the vertex at a given index
	 * If index > vertexCount, behavior is undefined
	 * The vertex position is relative to the position of the shape
	 *
	 * \param index The given index
	 * \param vector The new vector
	 */
	inline void setVertex(std::size_t index, sf::Vector2f const & vector) { m_recomputeEdges = true; m_vertices[index] = vector; }

	inline sf::Vector2f getVertex(std::size_t index) const { return m_vertices[index] + getPosition(); }
	inline std::size_t getVertexCount(void) const { return m_vertices.size(); }

	/*! Set the vertex count
	 * This method will resize the vectors that contain the vertices
	 *
	 * \param vertexCount The vertex count
	 */
	virtual void setVertexCount(std::size_t vertexCount);

	virtual sf::Vector2f const & getEdge(std::size_t index);
	virtual sf::Vector2f const & getNormal(std::size_t index);
	virtual sf::Vector2f const & getCenter(void);
	virtual sf::Rect<float> const & getGlobalBounds(void);
	virtual void rotate(float angle);
	virtual void debugDraw(sf::RenderTarget & render);

private:
	std::vector<sf::Vector2f>	m_vertices;
	std::vector<sf::Vector2f>	m_edges;
	std::vector<sf::Vector2f>	m_normals;
	bool				m_recomputeEdges;
	bool				m_recomputeGlobalBounds;

	void computeEdges(void);

};

#endif
