#ifndef POLYGON_HPP
# define POLYGON_HPP

# include <SFML/Graphics.hpp>
# include <vector>

class Polygon
{
public:
	Polygon(std::size_t p_verticesCount);
	virtual ~Polygon(void);

	void setVertex(std::size_t index, sf::Vector2f const & vector);
	void setVertex(std::size_t index, sf::Vertex const & vertex);
	sf::Vector2f const & getCenter(void);
	sf::Vector2f const & getEdge(std::size_t index);
	sf::Vector2f const & getNormal(std::size_t index);
	std::size_t getVerticeCount(void) const;

	virtual sf::Vector2f const & getVertex(std::size_t index) const;

	// TODO: remove
	std::vector<sf::Vertex>		m_vertices;

protected:
	std::vector<sf::Vector2f>	m_edges;
	std::vector<sf::Vector2f>	m_normals;
	bool				m_recompute;
	bool				m_recomputeCenter;
	sf::Vector2f			m_center;
	std::size_t			m_verticesCount;

	// Compute edges and normals
	virtual void computeCenter(void);
	virtual void computeEdges(void);

private:
	Polygon(void) = delete;

};

#endif
