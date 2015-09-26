#ifndef CLASSICNPC_HPP
# define CLASSICNPC_HPP

# include "ANpc.hpp"

class ClassicNpc : public ANpc
{
public:
	ClassicNpc(ResourceKey const & npcId = OCTO_OSS);
	virtual ~ClassicNpc(void) = default;

	virtual void setup(void);

private:

};

#endif
