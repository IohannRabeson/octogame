#ifndef JELLYFISH_HPP
# define JELLYFISH_HPP

# include "ASwimNpc.hpp"

class SkyCycle;

class JellyfishNpc : public ASwimNpc
{
public:
	JellyfishNpc(void);
	virtual ~JellyfishNpc(void) = default;

	virtual void setup(void);
};

#endif
