#include "BubbleNPC.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

BubbleNPC::BubbleNPC(void) :
	m_characterPerLine(0u),
	m_lineCount(1u)
{
}

void BubbleNPC::createExtension(sf::Vector2f const & position, octo::VertexBuilder & builder)
{
	sf::Vector2f rightUp(m_sizeCorner, 0.f);
	sf::Vector2f down(m_sizeCorner, m_sizeCorner);

	rightUp = rightUp + position;
	down = down + position;

	builder.createTriangle(position, rightUp, down, ABubble::getColor());
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
	ABubble::setColor(color);
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

	m_inactive.setup("put state here", color);
	m_inactive.setActive(true);
}

void BubbleNPC::update(sf::Time frameTime, octo::VertexBuilder& builder)
{
	if (ABubble::isActive())
	{
		sf::Vector2f position = ABubble::getPosition();
		position.y -= (m_size.y / 2.f + m_sizeCorner * 2.f);
		ABubble::createOctogon(m_size / 2.f, m_sizeCorner, position, ABubble::getColor(), true, builder);
		m_text.setPosition(ABubble::getTextUpLeft());
	}
	else
	{
		sf::Vector2f const & position = ABubble::getPosition();
		m_inactive.setPosition(position);
		m_inactive.update(frameTime, builder);
	}
}

sf::Text BubbleNPC::getText(void) const
{
	return m_text;
}

