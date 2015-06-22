#ifndef RECTANGLESHAPE_HPP
# define RECTANGLESHAPE_HPP

# include "PolygonShape.hpp"

class RectangleShape : public PolygonShape
{
public:
	RectangleShape(void);
	virtual ~RectangleShape(void) = default;

	/*! Set the size of the rectangle
	 *
	 * \param size The new size
	 */
	void setSize(sf::Vector2f const & size);

	/*! Get the size of the rectangle */
	inline sf::Vector2f const & getSize(void) const { return m_size; }

	/*! Get the vertex count needed to compute the sat */
	inline virtual std::size_t getEfficientVertexCount(void) const { return 2u; }

private:
	sf::Vector2f	m_size;

};

#endif
