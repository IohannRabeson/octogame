#ifndef CHALLENGES_HPP
# define CHALLENGES_HPP

# include "ChallengeManager.hpp"

class ChallengeDuplicate : public ChallengeManager::AChallenge
{
public:
	ChallengeDuplicate(void);
	virtual void updateShader(sf::Time frametime);

private:
	float	m_rotation;
};

class ChallengePersistence : public ChallengeManager::AChallenge
{
public:
	ChallengePersistence(void);
	virtual void updateShader(sf::Time frametime);

};

#endif
