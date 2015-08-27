#ifndef TREE_HPP
# define TREE_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
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

	struct OctogonValue
	{
		sf::Vector2f			size;
		sf::Vector2f			sizeCorner;
		sf::Vector2f			origin;
		float					cos;
		float					sin;
	};

	std::size_t					m_depth;
	sf::Vector2f				m_size;
	sf::Color					m_color;
	std::vector<float>			m_refAngle;
	std::size_t					m_count;
	std::size_t					m_angleMaxCount;
	float						m_mapSizeY;

	DecorAnimator				m_animator;
	float						m_animation;
	bool						m_growSide;

	bool						m_isLeaf;
	std::vector<OctogonValue>	m_octogonLeaf;
	std::vector<sf::Vector2f>	m_leafSize;
	float						m_leafCornerCoef;
	sf::Color					m_leafColor;
	std::size_t					m_countLeaf;
	bool						m_setLeaf;
	std::size_t					m_leafMaxCount;

	bool						m_sound;

	void computeQuad(sf::Vector2f const & size,
					sf::Vector2f const & center,
					float const cosAngle,
					float const sinAngle,
					QuadValue & quad);

	void setOctogonValue(OctogonValue & value,
						sf::Vector2f const & size,
						sf::Vector2f const & origin,
						float cos,
						float sin);

	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & sizeCorner,
						sf::Vector2f const & origin,
						float cos,
						float sin,
						sf::Color const & color,
						octo::VertexBuilder& builder);

	void createBiColorQuad(QuadValue const & quad,
							sf::Color const & color,
							float const deltaColor,
							octo::VertexBuilder & builder);

	void createTrunk(sf::Vector2f const & size,
						sf::Vector2f const & center,
						sf::Color const & color,
						octo::VertexBuilder & builder);

	void createLeaf(std::vector<OctogonValue> const & leaf,
					sf::Color const & color,
					octo::VertexBuilder & builder);

	void pythagorasTree(sf::Vector2f const & center,
						sf::Vector2f const & size,
						octo::VertexBuilder & builder,
						float const angle = 0.0f,
						float const cosAngle = std::cos(0.0f),
						float const sinAngle = std::sin(0.0f),
						std::size_t const currentDepth = 0u);

	void newTree(ABiome& biome);
	void playSound(ABiome & biome, sf::Vector2f const & position);
};

#endif
