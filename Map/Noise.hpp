#ifndef NOISE_H
# define NOISE_H

# include <vector>
# include <functional>

/*!
 * \ingroup Math
 * \class Noise
 * Generate coherent noise based on references values.
 * The seed can be changed.
 */
class Noise
{
public:
	typedef std::function<float(float x, float y)>	DistanceFunc;

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
	float perlin(float x, float y, std::size_t octaves, float persistence);

	/*! The real perlin noise
	 * \param octaves Define the number of time we compute the noise
	 * \param persistence Define the influence of each consecutiv octave
	 */
	float perlin(float x, float y, float z, std::size_t octaves, float persistence);

	/*! The Fractional Brownian Motion
	 * The images gain more detail with each octave
	 */
	float fBm(float x, float y, std::size_t octaves, float lacunarity, float gain);

	/*! The Fractional Brownian Motion
	 * The images gain more detail with each octave
	 */
	float fBm(float x, float y, float z, std::size_t octaves, float lacunarity, float gain);

	/*! The voronoi noise also called CellNoise or WorleyNoise
	 * The returned value is not always between 0 - 1
	 * \param F The n closest point, F < maxClosest */
	float voronoi3(float x, float y, float z, std::size_t F);

	/*! The voronoi noise also called CellNoise or WorleyNoise
	 * The returned value is not always between 0 - 1
	 */
	float voronoi3(float x, float y, float z);

	/*! The voronoi noise also called CellNoise or WorleyNoise
	 * The returned value is not always between 0 - 1
	 * \param F The n closest point, F < maxClosest */
	float voronoi2(float x, float y, std::size_t F);

	/*! The voronoi noise also called CellNoise or WorleyNoise
	 * The returned value is not always between 0 - 1
	 */
	float voronoi2(float x, float y);

	/*! The number of points used to find the closest distance
	 * The higher the value is, the greater the performances will decrease and details will increase */
	inline void setVoronoiMaxPoint(std::size_t maxPoint) { m_maxPoint = maxPoint; }

	/*! Get the number of points used to find the closest distance */
	inline std::size_t getVoronoiMaxPoint(void) const { return m_maxPoint; }

private:
	static constexpr std::size_t	MaxClosest = 100u;

	std::vector<int>				m_permutation;
	std::vector<float>				m_closest;
	std::size_t						m_seed;
	std::size_t						m_maxPoint;

	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float grad(int hash, float x, float y);

};

#endif
