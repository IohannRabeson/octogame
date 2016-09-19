#include "SnowGirl1Npc.hpp"

#include <Application.hpp>
#include <Console.hpp>
SnowGirl1Npc::SnowGirl1Npc(void) :
	ASpecialNpc(SNOWGIRL_1_OSS, false)
{
	setSize(sf::Vector2f(50.f, 120.f));
	setOrigin(sf::Vector2f(100.f, 112.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(25.f, -40.f));
	setup();
	octo::Application::getConsole().addCommand(L"ori", [this](sf::Vector2f const & p)
	{
		setOrigin(p);
		std::cout << "origin " << p.x << " " << p.y << std::endl;
		});
	octo::Application::getConsole().addCommand(L"size", [this](sf::Vector2f const & p)
	{
		setSize(p);
		std::cout << "size " << p.x << " " << p.y << std::endl;
		});
	octo::Application::getConsole().addCommand(L"texOff", [this](sf::Vector2f const & p)
	{
		setTextOffset(p);
		std::cout << "textOff " << p.x << " " << p.y << std::endl;
		});
}

void SnowGirl1Npc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
