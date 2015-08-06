#ifndef BUBBLESCREEN_HPP
# define BUBBLESCREEN_HPP

# include "BubbleManager.hpp"

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
	BubbleManager	m_bubbleManager;
};

#endif
