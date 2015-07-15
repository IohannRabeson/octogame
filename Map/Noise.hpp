#ifndef NOISE_H
# define NOISE_H

# include <vector>

/*!
 * \ingroup Math
 * \class Noise
 * Generate coherent noise based on references values.
 * The seed can be changed.
 */
class Noise
{
public:
	Noise(void);
	Noise(std::size_t seed);
	~Noise(void) = default;

	/*! Generate a new permutation vector based on the value of seed
	 * \param seed The new seed
	 */
	void setSeed(std::size_t seed);

	/*! 2D noise */
	float noise(float x, float y);

	/*! 3D noise */
	float noise(float x, float y, float z);

	/*! The real perlin noise
	 * \param octaves Define the number of time we compute the noise
	 * \param persistence Define the influence of each consecutiv octave
	 */
	float perlinNoise(float x, float y, std::size_t octaves, float persistence);

	/*! The real perlin noise
	 * \param octaves Define the number of time we compute the noise
	 * \param persistence Define the influence of each consecutiv octave
	 */
	float perlinNoise(float x, float y, float z, std::size_t octaves, float persistence);

	/*! The Fractional Brownian Motion
	 * The images gain more detail with each octave
	 */
	float fBm(float x, float y, std::size_t octaves, float lacunarity, float gain);

	/*! The Fractional Brownian Motion
	 * The images gain more detail with each octave
	 */
	float fBm(float x, float y, float z, std::size_t octaves, float lacunarity, float gain);

private:
	std::vector<int>	m_permutation;
	std::size_t			m_seed;

	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float grad(int hash, float x, float y);

};

#endif
