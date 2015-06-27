#include "Star.hpp"
#include "ABiome.hpp"
#include <math.hpp>
#include <cmath>

Star::Star() :
	m_angle(0.f),
	m_animator(3.f, 3.f, 3.f, 0.3f),
	m_animation(1.f)
{
}

Star::Star(sf::Vector2f const & size, sf::Color const & color, float const angle) :
	m_size(size),
	m_color(color),
	m_sizeHeart(size / 50.f),
	m_angle(angle),
	m_animator(3.f, 3.f, 0.f, 0.1f),
	m_animation(1.f),
	m_glowSize(size / 2.5f),
	m_glowSizeCorner(m_glowSize / 2.f)
{
}

void Star::createStar(sf::Vector2f const & size, sf::Vector2f const & sizeHeart, sf::Vector2f const & origin, sf::Color const & color, float const angle, float const cosAngle, float const sinAngle, octo::VertexBuilder& builder)
{
	sf::Vector2f left(-size.x / 2.f - sizeHeart.x, 0.f);
	sf::Vector2f right(size.x / 2.f + sizeHeart.x, 0.f);
	sf::Vector2f up(0.f, -size.y / 2.f - sizeHeart.y);
	sf::Vector2f down(0.f, size.y / 2.f + sizeHeart.y);
	sf::Vector2f heartLeftUp(-sizeHeart.x, -sizeHeart.y);
	sf::Vector2f heartRightUp(sizeHeart.x, -sizeHeart.y);
	sf::Vector2f heartLeftDown(-sizeHeart.x, sizeHeart.y);
	sf::Vector2f heartRightDown(sizeHeart.x, sizeHeart.y);

	if (angle != 0.f)
	{
		rotateVec(left, cosAngle, sinAngle);
		rotateVec(right, cosAngle, sinAngle);
		rotateVec(up, cosAngle, sinAngle);
		rotateVec(down, cosAngle, sinAngle);
		rotateVec(heartLeftUp, cosAngle, sinAngle);
		rotateVec(heartRightUp, cosAngle, sinAngle);
		rotateVec(heartLeftDown, cosAngle, sinAngle);
		rotateVec(heartRightDown, cosAngle, sinAngle);
	}

	builder.createTriangle(up + origin, heartLeftUp + origin, heartRightUp + origin, color);
	builder.createTriangle(down + origin, heartLeftDown + origin, heartRightDown + origin, color);
	builder.createTriangle(left + origin, heartLeftUp + origin, heartLeftDown + origin, color);
	builder.createTriangle(right + origin, heartRightUp + origin, heartRightDown + origin, color);
	builder.createQuad(heartLeftUp + origin, heartRightUp + origin, heartRightDown + origin, heartLeftDown + origin, color);
}

void Star::createGlow(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color const & color, float const angle, float const cosAngle, float const sinAngle, octo::VertexBuilder& builder)
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

	if (angle != 0.f)
	{
		rotateVec(upLeft, cosAngle, sinAngle);
		rotateVec(upRight, cosAngle, sinAngle);
		rotateVec(cornerUpLeft, cosAngle, sinAngle);
		rotateVec(cornerUpRight, cosAngle, sinAngle);
		rotateVec(upMidLeft, cosAngle, sinAngle);
		rotateVec(upMidRight, cosAngle, sinAngle);
		rotateVec(downLeft, cosAngle, sinAngle);
		rotateVec(downRight, cosAngle, sinAngle);
		rotateVec(cornerDownLeft, cosAngle, sinAngle);
		rotateVec(cornerDownRight, cosAngle, sinAngle);
		rotateVec(downMidLeft, cosAngle, sinAngle);
		rotateVec(downMidRight, cosAngle, sinAngle);
	}

	sf::Color originColor = color;
	originColor.a = originColor.a;
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
	m_sizeHeart = m_size / 50.f;

	m_glowSize = m_size / 2.5f;
	m_glowSizeCorner = m_glowSize / 2.f;
	m_animator.setup(biome.getStarLifeTime());
}

void Star::shine(sf::Time frameTime, octo::VertexBuilder& builder, sf::Vector2f const & position)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	float angle = m_angle * m_animation * octo::Deg2Rad;
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);

	sf::Color animationColor(m_color.r, m_color.g, m_color.b, m_color.a * (m_animation > 1.f ? 1.f : m_animation));
	createStar(m_size * m_animation, m_sizeHeart * m_animation, position, animationColor, m_angle, cosAngle, sinAngle, builder);
	createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, animationColor, m_angle, cosAngle, sinAngle, builder);
}

void Star::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();
	float angle = m_angle * m_animation * octo::Deg2Rad;
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);

	sf::Color animationColor(m_color.r, m_color.g, m_color.b, m_color.a * (m_animation > 1.f ? 1.f : m_animation));
	createStar(m_size * m_animation, m_sizeHeart * m_animation, position, animationColor, m_angle, cosAngle, sinAngle, builder);
	createGlow(m_glowSize * m_animation, m_glowSizeCorner * m_animation, position, animationColor, m_angle, cosAngle, sinAngle, builder);
}

void Star::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}
