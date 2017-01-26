#include "MysticanouilleNpc.hpp"
#include "Progress.hpp"
#include "TextManager.hpp"
#include <RectangleShape.hpp>
#include <ResourceManager.hpp>
#include <Application.hpp>

MysticanouilleNpc::MysticanouilleNpc(void) :
	ASpecialNpc(MYSTICANOUILLE_OSS)
{
	setType(GameObjectType::MysticanouilleNpc);
	setSize(sf::Vector2f(62.f, 300.f));
	setOrigin(sf::Vector2f(125.f, 64.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
	createText();
}

void MysticanouilleNpc::createText(void)
{
	TextManager & textManager = TextManager::getInstance();
	std::vector<std::wstring> const & texts = textManager.getTextsNpc(MYSTICANOUILLE_OSS);
	std::size_t portalLeft = Progress::RandomPortalMax - Progress::getInstance().countRandomDiscover();

	m_countDownText.resize(2u);
	if (portalLeft > 0u)
		m_countDownText[0] = texts[0] + L" " + std::to_wstring(portalLeft) + L" " + texts[1];
	else
		m_countDownText[0] = texts[2];
	m_countDownText[1] = L"Beurk!\n";
	ANpc::resetTexts(m_countDownText, textManager.getPriority(MYSTICANOUILLE_OSS));
}

void MysticanouilleNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void MysticanouilleNpc::update(sf::Time frameTime)
{
	ASpecialNpc::update(frameTime);

	float const distMax = 800.f;
	float const distMin = 450.f;
	sf::Vector2f const & octoPosition = Progress::getInstance().getOctoPos();
	octo::CharacterSprite & sprite = getSprite();
	float dist = std::sqrt(std::pow(getBox()->getBaryCenter().x - octoPosition.x, 2u) + std::pow(getBox()->getBaryCenter().y - octoPosition.y, 2u));
	float coef = 0.f;

	if (dist < distMin)
		coef = 1.f;
	else if (dist > distMax)
		coef = 0.f;
	else
		coef = 1.f - (dist - distMin) / (distMax - distMin);
	sprite.setColor(sf::Color(255, 255, 255, 255 * coef));
}
