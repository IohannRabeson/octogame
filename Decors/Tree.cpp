#include "Tree.hpp"
#include "ABiome.hpp"
#include <Math.hpp>

std::bernoulli_distribution	Tree::m_distribution(0.5);
std::default_random_engine	Tree::m_engine;

Tree::Tree(void) :
	m_depth(0u),
	m_count(0u),
	m_lifeTime(sf::seconds(0.f)),
	m_animation(1.f),
	m_growSide(true),
	m_isLeaf(true),
	m_countLeaf(0u),
	m_setLeaf(true)
{
}

void Tree::computeQuad(sf::Vector2f const & size, sf::Vector2f const & center, float cosAngle, float sinAngle, QuadValue & quad)
{
	float x = size.x / 2.0f;
	float y = size.y / 2.0f;

	quad.leftDown = sf::Vector2f(-x, y);
	quad.leftUp = sf::Vector2f(-x, -y);
	quad.rightDown = sf::Vector2f(x, y);
	quad.rightUp = sf::Vector2f(x, -y);

	rotateVec(quad.leftDown, cosAngle, sinAngle);
	rotateVec(quad.leftUp, cosAngle, sinAngle);
	rotateVec(quad.rightDown, cosAngle, sinAngle);
	rotateVec(quad.rightUp, cosAngle, sinAngle);

	quad.center = center;
}

void Tree::createBiColorQuad(QuadValue const & quad, sf::Color const & color, float const deltaColor, octo::VertexBuilder & builder)
{
	builder.createTriangle(quad.rightUp + quad.center, quad.rightDown + quad.center, quad.leftDown + quad.center, color);
	sf::Color tmpColor(deltaColor + color.r, deltaColor + color.g, deltaColor + color.b);
	builder.createTriangle(quad.leftDown + quad.center, quad.leftUp + quad.center, quad.rightUp + quad.center, tmpColor);
}

void Tree::createLeaf(std::vector<QuadValue> const & quads, sf::Color const & color, float const deltaColor, octo::VertexBuilder & builder)
{
	for (auto quad : quads)
		createBiColorQuad(quad, color, deltaColor, builder);
}

void Tree::pythagorasTree(sf::Vector2f const & center, sf::Vector2f const & size, octo::VertexBuilder & builder, float const angle, float const cosAngle, float const sinAngle, std::size_t currentDepth)
{
	// Stop recursion
	if (currentDepth >= m_depth)
		return;

	if (currentDepth == 0)
	{
		m_count = m_countLeaf = 0u;
		m_setLeaf = m_isLeaf;
	}

	// Get current angle
	float refAngle;
	if (m_growSide)
		refAngle = m_refAngle[m_count] * m_animation;
	else
		refAngle = 90.f - m_refAngle[m_count] * m_animation;
	m_count++;

	// Init color
	float colorChange = currentDepth * 15.f + 1;
	sf::Color color = sf::Color(m_color.r + colorChange, m_color.g + colorChange, m_color.b + colorChange);

	// Compute root rectangle
	QuadValue root;
	computeQuad(size, center, cosAngle, sinAngle, root);

	// Compute left branch
	float rectangleAngleLeft = angle - refAngle;
	float radianLeft = rectangleAngleLeft * octo::Deg2Rad;
	float cosLeft = cos(radianLeft);
	float sinLeft = sin(radianLeft);

	float leftSizeX = cos(refAngle * octo::Deg2Rad) * size.x;
	float leftSizeY = leftSizeX * size.y / size.x;
	leftSizeY *= m_animation;
	sf::Vector2f leftSize(leftSizeX, leftSizeY);
	sf::Vector2f leftCenter(leftSize.x / 2.f, -leftSize.y / 2.f);
	rotateVec(leftCenter, cosLeft, sinLeft);
	leftCenter += center + root.leftUp;

	// Compute right branch
	float rectangleAngleRight = angle + 90.f - refAngle;
	float radianRight = rectangleAngleRight * octo::Deg2Rad;
	float cosRight = cos(radianRight);
	float sinRight = sin(radianRight);

	float rightSizeX = cos((90.0f - refAngle) * octo::Deg2Rad) * size.x;
	float rightSizeY = (rightSizeX * size.y / size.x);
	rightSizeY *= m_animation;
	sf::Vector2f rightSize(rightSizeX, rightSizeY);
	sf::Vector2f rightCenter(-rightSize.x / 2.f, -rightSize.y / 2.f);
	rotateVec(rightCenter, cosRight, sinRight);
	rightCenter += center + root.rightUp;

	// Create leaf
	if (currentDepth == m_depth - 2 && m_isLeaf)
	{
		computeQuad(m_leafSize[m_countLeaf] * m_animation, rightCenter, cosRight, sinRight, m_leaf[m_countLeaf]);
		m_countLeaf++;
		computeQuad(m_leafSize[m_countLeaf] * m_animation, leftCenter, cosLeft, sinLeft, m_leaf[m_countLeaf]);
		m_countLeaf++;
	}

	// Left & right recursion
	pythagorasTree(leftCenter, leftSize, builder, rectangleAngleLeft, cosLeft, sinLeft, currentDepth + 1);
	pythagorasTree(rightCenter, rightSize, builder, rectangleAngleRight, cosRight, sinRight, currentDepth + 1);

	if (m_setLeaf == true && m_isLeaf == true)
	{
		m_setLeaf = false;
		createLeaf(m_leaf, m_leafColor, 5, builder);
	}

	//TODO: find a smart way to compute deltaColor
	createBiColorQuad(root, color, 5, builder);

	// Fill empty space with triangle
	color += sf::Color(5, 5, 5);
	sf::Vector2f upTriangle(-rightSize.x, 0.f);
	rotateVec(upTriangle, cosRight, sinRight);
	builder.createTriangle(root.rightUp + center, root.leftUp + center, upTriangle + center + root.rightUp, color);

}

void Tree::setup(ABiome& biome)
{
	m_depth = biome.getTreeDepth();
	m_size = biome.getTreeSize();
	m_color = biome.getTreeColor();

	std::size_t angleCount = std::pow(2, m_depth) + 1;
	m_refAngle.resize(angleCount);
	for (std::size_t i = 0u; i < angleCount; i++)
		m_refAngle[i] = biome.getTreeAngle();
	m_lifeTime = biome.getTreeLifeTime();
	m_growSide = Tree::getGrowSide();

	m_isLeaf = biome.canCreateLeaf();
	std::size_t leafCount = std::pow(2, m_depth) + 1;
	m_leaf.resize(leafCount);
	m_leafSize.resize(leafCount);
	for (std::size_t i = 0; i < leafCount; i++)
		m_leafSize[i] = biome.getLeafSize();
	m_leafColor = biome.getLeafColor();
}

void Tree::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	(void)frameTime;

	sf::Vector2f const & position = getPosition();
	//TODO: set position to be centered, to test when terrain will be here
	//Add behind root to stick the botom of screen
	//position.y -= m_size.y / 2.0f;
	//float delta = (m_size.y - m_size.y * m_animation) / 2;
	pythagorasTree(sf::Vector2f(position.x, position.y), sf::Vector2f(m_size.x, m_size.y * m_animation), builder);
}

void Tree::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

bool Tree::getGrowSide(void)
{
	return m_distribution(m_engine);
}

