#ifndef __SETTING_H__
#define __SETTING_H__

#include <d2d1.h>

enum class ePacketType
{
	None,
	C_EnterLobby,
	S_EnterLobby,
	C_Exit,
	S_Exit,
	C_OKLogin,
	S_OKLogin,
	S_FailedLogin,
	C_CreateRoom,
	S_CreateRoom,
	S_NotifyCreateRoom,
	S_SendSessions,
	S_SendRooms,
	S_EnterOtherUser,
	C_Chat,
	S_Chat,
	C_JoinRoom,
	S_JoinRoom,
	S_JoinRoomFail,
	C_LeaveRoom,
	S_LeaveRoom,
	S_UpdateRoomMemberLeave,
	S_UpdateLobbyRoomList,
	S_UpdateLobbyRoomMemberCount,
	S_NotifyJoinedUser,
	C_CheckRoomReady,
	S_CheckRoomReadyOK,
	S_CheckRoomReadyFail,
	C_UserRoomReady,
	S_UpdateUserState,
	S_UpdateWaitingRoomBtn,
	S_UpdateUserType,
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