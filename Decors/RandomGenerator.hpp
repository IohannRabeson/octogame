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
	static float randomFloat(float min, float max);
	static int randomInt(int min, int max);

private:
	static std::mt19937								s_engine;
	static std::uniform_real_distribution<float>	s_distributionFloat;
	static std::uniform_int_distribution<int>		s_distributionInt;
	static std::string								s_lastSeed;
};

#endif
