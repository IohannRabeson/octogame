#include "RandomGenerator.hpp"

RandomGenerator::RandomGenerator(void)
{
}

void RandomGenerator::setSeed(std::string string)
{
	if (string == "random")
	{
		std::random_device rd;
		m_engine.seed(rd());
	}
	else
	{
		std::seed_seq seed(string.begin(), string.end());
		m_engine.seed(seed);
	}
}

float RandomGenerator::randomFloat(float min, float max)
{
	m_distributionFloat.param(std::uniform_real_distribution<float>::param_type(min, max));
	return m_distributionFloat(m_engine);
}

int RandomGenerator::randomInt(int min, int max)
{
	m_distributionInt.param(std::uniform_int_distribution<int>::param_type(min, max));
	return m_distributionInt(m_engine);
}

bool RandomGenerator::randomBool(float percent)
{
	m_distributionBool.param(std::bernoulli_distribution::param_type(percent));
	return m_distributionBool(m_engine);
}
