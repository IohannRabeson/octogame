#include "OldDesertStaticNpc.hpp"

#include <Application.hpp>
#include <Console.hpp>
OldDesertStaticNpc::OldDesertStaticNpc(void) :
	AIdleNpc(NPC_OLD_MAN_DESERT_STATIC_OSS)
{
	setSize(sf::Vector2f(50.f, 120.f));
	setOrigin(sf::Vector2f(75.f, 44.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
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

void OldDesertStaticNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}
