#ifndef BUBBLESCREEN_HPP
# define BUBBLESCREEN_HPP

#include "BubbleText.hpp"

# include <AbstractState.hpp>
class BubbleScreen : public octo::AbstractState
{
public:
	BubbleScreen(void);

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

private:
	BubbleText	m_bubble;
	float x = 700;
	float timer = 0.f;
};

#endif
