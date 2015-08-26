#include "BubbleScreen.hpp"

BubbleScreen::BubbleScreen(void)
{
}

void	BubbleScreen::start()
{
	m_bubbleManager.setup();
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
	m_bubbleManager.update(frameTime, sf::Vector2f(400, 600));
}

void	BubbleScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_bubbleManager);
}


