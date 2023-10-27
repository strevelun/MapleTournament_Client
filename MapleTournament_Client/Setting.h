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
};

enum class eSkillType
{
	None,
	AttackCloud,
	LeftMove,
	LeftDoubleMove,
	RightMove,
	RightDoubleMove,

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
	WatingRoom,
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