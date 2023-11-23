#include "NetworkCore/NetworkManager.h"
#include "Managers/SkillManager.h"
#include "Skill/SkillInfo.h"
#include "Skill/SkillAttack.h"
#include "Game.h"

Game* Game::m_pInst = nullptr;

typedef unsigned short ushort;

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Init()
{
	m_portalPosition = { -1,-1 };
	m_curPlayerSlot = -1;
	m_curRound = 1;
	m_playingcount = 0;
	m_aliveCount = 0;

	for (int i = 0; i < RoomSlotNum; i++)
	{
		int xpos = 0, ypos = 0;
		switch (i)
		{
		case 0:
			xpos = 0;
			ypos = 0;
			break;
		case 1:
			xpos = 4;
			ypos = 0;
			break;
		case 2:
			xpos = 0;
			ypos = 3;
			break;
		case 3:
			xpos = 4;
			ypos = 3;
			break;
		}
		m_arrPlayer[i].Init(i, xpos, ypos);
		m_arrBoard[ypos][xpos].insert({ i, &m_arrPlayer[i] });
	}
	return true;
}

void Game::AddPlayer(int _slot)
{
	m_playingcount++;
	m_aliveCount++;
	m_arrPlayer[_slot].m_bPlaying = true;
	m_arrPlayer[_slot].m_bAlive = true;
}

void Game::AddScore(int _slot, int _score)
{
	m_arrPlayer[_slot].m_score += _score;
}

void Game::DecreaseAliveCount()
{
	m_aliveCount--;
}

void Game::RemovePlayerFromBoard(int _slot)
{
	m_arrBoard[m_arrPlayer[_slot].m_ypos][m_arrPlayer[_slot].m_xpos].erase(m_arrPlayer[_slot].m_slot);
}

void Game::SetMana(int _slot, int _mana)
{
	m_arrPlayer[_slot].m_mana = _mana;
}

void Game::SetSkillName(int _slot, eSkillName _name)
{
	m_arrPlayer[_slot].m_eSkillName = _name;
}

void Game::LeavePlayer(int _slot)
{
	m_playingcount--;
	m_aliveCount--;
	m_arrBoard[m_arrPlayer[_slot].m_ypos][m_arrPlayer[_slot].m_xpos].erase(m_arrPlayer[_slot].m_slot);
	m_arrPlayer[_slot].m_bPlaying = false;
	m_arrPlayer[_slot].m_bAlive = false;
}

void Game::GetHitResult(int _slot, std::vector<PlayerInfo*>& _list, std::vector<PlayerInfo*>& _listDead)
{
	PlayerInfo& player = m_arrPlayer[_slot];
	if (!player.m_bAlive || !player.m_bPlaying) return;

	PlayerInfo* pCounterPlayer = nullptr;

	const SkillInfo* pSkill = SkillManager::GetInst()->GetSkill(_slot, player.m_eSkillName);
	if (pSkill->GetType() != eSkillType::Attack) return;

	const SkillAttack* pSkillAttack = static_cast<const SkillAttack*>(pSkill);

	const std::vector<std::pair<int, int>>& listCoordinates = pSkillAttack->GetListCoordinates();

	std::vector<std::pair<int, int>>::const_iterator iter = listCoordinates.cbegin();
	std::vector<std::pair<int, int>>::const_iterator iterEnd = listCoordinates.cend();

	std::map<int, PlayerInfo*>::iterator boardIter;
	std::map<int, PlayerInfo*>::iterator boardIterEnd;

	int strikePower = pSkillAttack->GetStrikePower();

	for (; iter != iterEnd; ++iter)
	{
		if (player.m_xpos + iter->first < 0) continue;
		if (player.m_xpos + iter->first >= BoardWidth) continue;
		if (player.m_ypos + iter->second < 0) continue;
		if (player.m_ypos + iter->second >= BoardHeight) continue;
		if (m_arrBoard[player.m_ypos + iter->second][player.m_xpos + iter->first].size() == 0) continue;

		boardIter = m_arrBoard[player.m_ypos + iter->second][player.m_xpos + iter->first].begin();
		boardIterEnd = m_arrBoard[player.m_ypos + iter->second][player.m_xpos + iter->first].end();

		for (; boardIter != boardIterEnd; ++boardIter)
		{
			if (boardIter->second == &player) continue;

			pCounterPlayer = boardIter->second;

			if (pCounterPlayer && pCounterPlayer->m_bAlive)
			{
				pCounterPlayer->m_hp -= strikePower;
				if (pCounterPlayer->m_hp <= 0)
				{
					pCounterPlayer->m_bAlive = false;
					pCounterPlayer->m_hp = 0;
					_listDead.push_back(pCounterPlayer);
				}
				else
					_list.push_back(pCounterPlayer);
			}
		}
	}
}

void Game::UpdatePortal()
{
	int portalPositionX, portalPositionY, portalTeleportX, portalTeleportY;

	do {
		portalPositionX = rand() % BoardWidth;
		portalPositionY = rand() % BoardHeight;
		portalTeleportX = rand() % BoardWidth;
		portalTeleportY = rand() % BoardHeight;
	} while (m_arrBoard[portalPositionY][portalPositionX].size() >= 1);

	char buffer[255] = {};
	ushort count = sizeof(ushort);
	*(ushort*)(buffer + count) = (ushort)ePacketType::C_CreatePortal;			count += sizeof(ushort);
	*(char*)(buffer + count) = (char)portalPositionX;			count += sizeof(char);
	*(char*)(buffer + count) = (char)portalPositionY;			count += sizeof(char);
	*(char*)(buffer + count) = (char)portalTeleportX;			count += sizeof(char);
	*(char*)(buffer + count) = (char)portalTeleportY;			count += sizeof(char);
	*(ushort*)buffer = count;
	NetworkManager::GetInst()->Send(buffer);
}

void Game::UpdateSlotPos(int _slot, int _xpos, int _ypos)
{
	m_arrBoard[m_arrPlayer[_slot].m_ypos][m_arrPlayer[_slot].m_xpos].erase(_slot);
	m_arrPlayer[_slot].m_xpos = _xpos;
	m_arrPlayer[_slot].m_ypos = _ypos;
	m_arrBoard[_ypos][_xpos].insert({ _slot, &m_arrPlayer[_slot] });
}

int Game::UpdateNextSlot()
{
	do {
		if (m_curPlayerSlot >= RoomSlotNum - 1)
		{
			m_curPlayerSlot = -1;
			break;
		}
	} while (m_arrPlayer[++m_curPlayerSlot].m_bPlaying == false || m_arrPlayer[m_curPlayerSlot].m_bAlive == false);

	return m_curPlayerSlot;
}

bool Game::CheckNextRound()
{
	if (m_curPlayerSlot == -1)
	{
		m_curRound++;

		return true;
	}
	return false;
}

void Game::OnGameOver()
{
	// 나갈때 그 유저의 m_bPlaying을 false함. 그때 m_arrBoard에서 제거
	for (int i = 0; i < RoomSlotNum; i++)
		if (m_arrPlayer[i].m_bPlaying)
		{
			m_arrBoard[m_arrPlayer[i].m_ypos][m_arrPlayer[i].m_xpos].erase(m_arrPlayer[i].m_slot);
			m_arrPlayer[i].m_bPlaying = false;
		}
}

eMoveName Game::Move(int _slot, eMoveName _name)
{
	PlayerInfo& player = m_arrPlayer[_slot];
	if (!player.m_bAlive || !player.m_bPlaying) return eMoveName::None;

	if (_name == eMoveName::LeftMove)
	{
		if (player.m_xpos - 1 >= 0)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos][player.m_xpos - 1][player.m_slot] = &player;
			player.m_xpos -= 1;
		}
		else
			return eMoveName::None;
	}
	else if (_name == eMoveName::LeftDoubleMove)
	{
		if (player.m_xpos - 2 >= 0)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos][player.m_xpos - 2][player.m_slot] = &player;
			player.m_xpos -= 2;
		}
		else if (player.m_xpos - 1 >= 0)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos][player.m_xpos - 1][player.m_slot] = &player;
			player.m_xpos -= 1;
			_name = eMoveName::LeftMove;
		}
		else
			return eMoveName::None;
	}
	else if (_name == eMoveName::RightMove)
	{
		if (player.m_xpos + 1 < BoardWidth)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos][player.m_xpos + 1][player.m_slot] = &player;
			player.m_xpos += 1;
		}
		else
			return eMoveName::None;
	}
	else if (_name == eMoveName::RightDoubleMove)
	{
		if (player.m_xpos + 2 < BoardWidth)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos][player.m_xpos + 2][player.m_slot] = &player;
			player.m_xpos += 2;
		}
		else if (player.m_xpos + 1 < BoardWidth)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos][player.m_xpos + 1][player.m_slot] = &player;
			player.m_xpos += 1;
			_name = eMoveName::RightMove;
		}
		else
			return eMoveName::None;
	}
	else if (_name == eMoveName::UpMove)
	{
		if (player.m_ypos - 1 >= 0)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos - 1][player.m_xpos][player.m_slot] = &player;
			player.m_ypos -= 1;
		}
		else
			return eMoveName::None;
	}
	else if (_name == eMoveName::DownMove)
	{
		if (player.m_ypos + 1 < BoardHeight)
		{
			m_arrBoard[player.m_ypos][player.m_xpos].erase(player.m_slot);
			m_arrBoard[player.m_ypos + 1][player.m_xpos][player.m_slot] = &player;
			player.m_ypos += 1;
		}
		else
			return eMoveName::None;
	}
	return _name;
}

void Game::CheckPortal(u_int _slot)
{
	if (_slot >= RoomSlotNum) return;

	if (m_arrPlayer[_slot].m_xpos == m_portalPosition.first && m_arrPlayer[_slot].m_ypos == m_portalPosition.second)
	{
		// 플레이어 랜덤 이동시키고 한번 더 턴을 줌. 그리고 포탈은 비활성화
		// 서버에서 포탈 비활성화는 -1
		u_int newXPos;
		u_int newYPos;
		do {
			newXPos = rand() % BoardWidth;
			newYPos = rand() % BoardHeight;
		} while (m_arrPlayer[_slot].m_xpos == newXPos && m_arrPlayer[_slot].m_ypos == newYPos);

		RemovePlayerFromBoard(_slot);

		m_arrPlayer[_slot].m_xpos = newXPos;
		m_arrPlayer[_slot].m_ypos = newYPos;

		m_arrBoard[newYPos][newXPos][_slot] = &m_arrPlayer[_slot];

		m_arrPlayer[_slot].m_bWaitForPortal = true;

		m_portalPosition.first = -1;
		m_portalPosition.second = -1;
	}
}

void Game::CreatePortal(int _xpos, int _ypos, int _teleportX, int _teleportY)
{
	m_portalPosition.first = _xpos;
	m_portalPosition.second = _ypos;
	m_portalTeleportPosition.first = _teleportX;
	m_portalTeleportPosition.second = _teleportY;
}

void Game::ResetPlayerSkillName()
{
	for (PlayerInfo& info : m_arrPlayer)
	{
		if (!info.m_bAlive) continue;
		info.m_eSkillName = eSkillName::None;
	}
}

void PlayerInfo::Init(int _slot, int _xpos, int _ypos)
{
	m_bPlaying = false;
	m_score = 0; 
	m_slot = _slot;
	m_hp = Game::HPMax;
	m_mana = Game::MPMax;
	m_xpos = _xpos, m_ypos = _ypos;
	m_bAlive = false;
	m_bWaitForPortal = false;
	m_eSkillName = eSkillName::None;
}
