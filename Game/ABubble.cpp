#include "ABubble.hpp"
#include "ABiome.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

ABubble::ABubble(void) :
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f)
{
}

void ABubble::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x + sizeCorner.x, -size.y);
	sf::Vector2f upRight(size.x - sizeCorner.x, -size.y);
	sf::Vector2f upMidLeft(-size.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidRight(size.x, -size.y + sizeCorner.y);
	sf::Vector2f downLeft(-size.x + sizeCorner.x, size.y);
	sf::Vector2f downRight(size.x - sizeCorner.x, size.y);
	sf::Vector2f downMidLeft(-size.x, size.y - sizeCorner.y);
	sf::Vector2f downMidRight(size.x, size.y - sizeCorner.y);

	upLeft += origin;
	upRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	downMidLeft += origin;
	downMidRight += origin;

	builder.createTriangle(origin, upLeft, upRight, color);
	builder.createTriangle(origin, upRight, upMidRight, color);
	builder.createTriangle(origin, upMidRight, downMidRight, color);
	builder.createTriangle(origin, downMidRight, downRight, color);
	builder.createTriangle(origin, downRight, downLeft, color);
	builder.createTriangle(origin, downLeft, downMidLeft, color);
	builder.createTriangle(origin, downMidLeft, upMidLeft, color);
	builder.createTriangle(origin, upMidLeft, upLeft, color);

	m_textUpLeft = sf::Vector2f(-size.x + sizeCorner.x, -size.y + sizeCorner.y) + origin;
	m_extensionPos = downLeft;
}

/*
void ABubble::createExtension(sf::Vector2f const & position, sf::Vector2f const & size)
{

}
*/

void ABubble::createABubble(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color color, octo::VertexBuilder& builder)
{
	createOctogon(size, sizeCorner, origin, color, builder);
}

void ABubble::setup(void)
{
	m_size = sf::Vector2f(300.f, 400.f);
	m_sizeCorner = sf::Vector2f(50.f, 50.f);
	m_color = sf::Color(255, 255, 255, 200);

	octo::ResourceManager& resources = octo::Application::getResourceManager();
	m_text.setFont(resources.getFont(VERAMONO_TTF));
	m_text.setString("Salut, ceci est un test. Octo est un petit poulpe aventurier, partant decouvrir les confins de l'univers.");

	m_animator.setup();
}

sf::Text ABubble::getText(void) const
{
	return m_text;
}

void ABubble::update(sf::Time frameTime, octo::VertexBuilder& builder)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f position(900.f, 500.f);
	createABubble(m_size * m_animation, m_sizeCorner * m_animation, position, m_color, builder);
	m_text.setPosition(m_textUpLeft);
}
