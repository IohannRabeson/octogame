#ifndef WATERHOUSEBROKEN_HPP
# define WATERHOUSEBROKEN_HPP

# include "AIdleNpc.hpp"

class WaterHouseBroken : public AIdleNpc
{
public:
	WaterHouseBroken(void);
	virtual ~WaterHouseBroken(void) = default;

	virtual void setup(void);

};

#endif
