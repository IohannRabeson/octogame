#ifndef JELLYFISH_HPP
# define JELLYFISH_HPP

# include "ASwimNpc.hpp"

class SkyCycle;

class JellyfishNpc : public ASwimNpc
{
public:
	JellyfishNpc(ABiome & biome);
	virtual ~JellyfishNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void			update(sf::Time frametime);
};

#endif
