#ifndef CHECKPOINT_HPP
# define CHECKPOINT_HPP

# include "ANpc.hpp"

class CharacterOcto;

class CheckPoint : public ANpc, public AGameObject<GameObjectType::CheckPoint>
{
public:
	CheckPoint(void);
	virtual ~CheckPoint(void) = default;

	virtual void setup(void);
	virtual void collideOctoEvent(CharacterOcto * octo);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	sf::Vector2f	m_startPosition;
	bool			m_firstFrame;

};

#endif
