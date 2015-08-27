#include "ShineBuilder.hpp"
#include "ABiome.hpp"
#include <Math.hpp>

ShineBuilder::ShineBuilder()
{
}

void ShineBuilder::createStar(sf::Vector2f const & size, sf::Vector2f const & sizeHeart, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder, float const angle, float const cosAngle, float const sinAngle)
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
		octo::rotateVector(left, cosAngle, sinAngle);
		octo::rotateVector(right, cosAngle, sinAngle);
		octo::rotateVector(up, cosAngle, sinAngle);
		octo::rotateVector(down, cosAngle, sinAngle);
		octo::rotateVector(heartLeftUp, cosAngle, sinAngle);
		octo::rotateVector(heartRightUp, cosAngle, sinAngle);
		octo::rotateVector(heartLeftDown, cosAngle, sinAngle);
		octo::rotateVector(heartRightDown, cosAngle, sinAngle);
	}

	left += origin;
	right += origin;
	up += origin;
	down += origin;
	heartLeftUp += origin;
	heartRightUp += origin;
	heartLeftDown += origin;
	heartRightDown += origin;

	builder.createTriangle(up, heartLeftUp, heartRightUp, color);
	builder.createTriangle(down, heartLeftDown, heartRightDown, color);
	builder.createTriangle(left, heartLeftUp, heartLeftDown, color);
	builder.createTriangle(right, heartRightUp, heartRightDown, color);
	builder.createQuad(heartLeftUp, heartRightUp, heartRightDown, heartLeftDown, color);
}

void ShineBuilder::createGlow(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder, float const angle, float const cosAngle, float const sinAngle)
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
		octo::rotateVector(upLeft, cosAngle, sinAngle);
		octo::rotateVector(upRight, cosAngle, sinAngle);
		octo::rotateVector(cornerUpLeft, cosAngle, sinAngle);
		octo::rotateVector(cornerUpRight, cosAngle, sinAngle);
		octo::rotateVector(upMidLeft, cosAngle, sinAngle);
		octo::rotateVector(upMidRight, cosAngle, sinAngle);
		octo::rotateVector(downLeft, cosAngle, sinAngle);
		octo::rotateVector(downRight, cosAngle, sinAngle);
		octo::rotateVector(cornerDownLeft, cosAngle, sinAngle);
		octo::rotateVector(cornerDownRight, cosAngle, sinAngle);
		octo::rotateVector(downMidLeft, cosAngle, sinAngle);
		octo::rotateVector(downMidRight, cosAngle, sinAngle);
	}

	upLeft += origin;
	upRight += origin;
	cornerUpLeft += origin;
	cornerUpRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	cornerDownLeft += origin;
	cornerDownRight += origin;
	downMidLeft += origin;
	downMidRight += origin;

	sf::Color transparent = color;
	transparent.a = 0;

	builder.createVertex(upLeft, transparent);
	builder.createVertex(upRight, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(upRight, transparent);
	builder.createVertex(upMidRight, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(upMidRight, transparent);
	builder.createVertex(downMidRight, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(downMidRight, transparent);
	builder.createVertex(downRight, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(downRight, transparent);
	builder.createVertex(downLeft, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(downLeft, transparent);
	builder.createVertex(downMidLeft, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(downMidLeft, transparent);
	builder.createVertex(upMidLeft, transparent);
	builder.createVertex(origin, color);

	builder.createVertex(upMidLeft, transparent);
	builder.createVertex(upLeft, transparent);
	builder.createVertex(origin, color);
}

