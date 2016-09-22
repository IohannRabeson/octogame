#ifndef ANTHEMJUNGLE_HPP
# define ANTHEMJUNGLE_HPP

# include "ASpecialNpc.hpp"

class AnthemJungle : public ASpecialNpc
{
public:
	AnthemJungle(void);
	virtual ~AnthemJungle(void) = default;

	virtual void setup(void);

};

#endif
