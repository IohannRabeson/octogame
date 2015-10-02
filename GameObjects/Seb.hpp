#ifndef SEB_HPP
# define SEB_HPP

# include "InstanceDecor.hpp"

class Seb : public InstanceDecor
{
public:
	Seb(sf::Vector2f const & scale, sf::Vector2f const & position);
	virtual ~Seb(void) = default;

	virtual void update(sf::Time frameTime);

private:
	enum State
	{
		Idle,
		Special
	};
	octo::SpriteAnimation		m_animation;
	sf::Time					m_timer;
	sf::Time					m_timerMax;
	State						m_state;

};

#endif
