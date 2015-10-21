#ifndef JUMPNANOROBOT_HPP
# define JUMPNANOROBOT_HPP

# include "NanoRobot.hpp"

class JumpNanoRobot : public NanoRobot, public AGameObject<GameObjectType::JumpNanoRobot>
{
public:
	JumpNanoRobot(sf::Vector2f const & position);
	virtual ~JumpNanoRobot(void) = default;

};

#endif
