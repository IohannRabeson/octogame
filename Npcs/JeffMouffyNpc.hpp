#ifndef JEFFMOUFFY_HPP
# define JEFFMOUFFY_HPP

# include "ASpecialNpc.hpp"

class JeffMouffyNpc : public ASpecialNpc
{
public:
	JeffMouffyNpc(void);
	virtual ~JeffMouffyNpc(void) = default;

	virtual void setup(void);
	virtual void setPosition(sf::Vector2f const & position);

protected:
	virtual void updateState(void);
	virtual void updatePhysics(void);

private:
	bool			m_startTimer;
	bool			m_animationEnd;
	sf::Vector2f	m_startPosition;
	bool			m_isMet;
};

#endif
