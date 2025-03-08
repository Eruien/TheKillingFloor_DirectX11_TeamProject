#include "LFiniteState.h"

void LFiniteState::AddTransition(Event inputEvent, State outputState)
{
	m_StateMap.insert(std::make_pair(inputEvent, outputState));
}

State LFiniteState::Output(Event inputEvent)
{
	auto iter = m_StateMap.find(inputEvent);
	State state = iter->second;
	return state;
}

LFiniteState::LFiniteState(State state) : m_State(state) {}