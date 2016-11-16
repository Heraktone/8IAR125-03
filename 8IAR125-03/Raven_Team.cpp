#include "Raven_Team.h"



Raven_Team::Raven_Team(int color) : m_Color(color)
{
}

void Raven_Team::Update() {

}

void Raven_Team::AddMember(Raven_Bot* bot) {
	m_Bots.push_back(bot);
	bot->SetTeam(this);
}

void Raven_Team::RemoveMember(Raven_Bot* bot) {
	m_Bots.remove(bot);
	bot->SetTeam(0);
}