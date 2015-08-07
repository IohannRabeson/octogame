#include "Noise.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

Noise::Noise(void) :
	Noise(0u)
{
	m_permutation = {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
	43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
	97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
	107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
	43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
	97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
	107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
}

Noise::Noise(std::size_t seed) :
	m_seed(seed),
	m_maxPoint(1u)
{
	setSeed(seed);
	m_closest.resize(MaxClosest, std::numeric_limits<float>::max());
}

void Noise::setSeed(std::size_t seed)
{
	m_seed = seed;
	m_permutation.resize(256);
	std::iota(m_permutation.begin(), m_permutation.end(), 0);
	std::default_random_engine engine(seed);
	std::shuffle(m_permutation.begin(), m_permutation.end(), engine);
	m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());
}

float Noise::noise(float x, float y)
{
	// Get the integer part
	int X = x < 0 ? static_cast<int>(x) - 1 : static_cast<int>(x);
	int Y = y < 0 ? static_cast<int>(y) - 1 : static_cast<int>(y);

	// Get the fractionnal part
	x -= static_cast<float>(X);
	y -= static_cast<float>(Y);

	// Get the 8 last bit
	X &= 0xFF;
	Y &= 0xFF;

	// Compute fade curves for each of x, y
	float u = fade(x);
	float v = fade(y);

	// Hash coordinates of the 4 square corners
	int g00 = m_permutation[X] + Y;
	int g10 = m_permutation[X + 1] + Y;
	int g01 = g00 + 1;
	int g11 = g10 + 1;

	float x0 = lerp(u, grad(m_permutation[g00], x, y), grad(m_permutation[g10], x - 1.f, y));
	float x1 = lerp(u, grad(m_permutation[g01], x, y - 1.f), grad(m_permutation[g11], x - 1.f, y - 1.f));

	float xy = lerp(v, x0, x1);

	return xy;
}

float Noise::noise(float x, float y, float z)
{
	// Get the integer part
	int X = x < 0 ? static_cast<int>(x) - 1 : static_cast<int>(x);
	int Y = y < 0 ? static_cast<int>(y) - 1 : static_cast<int>(y);
	int Z = z < 0 ? static_cast<int>(z) - 1 : static_cast<int>(z);

	// Get the fractionnal part
	x -= static_cast<float>(X);
	y -= static_cast<float>(Y);
	z -= static_cast<float>(Z);

	// Get the 8 last bit
	X &= 0xFF;
	Y &= 0xFF;
	Z &= 0xFF;

	// Compute fade curves for each of x, y, z
	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	// Hash coordinates of the 8 cube corners
	int g00 = m_permutation[X] + Y;
	int g10 = m_permutation[X + 1] + Y;

	int g000 = m_permutation[g00] + Z;
	int g001 = g000 + 1;
	int g010 = m_permutation[g00 + 1] + Z;
	int g011 = g010 + 1;
	int g100 = m_permutation[g10] + Z;
	int g101 = g100 + 1;
	int g110 = m_permutation[g10 + 1] + Z;
	int g111 = g110 + 1;

	float x00 = lerp(u, grad(m_permutation[g000], x, y, z), grad(m_permutation[g100], x - 1.f, y, z));
	float x10 = lerp(u, grad(m_permutation[g010], x, y - 1.f, z), grad(m_permutation[g110], x - 1.f, y - 1.f, z));
	float x01 = lerp(u, grad(m_permutation[g001], x, y, z - 1.f), grad(m_permutation[g101], x - 1.f, y, z - 1.f));
	float x11 = lerp(u, grad(m_permutation[g011], x, y - 1.f, z - 1.f), grad(m_permutation[g111], x - 1.f, y - 1.f, z - 1.f));

	float xy0 = lerp(v, x00, x10);
	float xy1 = lerp(v, x01, x11);

	float xyz = lerp(w, xy0, xy1);

	return xyz;
}

float Noise::perlin(float x, float y, std::size_t octaves, float persistence)
{
	float amplitude = 1.f;
	float frequency = 1.f;
	float sum = 0.f;
	float max = 0.f;
	for (std::size_t i = 0u; i < octaves; i++)
	{
		sum += noise(x * frequency, y * frequency) * amplitude;
		max += amplitude;
		amplitude *= persistence;
		frequency *= 2.f;
	}
	return (sum / max);
}

float Noise::perlin(float x, float y, float z, std::size_t octaves, float persistence)
{
	float amplitude = 1.f;
	float frequency = 1.f;
	float sum = 0.f;
	float max = 0.f;
	for (std::size_t i = 0u; i < octaves; i++)
	{
		sum += noise(x * frequency, y * frequency, z * frequency) * amplitude;
		max += amplitude;
		amplitude *= persistence;
		frequency *= 2.f;
	}
	return (sum / max);
}

float Noise::fBm(float x, float y, std::size_t octaves, float lacunarity, float gain)
{
	float amplitude = 1.f;
	float frequency = 1.f;
	float sum = 0.f;
	for (std::size_t i = 0u; i < octaves; i++)
	{
		x *= frequency;
		y *= frequency;
		sum += noise(x, y) * amplitude;
		amplitude *= gain;
		frequency *= lacunarity;
	}
	return sum;
}

float Noise::fBm(float x, float y, float z, std::size_t octaves, float lacunarity, float gain)
{
	float amplitude = 1.f;
	float frequency = 1.f;
	float sum = 0.f;
	for (std::size_t i = 0u; i < octaves; i++)
	{
			x *= frequency;
			y *= frequency;
			z *= frequency;
		sum += noise(x, y, z) * amplitude;
		amplitude *= gain;
		frequency *= lacunarity;
	}
	return sum;
}

float euclidean(float x1, float y1, float x2, float y2)
{
	float dif_x = x1 - x2;
	float dif_y = y1 - y2;

	return pow(dif_x * dif_x + dif_y * dif_y, 0.5f);
}

float euclidean3(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dif_x = x1 - x2;
	float dif_y = y1 - y2;
	float dif_z = z1 - z2;

	return pow(dif_x * dif_x + dif_y * dif_y + dif_z * dif_z, 0.5f);
}

float Noise::voronoi2(float x, float y, std::size_t F)
{
	std::size_t maxClosest = F + 1u;
	std::size_t points;
	float tmpDist;
	float tmpX;
	float tmpY;
	int lastRand;

	// Get the integer part
	int X = x < 0 ? static_cast<int>(x) - 1 : static_cast<int>(x);
	int Y = y < 0 ? static_cast<int>(y) - 1 : static_cast<int>(y);

	// Get the fractionnal part
	x -= static_cast<float>(X);
	y -= static_cast<float>(Y);

	// Get the 8 last bit
	X &= 0xFF;
	Y &= 0xFF;

	// Init the closest distances
	for (std::size_t i = 0u; i < maxClosest; i++)
		m_closest[i] = std::numeric_limits<float>::max();

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			lastRand = m_permutation[X + i + 1] + Y + j + 1;
			points = lastRand % m_maxPoint + 1;
			for (std::size_t k = 0u; k < points; k++)
			{
				lastRand = m_permutation[lastRand];
				tmpX = static_cast<float>(lastRand) / 256.f;
				lastRand = m_permutation[lastRand];
				tmpY = static_cast<float>(lastRand) / 256.f;
				tmpDist = euclidean(tmpX + i, tmpY + j, x, y);

				// Sorted insert, to get the closest point at index 0
				for (std::size_t l = 0u; l < maxClosest; l++)
				{
					if (tmpDist < m_closest[l])
					{
						for (std::size_t m = l; m < maxClosest - 1u; m++)
							m_closest[m + 1u] = m_closest[m];
						m_closest[l] = tmpDist;
						break;
					}
				}
			}
		}
	}
	return m_closest[F];
}

float Noise::voronoi2(float x, float y)
{
	std::size_t points;
	float closest = std::numeric_limits<float>::max();
	float tmpDist;
	float tmpX;
	float tmpY;
	int lastRand;

	// Get the integer part
	int X = x < 0 ? static_cast<int>(x) - 1 : static_cast<int>(x);
	int Y = y < 0 ? static_cast<int>(y) - 1 : static_cast<int>(y);

	// Get the fractionnal part
	x -= static_cast<float>(X);
	y -= static_cast<float>(Y);

	// Get the 8 last bit
	X &= 0xFF;
	Y &= 0xFF;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			lastRand = m_permutation[X + i + 1] + Y + j + 1;
			points = lastRand % m_maxPoint + 1;
			for (std::size_t k = 0u; k < points; k++)
			{
				lastRand = m_permutation[lastRand];
				tmpX = static_cast<float>(lastRand) / 256.f;
				lastRand = m_permutation[lastRand];
				tmpY = static_cast<float>(lastRand) / 256.f;
				tmpDist = euclidean(tmpX + i, tmpY + j, x, y);

				// Get the closest point
				if (tmpDist < closest)
					closest = tmpDist;
			}
		}
	}
	return closest;
}

float Noise::voronoi3(float x, float y, float z, std::size_t F)
{
	std::size_t maxClosest = F + 1u;
	std::size_t points;
	float tmpDist;
	float tmpX;
	float tmpY;
	float tmpZ;
	int lastRand;

	// Get the integer part
	int X = x < 0 ? static_cast<int>(x) - 1 : static_cast<int>(x);
	int Y = y < 0 ? static_cast<int>(y) - 1 : static_cast<int>(y);
	int Z = z < 0 ? static_cast<int>(z) - 1 : static_cast<int>(z);

	// Get the fractionnal part
	x -= static_cast<float>(X);
	y -= static_cast<float>(Y);
	z -= static_cast<float>(Z);

	// Get the 8 last bit
	X &= 0xFF;
	Y &= 0xFF;
	Z &= 0xFF;

	// Init the closest distances
	for (std::size_t i = 0u; i < maxClosest; i++)
		m_closest[i] = std::numeric_limits<float>::max();

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int d = -1; d < 2; d++)
			{
				lastRand = m_permutation[m_permutation[X + i + 1] + Y + j + 1] + Z + d + 1;
				points = lastRand % m_maxPoint + 1;
				for (std::size_t k = 0u; k < points; k++)
				{
					lastRand = m_permutation[lastRand];
					tmpX = static_cast<float>(lastRand) / 256.f;
					lastRand = m_permutation[lastRand];
					tmpY = static_cast<float>(lastRand) / 256.f;
					lastRand = m_permutation[lastRand];
					tmpZ = static_cast<float>(lastRand) / 256.f;
					tmpDist = euclidean3(tmpX + i, tmpY + j, tmpZ + d, x, y, z);

					// Sorted insert, to get the closest point at index 0
					for (std::size_t l = 0u; l < maxClosest; l++)
					{
						if (tmpDist < m_closest[l])
						{
							for (std::size_t m = l; m < maxClosest - 1u; m++)
								m_closest[m + 1u] = m_closest[m];
							m_closest[l] = tmpDist;
							break;
						}
					}
				}
			}
		}
	}
	return m_closest[F];
}

float Noise::voronoi3(float x, float y, float z)
{
	std::size_t points;
	float closest = std::numeric_limits<float>::max();
	float tmpDist;
	float tmpX;
	float tmpY;
	float tmpZ;
	int lastRand;

	// Get the integer part
	int X = x < 0 ? static_cast<int>(x) - 1 : static_cast<int>(x);
	int Y = y < 0 ? static_cast<int>(y) - 1 : static_cast<int>(y);
	int Z = z < 0 ? static_cast<int>(z) - 1 : static_cast<int>(z);

	// Get the fractionnal part
	x -= static_cast<float>(X);
	y -= static_cast<float>(Y);
	z -= static_cast<float>(Z);

	// Get the 8 last bit
	X &= 0xFF;
	Y &= 0xFF;
	Z &= 0xFF;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int d = -1; d < 2; d++)
			{
				lastRand = m_permutation[m_permutation[X + i + 1] + Y + j + 1] + Z + d + 1;
				points = lastRand % m_maxPoint + 1;
				for (std::size_t k = 0u; k < points; k++)
				{
					lastRand = m_permutation[lastRand];
					tmpX = static_cast<float>(lastRand) / 256.f;
					lastRand = m_permutation[lastRand];
					tmpY = static_cast<float>(lastRand) / 256.f;
					lastRand = m_permutation[lastRand];
					tmpZ = static_cast<float>(lastRand) / 256.f;
					tmpDist = euclidean3(tmpX + i, tmpY + j, tmpZ + d, x, y, z);

					// Get the closest point
					if (tmpDist < closest)
						closest = tmpDist;
				}
			}
		}
	}
	return closest;
}

float Noise::fade(float t)
{
	// Used for bump mapping (more precision)
	//return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
	return t * t * (3.f - 2.f * t);
}

float Noise::lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float Noise::grad(int hash, float x, float y)
{
	switch (hash & 0xF)
	{
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x;
		case 0x5: return -x;
		case 0x6: return  x;
		case 0x7: return -x;
		case 0x8: return  y;
		case 0x9: return -y;
		case 0xA: return  y;
		case 0xB: return -y;
		case 0xC: return  y + x;
		case 0xD: return -y;
		case 0xE: return  y - x;
		case 0xF: return -y;
		default: return 0.f; // never happens
	}
}

float Noise::grad(int hash, float x, float y, float z)
{
	switch (hash & 0xF)
	{
		case 0x0: return  x + y;
		case 0x1: return -x + y;
		case 0x2: return  x - y;
		case 0x3: return -x - y;
		case 0x4: return  x + z;
		case 0x5: return -x + z;
		case 0x6: return  x - z;
		case 0x7: return -x - z;
		case 0x8: return  y + z;
		case 0x9: return -y + z;
		case 0xA: return  y - z;
		case 0xB: return -y - z;
		case 0xC: return  y + x;
		case 0xD: return -x + y;
		case 0xE: return -y + z;
		case 0xF: return -y - z;
		default: return 0.f; // never happens
	}
}
