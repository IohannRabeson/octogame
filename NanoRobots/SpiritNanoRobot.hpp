#ifndef SPIRITNANOROBOT_HPP
# define SPIRITNANOROBOT_HPP

# include "NanoRobot.hpp"

class SpiritNanoRobot : public NanoRobot, public AGameObject<GameObjectType::SpiritNanoRobot>
{
public:
	SpiritNanoRobot(sf::Vector2f const & position);
	virtual ~SpiritNanoRobot(void) = default;
	virtual void update(sf::Time frametime);

private:
	static std::size_t	s_seed;
};

#endif
