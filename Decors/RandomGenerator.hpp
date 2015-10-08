#ifndef RANDOMGENERATOR_HPP
# define RANDOMGENERATOR_HPP

# include <random>
# include <boost/random.hpp>

class RandomGenerator
{
public:
	RandomGenerator(void);
	RandomGenerator(std::string const & string);
	~RandomGenerator(void) = default;

	void setSeed(std::string const & string);
	float randomFloat(float min, float max);
	int randomInt(int min, int max);
	bool randomBool(float percent);
	std::size_t randomPiecewise(std::size_t max);

private:
	boost::random::mt19937							m_engine;
	boost::random::uniform_real_distribution<float>	m_distributionFloat;
	boost::random::uniform_int_distribution<int>	m_distributionInt;
	boost::random::bernoulli_distribution<float>	m_distributionBool;
	boost::random::piecewise_linear_distribution<>	m_distributionPiecewise;
};

#endif
