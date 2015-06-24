#ifndef TREE_HPP
# define TREE_HPP

# include "ADecor.hpp"
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Color.hpp>

class Tree : public ADecor
{
public:
	Tree(void);
	virtual ~Tree(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	struct QuadValue
	{
		sf::Vector2f			leftDown;
		sf::Vector2f			leftUp;
		sf::Vector2f			rightDown;
		sf::Vector2f			rightUp;
		sf::Vector2f			center;
	};

	std::size_t					m_depth;
	sf::Vector2f				m_size;
	sf::Color					m_color;
	std::vector<float>			m_refAngle;
	std::size_t					m_count;

	sf::Time					m_lifeTime;
	float						m_animation;
	bool						m_growSide;

	bool						m_isLeaf;
	std::vector<QuadValue>		m_leaf;
	std::vector<sf::Vector2f>	m_leafSize;
	sf::Color					m_leafColor;
	std::size_t					m_countLeaf;
	bool						m_setLeaf;

	void computeQuad(sf::Vector2f const & size,
					sf::Vector2f const & center,
					float const cosAngle,
					float const sinAngle,
					QuadValue & quad);

	void createBiColorQuad(QuadValue const & quad,
							sf::Color const & color,
							float const deltaColor,
							octo::VertexBuilder & builder);

	void createLeaf(std::vector<QuadValue> const & quads,
					sf::Color const & color,
					float const deltaColor,
					octo::VertexBuilder & builder);

	void pythagorasTree(sf::Vector2f const & center,
						sf::Vector2f const & size,
						octo::VertexBuilder & builder,
						float const angle = 0.0f,
						float const cosAngle = std::cos(0.0f),
						float const sinAngle = std::sin(0.0f),
						std::size_t const currentDepth = 0u);

private:
	static std::default_random_engine	m_engine;
	static std::bernoulli_distribution	m_distribution;

	static void rotateVec(sf::Vector2f & vector,
							float const cosAngle,
							float const sinAngle);

	static bool getGrowSide(void);
};

#endif
