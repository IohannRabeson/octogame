#include "BubbleScreen.hpp"
#include "ABubble.hpp"

BubbleScreen::BubbleScreen(void)
{
}

void	BubbleScreen::start()
{
	m_bubble.setup("dskjsgs sd sdfkgjhsd fg sdkfjhg ksdjfhg sdkfjgh sdkjfhg ksdjfhg ksdfjhg ksdfjhg sdkfjhg ksh", sf::Color(255, 255, 255, 200));
	m_bubble.setType(ABubble::Type::None);
}

void	BubbleScreen::pause()
{
}

void	BubbleScreen::resume()
{
}

void	BubbleScreen::stop()
{
}

void	BubbleScreen::update(sf::Time frameTime)
{
	m_bubble.update(frameTime);
	x -= frameTime.asSeconds() * 30;
	m_bubble.setPosition(sf::Vector2f(x, 500));
}

void	BubbleScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_bubble);
}


