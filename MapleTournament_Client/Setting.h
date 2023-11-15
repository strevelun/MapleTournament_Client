#ifndef __SETTING_H__
#define __SETTING_H__

#include <d2d1.h>

enum class ePacketType
{
	None,
	S_EnterLobby,
	C_Exit,
	C_OKLogin,
	S_OKLogin,
	S_FailedLogin,
	C_CreateRoom,
	S_CreateRoom,
	C_Chat,
	S_Chat,
	C_JoinRoom,
	S_JoinRoom,
	S_JoinRoomFail,
	C_LeaveRoom,
	S_UpdateRoomMemberLeave,
	S_NotifyJoinedUser,
	C_CheckRoomReady,
	S_CheckRoomReadyOK,
	S_CheckRoomReadyFail,
	C_UserRoomReady,
	S_UpdateUserState,
	S_UpdateWaitingRoomBtn,
	S_UpdateUserType,
	C_InGameReady,
	S_InGameReady,
	C_UpdateUserListPage,
	S_UpdateUserListPage,
	C_UpdateRoomListPage,
	S_UpdateRoomListPage,
	C_UpdateUserSlot,
	S_UpdateUserSlot,
	C_Skill,
	S_Skill,
	S_UpdateTurn,
	S_UpdateDashboard,
	C_NextTurn,
	S_GameOver,
	C_GameOver,
	S_GameOverSceneChange,
	C_LobbyInit,
	S_UpdateIngameUserLeave,
	S_Standby,
	C_Standby,
	S_UpdateProfile,
	C_CheckHit,
	S_CheckHit,
	C_UpdateScore,
	S_UpdateScore,
	C_CheckHeal,
	S_UpdateHeal,
	S_CreatePortal,
	S_Teleport,
	C_ExitInGame,
	S_ExitInGame,
};

enum class eInGameState
{
	None,
	Prepare,
	Play,
	UseSkill,
	GameOver,
};

enum class eSkillState
{
	None,
	InUse,
	CheckHit,
	CheckHeal,
	End,
};

enum class eActionType
{
	None,
	Skill,
	Move,
	Hit,
	Die,
	NumOfActions,
};

enum class eMoveName
{
	None,
	LeftMove,
	LeftDoubleMove,
	RightMove,
	RightDoubleMove,
	UpMove,
	DownMove,
	NumOfMoves,
};

enum class eSkillName
{
	None,
	Heal0,
	Attack0,
	Attack0_Left,
	Attack1,
	Attack2,
	Attack3,
	Attack4,
	NumOfSkills,
};

enum class eMemberType
{
	None,
	Member,
	Owner
};

enum class eMemberState
{
	None,
	Wait,
	Ready
}; 

enum class eType
{
	Tile,
	Block,
	Character,
	None
};

enum class eSessionState
{
	None,
	Login,
	Lobby,
	WaitingRoom,
	InGame
};

inline const wchar_t* sessionStateStr[] =
{
	L"None",
	L"Login",
	L"Lobby",
	L"WaitingRoom",
	L"InGame"
};

typedef struct _tSpriteData
{
	float pivotX = 0.5f, pivotY = 0.5f;
	D2D1_RECT_F rect = {};
	D2D1_SIZE_F size = {};
	DWORD* pixel = nullptr;
	ID2D1Bitmap* m_bitmap = nullptr;
	eType type;
} tSpriteData;

typedef struct _tAnimationFrame
{
	float pivotX = 0.5f, pivotY = 0.5f;
	D2D1_RECT_F rect = {};
	D2D1_SIZE_F size = {};

	_tAnimationFrame(const _tSpriteData& sprite)
		: pivotX(sprite.pivotX), pivotY(sprite.pivotY), rect(sprite.rect), size(sprite.size)
	{}
} tAnimationFrame;

#endif