#include "offsets.hpp"

OFFSETS::OFFSETS() {};

// BOX
int OFFSETS::OFFSET_MATRIX = 0x001B3BD0; // ViewMatrix
int OFFSETS::OFFSET_RENDER = 0x408B768; // ViewRender
int OFFSETS::OFFSET_ENTITYLIST = 0x18DA3F8; // cl_entitylist
int OFFSETS::OFFSET_ORIGIN = 0x14C; // m_vecAbsOrigin


// ENTITY
int OFFSETS::OFFSET_LOCAL_ENT = 0x1C898F8; // LocalPlayer

int OFFSETS::OFFSET_TEAM = 0x0450; // m_iTeamNum
int OFFSETS::OFFSET_HEALTH = 0x0440; // m_iHealth
int OFFSETS::OFFSET_SHEILD = 0x0170; // m_shieldHealth

int OFFSETS::OFFSET_LIFE_STATE = 0x0798; // m_lifeState
int OFFSETS::OFFSET_BLEED_OUT_STATE = 0x25E8; // m_bleedoutState
int OFFSETS::OFFSET_VISIBLE_TIME = 0x1A4C; // m_visibletime


// GLOW
int OFFSETS::GLOW_CONTEXT = 0x3C8; // Script_Highlight_SetCurrentContext
int OFFSETS::GLOW_VISIBLE_TYPE = 0x3D0; // Script_Highlight_SetVisibilityType - 5th mov
int OFFSETS::GLOW_TYPE = 0x2C4; // Script_Highlight_GetState + 4
int OFFSETS::GLOW_COLOR = 0x1D0; // Script_CopyHighlightState 15th mov
