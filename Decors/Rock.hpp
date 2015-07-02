#ifndef ROCK_HPP
# define ROCK_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Color.hpp>

class Rock : public ADecor
{
struct OctogonValue
{
	sf::Vector2f			size;
	sf::Vector2f			origin;
	float					sizeLeft;
	float					sizeRight;
	float					sizeRec;
};

public:
	Rock(void);
	virtual ~Rock(void) = default;

	virtual void	setup(ABiome& biome);
	virtual void	update(sf::Time frameTime,
						   octo::VertexBuilder& builder,
						   ABiome& biome);

private:
	sf::Vector2f				m_size;
	sf::Color					m_color;
	std::size_t					m_partCount;
	std::vector<OctogonValue>	m_values;
	DecorAnimator				m_animator;
	float						m_animation;

	sf::Vector2f				m_left;
	sf::Vector2f				m_right;

	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & origin,
						sf::Color const & color,
						float const & sizeLeft,
						float const & sizeRight,
						float const & sizeRec,
						sf::Vector2f const & position,
						octo::VertexBuilder& builder);

	void createRock(std::vector<OctogonValue> const & values,
					sf::Vector2f const & originRock,
					sf::Color const & color,
					octo::VertexBuilder& builder);
};

#endif
