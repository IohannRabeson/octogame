#include "ScientistCedric.hpp"

#include <Application.hpp>
#include <Console.hpp>
ScientistCedric::ScientistCedric(void) :
	ScientistNpc(SCIENTISTCEDRIC_OSS)
{
	setTextOffset(sf::Vector2f(0.f, -90.f));
	setup();
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	octo::Application::getConsole().addCommand(L"ori.ScientistCedric", [this](sf::Vector2f const & p)
	{
		setOrigin(p);
		std::cout << "ScientistCedric : origin " << p.x << " " << p.y << std::endl;
		});
	octo::Application::getConsole().addCommand(L"size.ScientistCedric", [this](sf::Vector2f const & p)
	{
		setSize(p);
		std::cout << "ScientistCedric : size " << p.x << " " << p.y << std::endl;
		});
	octo::Application::getConsole().addCommand(L"texOff.ScientistCedric", [this](sf::Vector2f const & p)
	{
		setTextOffset(p);
		std::cout << "ScientistCedric : textOff " << p.x << " " << p.y << std::endl;
		});
}

void ScientistCedric::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {25u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {26u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {27u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {28u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.2f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {23u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {24u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);
	setupMachine();
}

void ScientistCedric::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					right;
	StatePtr					special1;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	right = std::make_shared<State>("1", getWalkAnimation(), getSprite());
	special1 = std::make_shared<State>("2", getSpecial1Animation(), getSprite());

	machine.setStart(right);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, special1, idle);

	machine.addTransition(Right, right, right);

	machine.addTransition(Special1, right, special1);

	setMachine(machine);
	setNextEvent(Right);
}

void ScientistCedric::updateState(void)
{
	if (getSprite().getCurrentEvent() == Events::Special1 && getSprite().isTerminated())
		ANpc::setNextEvent(Events::Idle);
}
