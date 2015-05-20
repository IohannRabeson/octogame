#include "Tree.hpp"
#include "Map.hpp"

Tree::Tree(void) :
	Decor(),
	mn_countAngle(0u),
	mb_growSide(false),
	mb_isLeaf(false)
{
	// Number of square in pythagoras trees
	mn_maxRectangle = pow(2, DEPTH) - 1;
	// Squares divided in triangles + link triangles
	mn_maxTriangle = mn_maxRectangle * 3;
	mn_maxLeaf = pow(2, DEPTH - 1) - 1;

	allocateVertex(mn_maxTriangle * 3u + mn_maxLeaf * 6u);
	m_refAngle.resize(mn_maxRectangle);
}

Tree::~Tree(void)
{
	m_refAngle.clear();
}

void Tree::createRectangle(sf::Vector2f const & p_center, sf::Vector2f const & p_size, sf::Color & p_color, float p_valueColor, int * pn_count, float pf_cos, float pf_sin, sf::Vector2f * p_leftUp, sf::Vector2f * p_rightUp)
{
	float x = p_size.x / 2.0f;
	float y = p_size.y / 2.0f;

	sf::Vector2f leftDown(-x, y);
	sf::Vector2f leftUp(-x, -y);
	sf::Vector2f rightDown(x, y);
	sf::Vector2f rightUp(x, -y);

	rotateVec(&leftDown, pf_cos, pf_sin);
	rotateVec(&leftUp, pf_cos, pf_sin);
	rotateVec(&rightDown, pf_cos, pf_sin);
	rotateVec(&rightUp, pf_cos, pf_sin);

	// Create rectangle (2 triangles)
	createVertex(rightUp + p_center, p_color, pn_count);
	createVertex(rightDown + p_center, p_color, pn_count);
	createVertex(leftDown + p_center, p_color, pn_count);
	p_color += sf::Color(p_valueColor, p_valueColor, p_valueColor);
	createVertex(leftDown + p_center, p_color, pn_count);
	createVertex(leftUp + p_center, p_color, pn_count);
	createVertex(rightUp + p_center, p_color, pn_count);

	if (p_leftUp)
		*p_leftUp = leftUp;
	if (p_rightUp)
		*p_rightUp = rightUp;
}

void Tree::pythagorasTree(sf::Vector2f p_center, sf::Vector2f p_size, float pf_angle, float pf_cos, float pf_sin, const int pn_depth)
{
	m_leafColor = sf::Color(255 - m_color.r, 255 - m_color.g, 255 - m_color.b);
	if (pn_depth == 1)
	{
		mn_countLeaf = mn_countAngle = 0;
		mn_countTriangle = (mn_maxLeaf + 1) * 6u;
	}

	// Get seed angle
	float refAngle;
	if (mb_growSide)
		refAngle = m_refAngle[mn_countAngle] * mf_mouvement;
	else
		refAngle = 90.f - m_refAngle[mn_countAngle] * mf_mouvement;
	mn_countAngle++;

	// Init color
	float colorChange = pn_depth * 15;
	sf::Color color = sf::Color(m_color.r + colorChange, m_color.g + colorChange, m_color.b + colorChange);

	// Create extended root
	if (pn_depth == 1)
	{
		createVertex(sf::Vector2f(p_center.x - p_size.x / 2, p_center.y + p_size.y / 2), color, &mn_countTriangle);
		createVertex(sf::Vector2f(p_center.x + p_size.x / 2, p_center.y + p_size.y / 2), color, &mn_countTriangle);
		createVertex(sf::Vector2f(p_center.x + p_size.x / 2, SIZE_SCREEN_Y), color, &mn_countTriangle);
		createVertex(sf::Vector2f(p_center.x - p_size.x / 2, p_center.y + p_size.y / 2), color, &mn_countTriangle);
		createVertex(sf::Vector2f(p_center.x + p_size.x / 2, SIZE_SCREEN_Y), color, &mn_countTriangle);
		createVertex(sf::Vector2f(p_center.x - p_size.x / 2, SIZE_SCREEN_Y), color, &mn_countTriangle);
	}

	// Create root rectangle
	sf::Vector2f leftUpRoot;
	sf::Vector2f rightUpRoot;
	createRectangle(p_center, p_size, color, 5, &mn_countTriangle, pf_cos, pf_sin, &leftUpRoot, &rightUpRoot);

	// Stop recursion
	if (pn_depth >= DEPTH)
		return;

	// Compute left branch
	float rectangleAngleLeft = pf_angle - refAngle;
	float radianLeft = rectangleAngleLeft * PI / 180.f;
	float cosLeft = cos(radianLeft);
	float sinLeft = sin(radianLeft);

	float leftSizeX = cos(refAngle * RADIAN) * p_size.x;
	float leftSizeY = leftSizeX * p_size.y / p_size.x;
	leftSizeY *= mf_mouvement;
	sf::Vector2f leftSize(leftSizeX, leftSizeY);
	sf::Vector2f leftCenter(leftSize.x / 2.f, -leftSize.y / 2.f);
	rotateVec(&leftCenter, cosLeft, sinLeft);
	leftCenter += p_center + leftUpRoot;

	// Compute right branch
	float rectangleAngleRight = pf_angle + 90.f - refAngle;
	float radianRight = rectangleAngleRight * PI / 180.f;
	float cosRight = cos(radianRight);
	float sinRight = sin(radianRight);

	float rightSizeX = cos((90.0f - refAngle) * RADIAN) * p_size.x;
	float rightSizeY = (rightSizeX * p_size.y / p_size.x);
	rightSizeY *= mf_mouvement;
	sf::Vector2f rightSize(rightSizeX, rightSizeY);
	sf::Vector2f rightCenter(-rightSize.x / 2.f, -rightSize.y / 2.f);
	rotateVec(&rightCenter, cosRight, sinRight);
	rightCenter += p_center + rightUpRoot;

	// Create leaf
	if (pn_depth == DEPTH - 1 && mb_isLeaf)
	{
		float size = (m_size.y * refAngle / (m_biome->m_tree.mn_minAngle - m_biome->m_tree.mn_maxAngle)) * mf_mouvement;
		m_leafColor = m_color;
		createRectangle(rightCenter, sf::Vector2f(size, size), m_leafColor, 5, &mn_countLeaf, cosRight, sinRight);
		createRectangle(leftCenter, sf::Vector2f(size, size), m_leafColor, 5, &mn_countLeaf, cosLeft, sinLeft);
	}

	// Left recursion
	pythagorasTree(leftCenter, leftSize, rectangleAngleLeft, cosLeft, sinLeft, pn_depth + 1);

	//color += sf::Color(10, 10, 10);
	color += sf::Color(5, 5, 5);
	// Fill empty space with triangle
	createVertex(rightUpRoot + p_center, color, &mn_countTriangle);
	createVertex(leftUpRoot + p_center, color, &mn_countTriangle);
	// Get upTriangle in m_rectangle
	sf::Vector2f upTriangle(-rightSize.x, 0.f);
	rotateVec(&upTriangle, cosRight, sinRight);
	createVertex(upTriangle + p_center + rightUpRoot, color, &mn_countTriangle);

	// Right recusrion
	pythagorasTree(rightCenter, rightSize, rectangleAngleRight, cosRight, sinRight, pn_depth + 1);
}

void Tree::randomDecor(void)
{
	Decor::randomDecor();
	mf_liveTime = randomRange(m_biome->m_tree.mn_minLive, m_biome->m_tree.mn_maxLive);
	mb_growSide = static_cast<bool>(rand() % 2);
	mb_isLeaf = m_biome->mn_temperature >= 0 ? true : false;

	m_color = sf::Color(180.f, 33.f, 85.f);
	//m_leafColor = sf::Color(212.f, 185.f, 39.f);
	float tmpX = randomRange(m_biome->m_tree.mn_minSizeX, m_biome->m_tree.mn_maxSizeX);
	float tmpY = randomRange(m_biome->m_tree.mn_minSizeY, m_biome->m_tree.mn_maxSizeY);
	m_size = sf::Vector2f(tmpX, tmpY);

	for (int i = 0; i < mn_maxRectangle; i++)
		m_refAngle[i] = randomRange(m_biome->m_tree.mn_minAngle, m_biome->m_tree.mn_maxAngle);
	pythagorasTree(m_origin, m_size);
}

void Tree::init(Biome * p_biome)
{
	Decor::init(p_biome);
	randomDecor();
}

void Tree::update(float pf_deltatime)
{
	Decor::update(pf_deltatime);
	Decor::putOnMap();
	//m_origin.x -= m_size.x / 2.0f;
	m_origin.y -= m_size.y / 2.0f;

	float delta = (m_size.y - m_size.y * mf_mouvement) / 2;
	pythagorasTree(sf::Vector2f(m_origin.x, m_origin.y + delta), sf::Vector2f(m_size.x, m_size.y * mf_mouvement));
}
