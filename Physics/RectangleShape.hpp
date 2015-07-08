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
	inline void setSize(sf::Vector2f const & size) { setSize(size.x, size.y); }

	/*! Set the size of the rectangle
	 *
	 * \param size The new size
	 */
	void setSize(float width, float height);

	/*! Get the size of the rectangle */
	inline sf::Vector2f const & getSize(void) const { return m_size; }

	/*! Get the vertex count needed to compute the sat */
	inline virtual std::size_t getEfficientVertexCount(void) const { return 3u; }

private:
	sf::Vector2f	m_size;

};

#endif
