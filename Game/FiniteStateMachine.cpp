/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FiniteStateMachine.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 14:51:35 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/03 19:51:48 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FiniteStateMachine.hpp"
#include <cassert>
#include <stdexcept>

FiniteStateMachine::FiniteStateMachine() :
	m_isRunning(false)
{
}

FiniteStateMachine::StatePtr const&	FiniteStateMachine::getCurrentState()const
{
	return (m_current);
}

FiniteStateMachine::StatePtr const& FiniteStateMachine::getStartState()const
{
	return (m_startState);
}

void	FiniteStateMachine::addTransition(EventId eventId, StatePtr source, StatePtr target, TransitionCallback callback)
{
	if (source == nullptr)
		throw std::logic_error("FSM: null source");
	if (target == nullptr)
		throw std::logic_error("FSM: null target");
	m_states.insert(source);
	m_states.insert(target);
	source->addTransition(eventId, target, callback);	
}

void	FiniteStateMachine::setNextEvent(EventId eventId)
{
	m_nextEventId = eventId;
}

void	FiniteStateMachine::update(sf::Time frameTime)
{
	if (m_nextEventId != NullEvent)
	{
		m_current->stop();
		m_current = m_current->getNext(m_nextEventId);
		m_current->start();
		m_nextEventId = NullEvent;
	}
	m_current->update(frameTime);
}

void	FiniteStateMachine::setStart(StatePtr const& state)
{
	m_startState = state;
}

void	FiniteStateMachine::start()
{
	assert (m_startState != nullptr);

	m_isRunning = true;
	m_nextEventId = NullEvent;
	if (m_current)
	{
		m_current->stop();
	}
	m_current = m_startState;
	if (m_current)
	{
		m_current->start();
	}
}

bool	FiniteStateMachine::isReady()const
{
	return (m_startState != nullptr);
}

FiniteStateMachine::AState::AState(std::string const& name) :
	m_name(name)
{
}

FiniteStateMachine::AState::~AState()
{
}

void	FiniteStateMachine::AState::addTransition(EventId eventId, StatePtr target)
{
	m_transitions[eventId] = Transition(eventId, target);
}

void	FiniteStateMachine::AState::addTransition(EventId eventId, StatePtr target, TransitionCallback callback)
{
	m_transitions[eventId] = Transition(eventId, target, callback);
}

FiniteStateMachine::StatePtr	FiniteStateMachine::AState::getNext(EventId eventId)
{
	auto	it = m_transitions.find(eventId);
	StatePtr	nextNode;

	if (it != m_transitions.end())
	{
		nextNode = it->second.getTargetNode();
		it->second.notifyCallback();
	}
	return (nextNode);
}

typename FiniteStateMachine::AState::ConstIterator		FiniteStateMachine::AState::begin()const
{
	return (m_transitions.begin());
}

typename FiniteStateMachine::AState::ConstIterator		FiniteStateMachine::AState::end()const
{
	return (m_transitions.end());
}

std::string const&	FiniteStateMachine::AState::getName()const
{
	return (m_name);
}

void	FiniteStateMachine::exportDot(std::ostream& os, std::string const& name)
{
	os << "digraph " << name << "\n{\n";
	for (auto state : m_states)
	{
		os << reinterpret_cast<std::size_t>(state.get()) << " [label=\"" << state->getName() << "\"];\n";
	}
	for (auto state : m_states)
	{
		for (auto transition : *state)
		{
			os << reinterpret_cast<std::size_t>(state.get()) << " -> " <<
				  reinterpret_cast<std::size_t>(transition.second.getTargetNode().get()) <<
				  " [label=\"" << transition.first << "\"];\n";
		}
	}
	os << "}\n";
}

//
// class FiniteStateMachine::Transition
//
FiniteStateMachine::Transition::Transition(EventId triggerEventId) :
	m_eventId(triggerEventId)
{
}

FiniteStateMachine::Transition::Transition(EventId triggerEventId, StatePtr target, TransitionCallback callback) :
	m_eventId(triggerEventId),
	m_callback(callback),
	m_target(target)
{
}

void	FiniteStateMachine::Transition::notifyCallback()
{
	if (m_callback)
	{
		m_callback();
	}
}

FiniteStateMachine::StatePtr const&	FiniteStateMachine::Transition::getTargetNode()const
{
	return (m_target);
}

FiniteStateMachine::EventId		FiniteStateMachine::Transition::getTriggerEvent()const
{
	return (m_eventId);
}
