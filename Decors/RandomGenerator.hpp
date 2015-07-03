#ifndef RANDOMGENERATOR_HPP
# define RANDOMGENERATOR_HPP

# include <random>

class RandomGenerator
{
public:
	RandomGenerator(void);
	~RandomGenerator(void) = default;

	void setSeed(std::string const & string);
	float randomFloat(float min, float max);
	int randomInt(int min, int max);
	bool randomBool(float percent);
	std::size_t randomPiecewise(std::size_t max);

private:
	std::mt19937							m_engine;
	std::uniform_real_distribution<float>	m_distributionFloat;
	std::uniform_int_distribution<int>		m_distributionInt;
	std::bernoulli_distribution				m_distributionBool;
	std::piecewise_linear_distribution<>	m_distributionPiecewise;
};

#endif
