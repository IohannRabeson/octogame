#include "RepairShipNanoRobot.hpp"
#include "ResourceDefinitions.hpp"

RepairShipNanoRobot::RepairShipNanoRobot(sf::Vector2f const & position) :
	NanoRobot(position, NANO_REPAIR_SHIP_OSS, 4, 11185654, sf::Vector2f(0.f, -24.f)),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(2.f))
{
	setup(this);

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(288.f, 78.f));
	targets.push_back(sf::Vector2f(269.f, 64.f));
	targets.push_back(sf::Vector2f(262.f, 73.f));
	targets.push_back(sf::Vector2f(290.f, 95.f));
	targets.push_back(sf::Vector2f(293.f, 113.f));
	targets.push_back(sf::Vector2f(254.f, 83.f));
	targets.push_back(sf::Vector2f(246.f, 93.f));
	targets.push_back(sf::Vector2f(289.f, 125.f));
	targets.push_back(sf::Vector2f(279.f, 134.f));
	targets.push_back(sf::Vector2f(239.f, 104.f));
	targets.push_back(sf::Vector2f(242.f, 121.f));
	targets.push_back(sf::Vector2f(278.f, 147.f));
	targets.push_back(sf::Vector2f(276.f, 162.f));
	targets.push_back(sf::Vector2f(244.f, 138.f));
	targets.push_back(sf::Vector2f(246.f, 158.f));
	targets.push_back(sf::Vector2f(276.f, 178.f));
	targets.push_back(sf::Vector2f(276.f, 192.f));
	targets.push_back(sf::Vector2f(250.f, 173.f));
	targets.push_back(sf::Vector2f(253.f, 191.f));
	targets.push_back(sf::Vector2f(275.f, 208.f));
	targets.push_back(sf::Vector2f(273.f, 222.f));
	targets.push_back(sf::Vector2f(255.f, 208.f));
	targets.push_back(sf::Vector2f(257.f, 226.f));
	targets.push_back(sf::Vector2f(273.f, 237.f));
	targets.push_back(sf::Vector2f(271.f, 253.f));
	targets.push_back(sf::Vector2f(260.f, 244.f));
	targets.push_back(sf::Vector2f(239.f, 104.f));
	targets.push_back(sf::Vector2f(269.f, 64.f));
	setTargets(targets, 0.6f);
	setUsePathLaser(true);

	setSwarmTarget(position);
}

static sf::Color makeRainbow(float step)
{
	float r = 0.f;
	float g = 0.f;
	float b = 0.f;
	int i = static_cast<int>(step * 6.f);
	float f = step * 6.f - i;
	float q = 1.f - f;

	switch (i % 6)
	{
		case 0:
			r = 1.f;
			g = f;
			b = 0.f;
			break;
		case 1:
			r = q;
			g = 1.f;
			b = 0.f;
			break;
		case 2:
			r = 0.f;
			g = 1.f;
			b = f;
			break;
		case 3:
			r = 0.f;
			g = q;
			b = 1.f;
			break;
		case 4:
			r = f;
			g = 0.f;
			b = 1.f;
			break;
		case 5:
			r = 1.f;
			g = 0.f;
			b = q;
			break;
	}
	return sf::Color(r * 255.f, g * 255.f, b * 255.f);
}

void RepairShipNanoRobot::updateRepairShip(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer > m_timerMax)
		m_timer -= m_timerMax;
	setRepairShipPosition(sf::Vector2f(220.f, -150.f));
	setLaserColor(makeRainbow(m_timer / m_timerMax));
	setLaserConvergence(getPosition() + sf::Vector2f(0.f, -24.f));
}
