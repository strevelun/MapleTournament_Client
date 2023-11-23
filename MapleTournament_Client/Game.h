#pragma once

#include "Defines.h"
#include "Setting.h"

#include <array>
#include <map>
#include <vector>

struct PlayerInfo
{
	bool m_bAlive = false;
	int m_score = 0; // Å³ ¼ö
	int m_slot = 0;
	int m_hp = 0;
	int m_mana = 0;
	int m_xpos = 0, m_ypos = 0;
	eSkillName m_eSkillName = eSkillName::None;

	void Init(int _slot, int _xpos, int _ypos);
};

class Game
{
public:
	static constexpr int RoomSlotNum = 4;
	static constexpr int BoardWidth = 5;
	static constexpr int BoardHeight = 4;

	static constexpr int HPMax = 20;
	static constexpr int MPMax = 10;

	static constexpr int GameMaxRound = 30;

private:
	std::array<std::array<std::map<int, PlayerInfo*>, BoardWidth>, BoardHeight> m_arrBoard;
	std::array<PlayerInfo, RoomSlotNum> m_arrPlayer;

	std::pair<int, int> m_portalPosition;
	std::pair<int, int> m_portalTeleportPosition;

	int m_curPlayerSlot = -1;
	unsigned int m_curRound = 1;
	int m_aliveCount = 0;

	int m_mySlot = 0;
	int m_owner = 0;

public:
	bool Init();

	void AddPlayer(int _slot);
	void AddScore(int _slot, int _score);

	void DecreaseAliveCount();

	void RemovePlayerFromBoard(int _slot);

	void SetMySlot(int _slot) { m_mySlot = _slot; }
	void SetMana(int _slot, int _mana);
	void SetSkillName(int _slot, eSkillName _name);
	void SetOwner(int _slot) { m_owner = _slot; }

	void LeavePlayer(int _slot);


	bool IsMyTurn() const { return m_curPlayerSlot == m_mySlot; }
	bool IsSlotAlive(int _slot) const { return m_arrPlayer[_slot].m_bAlive; }
	bool AmIOwner() const { return m_owner == m_mySlot; }

	int GetAliveCount() const { return m_aliveCount; }
	int GetCurSlot() const { return m_curPlayerSlot; }
	unsigned int GetCurRound() const { return m_curRound; }
	int GetMana(int _slot) const { return m_arrPlayer[_slot].m_mana; }
	eSkillName GetSlotSkillName(int _slot) { return m_arrPlayer[_slot].m_eSkillName; }
	int GetMyScore() const { return m_arrPlayer[m_mySlot].m_score; }
	int GetMyXPos() const { return m_arrPlayer[m_mySlot].m_xpos; }
	int GetMyYPos() const { return m_arrPlayer[m_mySlot].m_ypos; }
	int GetSlotXPos(int _slot) const { return m_arrPlayer[_slot].m_xpos; }
	int GetSlotYPos(int _slot) const { return m_arrPlayer[_slot].m_ypos; }
	void GetHitResult(int _slot, std::vector<PlayerInfo*>& _list, std::vector<PlayerInfo*>& _listDead);
	int GetMySlot() const { return m_mySlot; }
	int GetPortalTeleportPosX() const { return m_portalTeleportPosition.first; }
	int GetPortalTeleportPosY() const { return m_portalTeleportPosition.second; }

	void UpdatePortal();
	void UpdateSlotPos(int _slot, int _xpos, int _ypos);
	int UpdateNextSlot();

	bool CheckNextRound();
	void OnGameOver();

	eMoveName Move(int _slot, eMoveName _name);
	bool CheckPlayerOnPortal(u_int _slot);

	void CreatePortal(int _xpos, int _ypos, int _teleportX, int _teleportY);

	void ResetPlayerSkillName();

	SINGLETON(Game)
};

