#ifndef OBB_HPP
# define OBB_HPP

# include "Polygon.hpp"

class OBB : public Polygon
{
public:
	OBB(void);
	virtual ~OBB(void) = default;

	void setSize(sf::Vector2f const & size);
	sf::Vector2f const & getSize(void) const;
	//virtual sf::Vector2f const & getEdge(std::size_t index);
	//virtual sf::Vector2f const & getNormal(std::size_t index);

private:
	virtual void setVertexCount(std::size_t vertexCount);
	sf::Vector2f m_size;


};

#endif
