#ifndef RANDOMGENERATOR_HPP
# define RANDOMGENERATOR_HPP

# include <random>

class RandomGenerator
{
public:
	RandomGenerator(void);
	RandomGenerator(std::string string);
	~RandomGenerator(void) = default;

	static void setSeed(std::string string);
	float randomFloat(float min, float max);
	int randomInt(int min, int max);
	bool randomBool(float percent);

private:
	static std::mt19937								s_engine;
	static std::uniform_real_distribution<float>	s_distributionFloat;
	static std::uniform_int_distribution<int>		s_distributionInt;
	static std::bernoulli_distribution				s_distributionBool;
	static std::string								s_lastSeed;
};

#endif
