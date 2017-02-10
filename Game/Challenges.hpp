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

class ChallengePixelate : public ChallengeManager::AChallenge
{
public:
	ChallengePixelate(void);
	virtual void updateShader(sf::Time frametime);
};

class ChallengeDisplacement : public ChallengeManager::AChallenge
{
public:
	ChallengeDisplacement(void);
	virtual void updateShader(sf::Time frametime);
};

class ChallengeBlur : public ChallengeManager::AChallenge
{
public:
	ChallengeBlur(void);
	virtual void updateShader(sf::Time frametime);
};

#endif
