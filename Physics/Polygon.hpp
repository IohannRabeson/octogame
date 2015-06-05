#ifndef POLYGON_HPP
# define POLYGON_HPP

# include <SFML/Graphics.hpp>
# include <vector>

class Polygon
{
public:
	enum CollideType
	{
		e_tile = 1,
		e_player = 2,
		e_pnj = 4
	};

	virtual ~Polygon(void);

	sf::Vector2f const & getCenter(void);
	sf::Vector2f const & getEdge(std::size_t index);
	sf::Vector2f const & getNormal(std::size_t index);

	inline int getCollideType(void) const { return m_collideType; }
	inline void setVertex(std::size_t index, sf::Vector2f const & vector) { m_vertices[index].position = vector; }
	inline void setVertex(std::size_t index, sf::Vertex const & vertex) { m_vertices[index] = vertex; }
	inline std::size_t getVerticeCount(void) const { return m_verticesCount; }

	virtual sf::Vector2f const & getVertex(std::size_t index) const;

	// TODO: remove
	std::vector<sf::Vertex>		m_vertices;

protected:
	Polygon(std::size_t p_verticesCount, CollideType p_collideType);

	std::vector<sf::Vector2f>	m_edges;
	std::vector<sf::Vector2f>	m_normals;
	bool				m_recompute;
	bool				m_recomputeCenter;
	sf::Vector2f			m_center;
	std::size_t			m_verticesCount;
	int				m_collideType;

	// Compute edges and normals
	virtual void computeCenter(void);
	virtual void computeEdges(void);

private:
	Polygon(void) = delete;

};

#endif
