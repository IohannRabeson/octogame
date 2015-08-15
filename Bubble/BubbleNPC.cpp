#include "BubbleNPC.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

BubbleNPC::BubbleNPC(void) :
	m_lineCount(1u),
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f)
{
}

void BubbleNPC::createExtension(sf::Vector2f const & position, octo::VertexBuilder & builder)
{
	sf::Vector2f rightUp(m_sizeCorner, 0.f);
	sf::Vector2f down(m_sizeCorner, m_sizeCorner);

	rightUp = rightUp * m_animation + position;
	down = down * m_animation + position;

	builder.createTriangle(position, rightUp, down, m_color);
}

void BubbleNPC::setupBlocString(void)
{
	std::size_t j = 0u;

	auto lastSpace = m_phrase.begin();
	for (auto it = m_phrase.begin(); it != m_phrase.end(); it++)
	{
		j++;
		if (*it == ' ')
			lastSpace = it;
		if (j == m_characterPerLine)
		{
			*lastSpace = '\n';
			it = ++lastSpace;
			j = 0u;
			m_lineCount++;
		}
	}
}

void BubbleNPC::setup(std::string const & phrase, sf::Color const & color, std::size_t characterSize)
{
	if (characterSize == 0u)
		characterSize = m_characterSize;
	m_phrase = phrase;
	m_color = color;
	m_size.x = m_bubbleWidth;

	octo::ResourceManager& resources = octo::Application::getResourceManager();
	m_font = resources.getFont(VERAMONO_TTF);
	m_text.setFont(m_font);
	m_text.setColor(sf::Color(0, 0, 0));
	m_text.setCharacterSize(characterSize);
	m_text.setString(m_phrase);

	float widthTotalText = m_text.findCharacterPos(m_phrase.size() - 1).x - m_text.findCharacterPos(0).x;
	m_characterPerLine = static_cast<std::size_t>(m_size.x / (widthTotalText / m_phrase.size()));

	setupBlocString();
	m_size.y = m_lineCount * m_font.getLineSpacing(characterSize);
	if (m_lineCount == 1u)
		m_size.x = widthTotalText + m_font.getGlyph(m_phrase[m_phrase.size() - 1], characterSize, 0).advance;
	m_text.setString(m_phrase);

	m_animator.setup();
}

void BubbleNPC::update(sf::Time frameTime, octo::VertexBuilder& builder)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f position = ABubble::getPosition();
	position.y -= (m_size.y / 2.f + m_sizeCorner * 2.f) * m_animation;
	m_text.setScale(1.f, m_animation);
	ABubble::createOctogon(sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f * m_animation), m_sizeCorner * m_animation, position, m_color, builder);
	m_text.setPosition(ABubble::getTextUpLeft());
}

sf::Text BubbleNPC::getText(void) const
{
	return m_text;
}

