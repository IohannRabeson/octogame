#ifndef TREE_HPP
# define TREE_HPP

# include "ADecor.hpp"
# include <SFML/Graphics/Color.hpp>

class Tree : public ADecor
{
public:
	Tree(void);
	virtual ~Tree(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						DecorBuilder& builder,
						ABiome& biome);

private:
	struct QuadValue
	{
		sf::Vector2f		leftDown;
		sf::Vector2f		leftUp;
		sf::Vector2f		rightDown;
		sf::Vector2f		rightUp;
		sf::Vector2f		center;
	};

	unsigned int			m_depth;
	sf::Vector2f			m_size;
	sf::Color				m_color;
	std::vector<float>		m_refAngle;

	sf::Time				m_lifeTime;
	float					m_animation;

	bool					m_growSide;
	std::vector<QuadValue>	m_leaf;
	sf::Color				m_leafColor;
	//int						m_maxLeaf;
	//int						m_countLeaf;
	//bool					m_isLeaf;

	void computeQuad(sf::Vector2f const & size,
					sf::Vector2f const & center,
					float cosAngle,
					float sinAngle,
					QuadValue & quad);

	void createBiColorQuad(QuadValue const & quad,
							sf::Color & color,
							float const deltaColor,
							DecorBuilder & builder);

	void createLeaf(std::vector<QuadValue> const & quads,
					sf::Color & color,
					float const deltaColor,
					DecorBuilder & builder);

	void pythagorasTree(sf::Vector2f const & center,
						sf::Vector2f const & size,
						DecorBuilder & builder,
						unsigned int angleCount = 0u,
						float const angle = 0.0f,
						float const cosAngle = std::cos(0.0f),
						float const sinAngle = std::sin(0.0f),
						unsigned int const currentDepth = 1);

	static void rotateVec(sf::Vector2f & vector,
							float const cosAngle,
							float const sinAngle);
	float randomFloat(float min, float max);
};

#endif
