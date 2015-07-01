#include "RandomGenerator.hpp"

std::mt19937							RandomGenerator::s_engine;
std::uniform_real_distribution<float>	RandomGenerator::s_distributionFloat;
std::uniform_int_distribution<int>		RandomGenerator::s_distributionInt;
std::bernoulli_distribution				RandomGenerator::s_distributionBool(0.5);
std::string								RandomGenerator::s_lastSeed;

RandomGenerator::RandomGenerator(void)
{
}

RandomGenerator::RandomGenerator(std::string string)
{
	setSeed(string);
}

void RandomGenerator::setSeed(std::string string)
{
	if (string != s_lastSeed)
	{
		s_lastSeed = string;
		std::seed_seq seed(string.begin(), string.end());
		s_engine.seed(seed);
	}
	else if (string == "")
	{
		static std::random_device rd;
		s_engine.seed(rd());
	}
}

float RandomGenerator::randomFloat(float min, float max)
{
	s_distributionFloat.param(std::uniform_real_distribution<float>::param_type(min, max));
	return s_distributionFloat(s_engine);
}

int RandomGenerator::randomInt(int min, int max)
{
	s_distributionInt.param(std::uniform_int_distribution<int>::param_type(min, max));
	return s_distributionInt(s_engine);
}

bool RandomGenerator::randomBool(float percent)
{
	s_distributionBool.param(std::bernoulli_distribution::param_type(percent));
	return s_distributionBool(s_engine);
}
