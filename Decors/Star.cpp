#include "Star.hpp"
#include "ABiome.hpp"
#include <cmath>

Star::Star() :
	m_liveTime(sf::Time::Zero),
	m_animation(1.f)
{
}

void Star::createStar(sf::Vector2f const & size, sf::Vector2f const & sizeHeart, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f left(-size.x / 2.f - sizeHeart.x, 0.f);
	sf::Vector2f right(size.x / 2.f + sizeHeart.x, 0.f);
	sf::Vector2f up(0.f, -size.y / 2.f - sizeHeart.y);
	sf::Vector2f down(0.f, size.y / 2.f + sizeHeart.y);
	sf::Vector2f heartLeftUp(-sizeHeart.x, -sizeHeart.y);
	sf::Vector2f heartRightUp(sizeHeart.x, -sizeHeart.y);
	sf::Vector2f heartLeftDown(-sizeHeart.x, sizeHeart.y);
	sf::Vector2f heartRightDown(sizeHeart.x, sizeHeart.y);

	builder.createTriangle(up + origin, heartLeftUp + origin, heartRightUp + origin, color);
	builder.createTriangle(down + origin, heartLeftDown + origin, heartRightDown + origin, color);
	builder.createTriangle(left + origin, heartLeftUp + origin, heartLeftDown + origin, color);
	builder.createTriangle(right + origin, heartRightUp + origin, heartRightDown + origin, color);
	builder.createQuad(heartLeftUp + origin, heartRightUp + origin, heartRightDown + origin, heartLeftDown + origin, color);
}

void Star::createGlow(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x + sizeCorner.x, -size.y);
	sf::Vector2f upRight(size.x - sizeCorner.x, -size.y);
	sf::Vector2f cornerUpLeft(-size.x + sizeCorner.x, -size.y + sizeCorner.y);
	sf::Vector2f cornerUpRight(size.x - sizeCorner.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidLeft(-size.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidRight(size.x, -size.y + sizeCorner.y);
	sf::Vector2f downLeft(-size.x + sizeCorner.x, size.y);
	sf::Vector2f downRight(size.x - sizeCorner.x, size.y);
	sf::Vector2f cornerDownLeft(-size.x + sizeCorner.x, size.y - sizeCorner.y);
	sf::Vector2f cornerDownRight(size.x - sizeCorner.x, size.y - sizeCorner.y);
	sf::Vector2f downMidLeft(-size.x, size.y - sizeCorner.y);
	sf::Vector2f downMidRight(size.x, size.y - sizeCorner.y);

	sf::Color originColor = color;
	originColor.a = 100;
	sf::Color transparent = color;
	transparent.a = 0;

	builder.createVertex(upLeft + origin, transparent);
	builder.createVertex(upRight + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(upRight + origin, transparent);
	builder.createVertex(upMidRight + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(upMidRight + origin, transparent);
	builder.createVertex(downMidRight + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(downMidRight + origin, transparent);
	builder.createVertex(downRight + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(downRight + origin, transparent);
	builder.createVertex(downLeft + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(downLeft + origin, transparent);
	builder.createVertex(downMidLeft + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(downMidLeft + origin, transparent);
	builder.createVertex(upMidLeft + origin, transparent);
	builder.createVertex(origin, originColor);

	builder.createVertex(upMidLeft + origin, transparent);
	builder.createVertex(upLeft + origin, transparent);
	builder.createVertex(origin, originColor);
}

void Star::setup(ABiome& biome)
{
	m_size = biome.getStarSize();
	m_color = biome.getStarColor();
	m_sizeHeart = sf::Vector2f(m_size.x / 50.f, m_size.x / 50.f);
	m_liveTime = biome.getStarLifeTime();

	m_glowSize = m_size / 2.5f;
	m_glowSizeCorner = m_glowSize / 2.f;
}

void Star::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	(void)frameTime;

	sf::Vector2f const & position = getPosition();
	createStar(m_size * m_animation, m_sizeHeart * m_animation, position, m_color, builder);
	createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, m_color, builder);
}

