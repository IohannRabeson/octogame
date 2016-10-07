#include "Tree.hpp"
#include "ABiome.hpp"
#include <Math.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>
#include <Math.hpp>
#include "ResourceDefinitions.hpp"

Tree::Tree(bool onInstance) :
	m_depth(0u),
	m_count(0u),
	m_angleMaxCount(0u),
	m_mapSizeY(0.f),
	m_animator(4.f, 4.f, 3.f, 0.01f),
	m_animation(1.f),
	m_growSide(true),
	m_isLeaf(true),
	m_leafCornerCoef(2.f),
	m_countLeaf(0u),
	m_setLeaf(true),
	m_leafMaxCount(0u),
	m_sound(true),
	m_onInstance(onInstance)
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

	octo::rotateVector(quad.leftDown, cosAngle, sinAngle);
	octo::rotateVector(quad.leftUp, cosAngle, sinAngle);
	octo::rotateVector(quad.rightDown, cosAngle, sinAngle);
	octo::rotateVector(quad.rightUp, cosAngle, sinAngle);

	quad.center = center;
}

void Tree::setOctogonValue(OctogonValue & value, sf::Vector2f const & size, sf::Vector2f const & origin, float cos, float sin)
{
	value.size = size;
	value.sizeCorner = size / m_leafCornerCoef;
	value.origin = origin;
	value.cos = cos;
	value.sin = sin;
}

void Tree::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, float cos, float sin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x + sizeCorner.x, -size.y);
	sf::Vector2f upRight(size.x - sizeCorner.x, -size.y);
	sf::Vector2f upMidLeft(-size.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidRight(size.x, -size.y + sizeCorner.y);
	sf::Vector2f downLeft(-size.x + sizeCorner.x, size.y);
	sf::Vector2f downRight(size.x - sizeCorner.x, size.y);
	sf::Vector2f downMidLeft(-size.x, size.y - sizeCorner.y);
	sf::Vector2f downMidRight(size.x, size.y - sizeCorner.y);

	octo::rotateVector(upLeft, cos, sin);
	octo::rotateVector(upRight, cos, sin);
	octo::rotateVector(upMidLeft, cos, sin);
	octo::rotateVector(upMidRight, cos, sin);
	octo::rotateVector(downLeft, cos, sin);
	octo::rotateVector(downRight, cos, sin);
	octo::rotateVector(downMidLeft, cos, sin);
	octo::rotateVector(downMidRight, cos, sin);

	upLeft += origin;
	upRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	downMidLeft += origin;
	downMidRight += origin;

	sf::Color deltaColor = color + sf::Color(7, 7, 7, 0);
	builder.createTriangle(origin, upLeft, upRight, deltaColor);
	builder.createTriangle(origin, upRight, upMidRight, deltaColor);
	builder.createTriangle(origin, upMidRight, downMidRight, deltaColor);
	builder.createTriangle(origin, downMidRight, downRight, deltaColor);
	builder.createTriangle(origin, downRight, downLeft, color);
	builder.createTriangle(origin, downLeft, downMidLeft, color);
	builder.createTriangle(origin, downMidLeft, upMidLeft, color);
	builder.createTriangle(origin, upMidLeft, upLeft, color);
}

void Tree::createBiColorQuad(QuadValue const & quad, sf::Color const & color, float const deltaColor, octo::VertexBuilder & builder)
{
	sf::Vector2f tmpLeftDown = quad.leftDown + quad.center;
	sf::Vector2f tmpRightUp = quad.rightUp + quad.center;

	builder.createTriangle(tmpRightUp, quad.rightDown + quad.center, tmpLeftDown, color);
	sf::Color tmpColor(deltaColor + color.r, deltaColor + color.g, deltaColor + color.b, color.a);
	builder.createTriangle(tmpLeftDown, quad.leftUp + quad.center, tmpRightUp, tmpColor);
}

void Tree::createTrunk(sf::Vector2f const & size, sf::Vector2f const & center, sf::Color const & color, octo::VertexBuilder & builder)
{
	sf::Vector2f upLeft(center.x - size.x / 2.f, center.y - size.y / 2.f);
	sf::Vector2f upRight(center.x + size.x / 2.f, center.y - size.y / 2.f);
	sf::Vector2f downLeft(upLeft.x, upLeft.y + size.x * 10.f);
	sf::Vector2f downRight(upRight.x, upRight.y + size.x * 4.f);
	sf::Vector2f downMid(center.x, downRight.y + size.x);

	builder.createQuad(upLeft, upRight, downRight, downLeft, color);
	builder.createTriangle(downLeft, downRight, downMid, color);
}

void Tree::createLeaf(std::vector<OctogonValue> const & leafs, sf::Color const & color, octo::VertexBuilder & builder)
{
	for (auto leaf : leafs)
		createOctogon(leaf.size, leaf.sizeCorner, leaf.origin, leaf.cos, leaf.sin, color ,builder);
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
	//TODO: find a smart way to compute deltaColor
	float colorChange = currentDepth * 5.f + 1;
	sf::Color color = sf::Color(m_color.r + colorChange, m_color.g + colorChange, m_color.b + colorChange, m_color.a);

	// Compute root rectangle
	QuadValue root;
	computeQuad(size, center, cosAngle, sinAngle, root);

	// Compute left branch
	float rectangleAngleLeft = angle - refAngle;
	float radianLeft = rectangleAngleLeft * octo::Deg2Rad;
	float cosLeft = std::cos(radianLeft);
	float sinLeft = std::sin(radianLeft);

	float leftSizeX = std::cos(refAngle * octo::Deg2Rad) * size.x;
	float leftSizeY = leftSizeX * size.y / size.x;
	leftSizeY *= m_animation;
	sf::Vector2f leftSize(leftSizeX, leftSizeY);
	sf::Vector2f leftCenter(leftSize.x / 2.f, -leftSize.y / 2.f);
	octo::rotateVector(leftCenter, cosLeft, sinLeft);
	leftCenter += center + root.leftUp;

	// Compute right branch
	float rectangleAngleRight = angle + 90.f - refAngle;
	float radianRight = rectangleAngleRight * octo::Deg2Rad;
	float cosRight = std::cos(radianRight);
	float sinRight = std::sin(radianRight);

	float rightSizeX = std::cos((90.0f - refAngle) * octo::Deg2Rad) * size.x;
	float rightSizeY = (rightSizeX * size.y / size.x);
	rightSizeY *= m_animation;
	sf::Vector2f rightSize(rightSizeX, rightSizeY);
	sf::Vector2f rightCenter(-rightSize.x / 2.f, -rightSize.y / 2.f);
	octo::rotateVector(rightCenter, cosRight, sinRight);
	rightCenter += center + root.rightUp;

	// Create leaf
	if (m_isLeaf && currentDepth == m_depth - 2)
	{
		setOctogonValue(m_octogonLeaf[m_countLeaf], m_leafSize[m_countLeaf] / 2.f * m_animation, rightCenter, cosRight, sinRight);
		m_countLeaf++;
		setOctogonValue(m_octogonLeaf[m_countLeaf], m_leafSize[m_countLeaf] / 2.f * m_animation, leftCenter, cosLeft, sinLeft);
		m_countLeaf++;
	}

	// Left & right recursion
	pythagorasTree(leftCenter, leftSize, builder, rectangleAngleLeft, cosLeft, sinLeft, currentDepth + 1);
	pythagorasTree(rightCenter, rightSize, builder, rectangleAngleRight, cosRight, sinRight, currentDepth + 1);

	if (m_setLeaf == true && m_isLeaf == true)
	{
		m_setLeaf = false;
		createLeaf(m_octogonLeaf, m_leafColor, builder);
	}

	//TODO: find a smart way to compute deltaColor
	createBiColorQuad(root, color, 5, builder);

	// Fill empty space with triangle
	color += sf::Color(5, 5, 5, 0);
	sf::Vector2f upTriangle(-rightSize.x, 0.f);
	octo::rotateVector(upTriangle, cosRight, sinRight);
	builder.createTriangle(root.rightUp + center, root.leftUp + center, upTriangle + center + root.rightUp, color);

	if (currentDepth == 0u && !m_onInstance)
		createTrunk(size, center, m_color, builder);
}

void Tree::setup(ABiome& biome)
{
	m_depth = biome.getTreeDepth();

	m_angleMaxCount = std::pow(2, m_depth) + 1;
	m_refAngle.resize(m_angleMaxCount);

	m_leafMaxCount = std::pow(2, m_depth) + 1;
	m_octogonLeaf.resize(m_leafMaxCount);
	m_leafSize.resize(m_leafMaxCount);
	m_mapSizeY = biome.getMapSizeFloat().y;
	m_animator.setBeatMouvement(biome.getTreeBeatMouvement());

	newTree(biome);
}

void Tree::newTree(ABiome& biome)
{
	m_size = biome.getTreeSize();
	m_color = biome.getTreeColor();

	for (std::size_t i = 0u; i < m_angleMaxCount; i++)
		m_refAngle[i] = biome.getTreeAngle();
	m_animator.setup(biome.getTreeLifeTime());
	m_growSide = biome.randomBool(0.5);

	m_isLeaf = biome.canCreateLeaf();

	for (std::size_t i = 0; i < m_leafMaxCount; i++)
		m_leafSize[i] = biome.getLeafSize();
	m_leafColor = biome.getLeafColor();
	m_animator.setup(biome.getTreeLifeTime());
	m_sound = true;
}

void Tree::playSound(ABiome & biome, sf::Vector2f const & position)
{
	if (m_sound && m_animation)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();

		float volume = 0.7f;
		if (m_size.y < 100.f)
			volume *= m_size.y / 100.f;
		audio.playSound(resources.getSound(TREE_OGG), volume, biome.randomFloat(0.8f, 1.f), sf::Vector3f(position.x, position.y, 0.f), 500.f, 40.f);
		m_sound = false;
	}
}

void Tree::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	if (biome.getTreeIsMoving() == false)
		m_animator.sleep();

	m_animator.update(frameTime);
	if (m_animator.getState() == DecorAnimator::State::Dead)
		newTree(biome);
	m_animation = m_animator.getAnimation();
	sf::Vector2f const & position = getPosition();

	playSound(biome, position);

	float positionY = position.y + (m_size.y - m_size.y * m_animation) / 2 - m_size.y / 2.f;
	pythagorasTree(sf::Vector2f(position.x, positionY), sf::Vector2f(m_size.x, m_size.y * m_animation), builder);
}

