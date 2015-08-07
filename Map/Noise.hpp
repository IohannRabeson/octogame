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
	enum class DistanceType : std::uint8_t
	{
		Euclidean = 0u,
		EuclideanSquared = 1u,
		Manhattan = 2u,
		Chebychev = 3u,
		Quadratic = 4u,
		Minkowski = 5u
	};

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
	 * The higher the value is, the greater the performances will decrease and also details will increase
	 * \see voronoi2, voronoi3
	 */
	inline void setVoronoiMaxPoint(std::size_t maxPoint) { m_maxPoint = maxPoint; }

	/*! Get the number of points used to find the closest distance
	 * \see voronoi2, voronoi3
	 */
	inline std::size_t getVoronoiMaxPoint(void) const { return m_maxPoint; }

	/*! Set the distance type used to compute voronoi noise
	 * \see voronoi2, voronoi3
	 */
	void setDistanceType(DistanceType distanceType);

	/*! Get the distance type
	 * \see voronoi2, voronoi3
	 */
	inline DistanceType getDistanceType(void) const { return m_distanceType; }

	/*! Set the minkowski coefficient used to compute voronoi noise with minkowsky distance
	 * \see voronoi2, voronoi3
	 */
	inline void setMinkowskiCoeff(float minkowskiCoeff) { m_minkowskiCoeff = minkowskiCoeff; }

	/*! Get the minkowski coefficient
	 * \see voronoi2, voronoi3
	 */
	inline float getMinkowskiCoeff(void) const { return m_minkowskiCoeff; }

private:
	static constexpr std::size_t									MaxClosest = 100u;

	std::function<float(float, float, float, float, float, float)>	m_distanceFun3;
	std::function<float(float, float, float, float)>				m_distanceFun2;

	std::vector<int>					m_permutation;
	std::vector<float>					m_closest;
	std::size_t							m_seed;
	std::size_t							m_maxPoint;
	DistanceType						m_distanceType;
	float								m_minkowskiCoeff;

	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float grad(int hash, float x, float y);

	// Use to compute distance between points
	static float euclidean2(float x1, float y1, float x2, float y2);
	static float euclidean3(float x1, float y1, float z1, float x2, float y2, float z2);
	static float euclideanSquared2(float x1, float y1, float x2, float y2);
	static float euclideanSquared3(float x1, float y1, float z1, float x2, float y2, float z2);
	static float manhattan2(float x1, float y1, float x2, float y2);
	static float manhattan3(float x1, float y1, float z1, float x2, float y2, float z2);
	static float chebychev2(float x1, float y1, float x2, float y2);
	static float chebychev3(float x1, float y1, float z1, float x2, float y2, float z2);
	static float quadratic2(float x1, float y1, float x2, float y2);
	static float quadratic3(float x1, float y1, float z1, float x2, float y2, float z2);
	static float minkowski2(float minkowskiCoeff, float x1, float y1, float x2, float y2);
	static float minkowski3(float minkowskiCoeff, float x1, float y1, float z1, float x2, float y2, float z2);

};

#endif
