#include "noahLib.h"
#include "opcodeTables.h"
#include "opcodes.h"

std::array<tOpcode, 256> fieldScriptOpcodes;
std::array<tOpcode, 256> fieldScriptOpcodes_EX;

void initOpcodes()
{
    fieldScriptOpcodes[0x00] = OP_STOP;
    fieldScriptOpcodes[0x01] = OP_JUMP;
    fieldScriptOpcodes[0x02] = OP_JUMP_IF;
    fieldScriptOpcodes[0x04] = OP_4;
    fieldScriptOpcodes[0x05] = OP_CALL;
    fieldScriptOpcodes[0x07] = OP_RUN_ENTITY_SCRIPT_ASYNC;
    fieldScriptOpcodes[0x08] = OP_RUN_ENTITY_SCRIPT_UNKMODE;
    fieldScriptOpcodes[0x09] = OP_RUN_ENTITY_SCRIPT_BLOCKING;
    fieldScriptOpcodes[0x0A] = OP_CALL_IF_IN_TRIGGER;
    fieldScriptOpcodes[0x0B] = OP_INIT_ENTITY_NPC;
    fieldScriptOpcodes[0x0C] = OP_UPDATE_CHARACTER_INFINITLY;
    fieldScriptOpcodes[0x0D] = OP_RETURN;
    fieldScriptOpcodes[0x10] = OP_10;
    fieldScriptOpcodes[0x16] = OP_INIT_ENTITY_PC;
    fieldScriptOpcodes[0x15] = OP_15;
    fieldScriptOpcodes[0x17] = OP_17;
    fieldScriptOpcodes[0x18] = OP_SET_BOUNDING_VOLUME;
    fieldScriptOpcodes[0x19] = OP_SET_ACTOR_POSITION_2D;
    fieldScriptOpcodes[0x1A] = OP_SET_ENTITY_WALKMESH_LAYER;
    fieldScriptOpcodes[0x1C] = OP_SET_CURRENT_ACTOR_ELEVATION;
    fieldScriptOpcodes[0x1D] = OP_SET_CURRENT_ACTOR_POSITION_3D;
    fieldScriptOpcodes[0x1E] = OP_UPDATE_ELEVATION;
    fieldScriptOpcodes[0x1F] = OP_1F;
    fieldScriptOpcodes[0x20] = OP_SET_CURRENT_ACTOR_FLAGS;
    fieldScriptOpcodes[0x21] = OP_21;
    fieldScriptOpcodes[0x22] = OP_22;
    fieldScriptOpcodes[0x23] = OP_23;
    fieldScriptOpcodes[0x24] = OP_24;
    fieldScriptOpcodes[0x25] = OP_25;
    fieldScriptOpcodes[0x26] = OP_WAIT;
    fieldScriptOpcodes[0x27] = OP_RESET_CHARACTER;
    fieldScriptOpcodes[0x28] = OP_28;
    fieldScriptOpcodes[0x29] = OP_DELETE_ENTITY;
    fieldScriptOpcodes[0x2A] = OP_2A;
    fieldScriptOpcodes[0x2C] = OP_PLAY_ANIMATION;
    fieldScriptOpcodes[0x2D] = OP_GET_ACTOR_POSITION;
    fieldScriptOpcodes[0x2E] = OP_GET_ACTOR_FACING_ANGLE;
    fieldScriptOpcodes[0x31] = OP_JUMP_IF_PAD_MASK;
    fieldScriptOpcodes[0x35] = OP_SET_VAR_FROM_S16;
    fieldScriptOpcodes[0x36] = OP_SET_VAR_TRUE;
    fieldScriptOpcodes[0x37] = OP_SET_VAR_FALSE;
    fieldScriptOpcodes[0x38] = OP_SET_VAR_ADD;
    fieldScriptOpcodes[0x39] = OP_SET_VAR_SUBSTRACT;
    fieldScriptOpcodes[0x3A] = OP_SET_VARBIT_TRUE;
    fieldScriptOpcodes[0x3B] = OP_SET_VARBIT_FALSE;
    fieldScriptOpcodes[0x3C] = OP_SET_VAR_INC;
    fieldScriptOpcodes[0x3D] = OP_SET_VAR_DEC;
    fieldScriptOpcodes[0x43] = OP_SET_VAR_RANDOM;
    fieldScriptOpcodes[0x44] = OP_44;
    fieldScriptOpcodes[0x46] = OP_46;
    fieldScriptOpcodes[0x47] = OP_WALK_AND_CHANGE_FIELD;
    fieldScriptOpcodes[0x4A] = OP_SPRITE_WALK_TO_POSITION_AND_WAIT;
    fieldScriptOpcodes[0x4B] = OP_4B;
    fieldScriptOpcodes[0x4C] = OP_4C;
    fieldScriptOpcodes[0x52] = OP_52;
    fieldScriptOpcodes[0x53] = OP_53;
    fieldScriptOpcodes[0x57] = OP_57;
    fieldScriptOpcodes[0x58] = OP_58;
    fieldScriptOpcodes[0x59] = OP_WALK_RANDOM_DIRECTION;
    fieldScriptOpcodes[0x5A] = OP_5A;
    fieldScriptOpcodes[0x5B] = resetFieldScriptActor;
    fieldScriptOpcodes[0x5C] = OP_5C;
    fieldScriptOpcodes[0x5D] = OP_PLAY_ANIMATION_EX;
    fieldScriptOpcodes[0x5E] = OP_WAIT_ANIMATION_END;
    fieldScriptOpcodes[0x5F] = OP_SET_CURRENT_ACTOR_CARDINAL_DIRECTION;
    fieldScriptOpcodes[0x60] = OP_60;
    fieldScriptOpcodes[0x61] = OP_SET_CAMERA_TARGET_OVERRIDE;
    fieldScriptOpcodes[0x62] = OP_SET_DESIRED_CAMERA_TARGET_TO_ENTITY;
    fieldScriptOpcodes[0x63] = OP_SET_DESIRED_CAMERA_TARGET;
    fieldScriptOpcodes[0x64] = OP_64;
    fieldScriptOpcodes[0x65] = OP_SET_CAMERA_POSITION_OVERRIDE;
    fieldScriptOpcodes[0x67] = OP_67;
    fieldScriptOpcodes[0x69] = OP_SET_CURRENT_ACTOR_ROTATION;
    fieldScriptOpcodes[0x6A] = OP_SET_ACTOR_ROTATION2;
    fieldScriptOpcodes[0x6B] = OP_ROTATE_ACTOR_CLOCKWISE;
    fieldScriptOpcodes[0x6C] = OP_ROTATE_ACTOR_ANTICLOCKWISE;
    fieldScriptOpcodes[0x6D] = OP_GET_ANGLE_COS;
    fieldScriptOpcodes[0x6E] = OP_GET_ANGLE_SIN;
    fieldScriptOpcodes[0x6F] = OP_ROTATE_TO_ACTOR;
    fieldScriptOpcodes[0x70] = OP_ROTATE_AWAY_FROM_ACTOR;
    fieldScriptOpcodes[0x72] = OP_72;
    fieldScriptOpcodes[0x74] = OP_PLAY_SOUND_EFFECT;
    fieldScriptOpcodes[0x75] = OP_PLAY_MUSIC;
    fieldScriptOpcodes[0x76] = OP_76;
    fieldScriptOpcodes[0x79] = OP_RESTORE_HP;
    fieldScriptOpcodes[0x7A] = OP_RESTORE_MP;
    fieldScriptOpcodes[0x84] = OP_IF_GAMEPROGRESS_LESS;
    fieldScriptOpcodes[0x85] = OP_IF_GAMEPROGRESS_GREATER;
    fieldScriptOpcodes[0x86] = OP_IF_GAMEPROGRESS_EQUAL;
    fieldScriptOpcodes[0x87] = OP_SET_GAMEPROGRESS;
    fieldScriptOpcodes[0x92] = OP_92;
    fieldScriptOpcodes[0x93] = OP_ADD_ENDITY_TO_MECHA_LIST;
    fieldScriptOpcodes[0x98] = OP_CHANGE_FIELD_WHEN_READY2;
    fieldScriptOpcodes[0x99] = OP_99;
    fieldScriptOpcodes[0x9A] = OP_9A;
    fieldScriptOpcodes[0x9B] = OP_SET_CAMERA_INTERPOLATION_RATE;
    fieldScriptOpcodes[0x9C] = OP_WAIT_DIALOG;
    fieldScriptOpcodes[0x9D] = OP_9D;
    fieldScriptOpcodes[0xA0] = OP_SET_SCREEN_GEOMETRY;
    fieldScriptOpcodes[0xA2] = OP_A2;
    fieldScriptOpcodes[0xA3] = OP_SET_DESIRED_CAMERA_POSITION;
    fieldScriptOpcodes[0xA4] = OP_A4;
    fieldScriptOpcodes[0xA7] = OP_UPDATE_CHARACTER;
    fieldScriptOpcodes[0xA9] = OP_SETUP_MULTICHOICE;
    fieldScriptOpcodes[0xA8] = OP_A8;
    fieldScriptOpcodes[0xAA] = OP_ROTATE_CAMERA_RELATIVE;
    fieldScriptOpcodes[0xAC] = OP_AC;
    fieldScriptOpcodes[0xB3] = OP_START_FADE_IN;
    fieldScriptOpcodes[0xB4] = OP_START_FADE_TO_BLACK;
    fieldScriptOpcodes[0xB5] = OP_B5;
    fieldScriptOpcodes[0xB6] = OP_B6;
    fieldScriptOpcodes[0xB7] = OP_B7;
    fieldScriptOpcodes[0xB8] = OP_B8;
    fieldScriptOpcodes[0xBA] = OP_ADD_FROM_AVAILABLE_PARTY;
    fieldScriptOpcodes[0xBB] = OP_REMOVE_FROM_AVAILABLE_PARTY;
    fieldScriptOpcodes[0xBC] = OP_INIT_ENTITY_SCRIPT;
    fieldScriptOpcodes[0xBE] = OP_ROTATION_3D_YAW_SUBSTRACT;
    fieldScriptOpcodes[0xBF] = OP_ROTATION_3D_PITCH_ADD;
    fieldScriptOpcodes[0xC0] = OP_ROTATION_3D_PITCH_SUBSTRACT;
    fieldScriptOpcodes[0xC1] = OP_ROTATION_3D_ROLL_ADD;
    fieldScriptOpcodes[0xC2] = OP_ROTATION_3D_ROLL_SUBSTRACT;
    fieldScriptOpcodes[0xC4] = OP_OPEN_DOOR;
    fieldScriptOpcodes[0xC5] = OP_CLOSE_DOOR;
    fieldScriptOpcodes[0xC6] = OP_INCREASE_FIELD_EXECUTION_MAX_CYCLES;
    fieldScriptOpcodes[0xC8] = OP_C8;
    fieldScriptOpcodes[0xC9] = OP_IF_PLAYER_IN_TRIGGER2;
    fieldScriptOpcodes[0xCB] = OP_IF_PLAYER_IN_TRIGGER;
    fieldScriptOpcodes[0xCD] = OP_CD;
    fieldScriptOpcodes[0xCE] = OP_CE;
    fieldScriptOpcodes[0xD0] = OP_SET_DIALOG_WINDOW_PARAM;
    fieldScriptOpcodes[0xD2] = OP_SHOW_DIALOG_WINDOW_MODE0;
    fieldScriptOpcodes[0xD4] = OP_SHOW_DIALOG_WINDOW_FOR_OTHER_ACTOR_MODE0;
    fieldScriptOpcodes[0xD6] = OP_D6;
    fieldScriptOpcodes[0xD7] = OP_SET_OBJECT_SWIVEL_ON_X_AXIS;
    fieldScriptOpcodes[0xD9] = OP_SET_OBJECT_SWIVEL_ON_Z_AXIS;
    fieldScriptOpcodes[0xDB] = OP_DB;
    fieldScriptOpcodes[0xDC] = OP_DC;
    fieldScriptOpcodes[0xDE] = OP_DE;
    fieldScriptOpcodes[0xDF] = OP_DF;
    fieldScriptOpcodes[0xE1] = OP_VRAM_OPERATION;
    fieldScriptOpcodes[0xE5] = OP_SETUP_FOG;
    fieldScriptOpcodes[0xE6] = OP_E6;
    fieldScriptOpcodes[0xE7] = OP_SET_BACKGROUND_CLEAR_COLOR;
    fieldScriptOpcodes[0xEC] = ORBIT;
    fieldScriptOpcodes[0xEE] = OP_EE;
    fieldScriptOpcodes[0xEF] = OP_WAIT_CAMERA;
    fieldScriptOpcodes[0xF1] = OP_SETUP_SCREEN_EFFECT1;
    fieldScriptOpcodes[0xF2] = OP_SETUP_CAMERA_SHAKE;
    fieldScriptOpcodes[0xF3] = OP_F3;
    fieldScriptOpcodes[0xF4] = OP_CLOSE_CURRENT_ACTOR_DIALOG;
    fieldScriptOpcodes[0xF5] = OP_SHOW_DIALOG_WINDOW_FOR_CURRENT_ACTOR_MODE3;
    fieldScriptOpcodes[0xF6] = OP_F6;
    fieldScriptOpcodes[0xF8] = OP_SET_FLAGS_DYNAMIC;
    fieldScriptOpcodes[0xF9] = OP_F9;
    fieldScriptOpcodes[0xFB] = OP_FB;
    fieldScriptOpcodes[0xFC] = OP_FC;
    fieldScriptOpcodes[0xFE] = OP_EXTENDED_OPCODE;
    fieldScriptOpcodes[0xFF] = OP_FF;

    fieldScriptOpcodes_EX[0x01] = OP_RAND_ROTATION;
    fieldScriptOpcodes_EX[0x03] = OP_SET_CURRENT_ACTOR_SCALE;
    fieldScriptOpcodes_EX[0x04] = OPX_04;
    fieldScriptOpcodes_EX[0x07] = OPX_07;
    fieldScriptOpcodes_EX[0x09] = OPX_09;
    fieldScriptOpcodes_EX[0x0A] = OPX_0A;
    fieldScriptOpcodes_EX[0x0C] = OPX_0C;
    fieldScriptOpcodes_EX[0x0D] = OP_SET_DIALOG_PORTRAIT;
    fieldScriptOpcodes_EX[0x0E] = OP_SET_MUSIC_PARAMS;
    fieldScriptOpcodes_EX[0x10] = OPX_10;
    fieldScriptOpcodes_EX[0x13] = OPX_13;
    fieldScriptOpcodes_EX[0x15] = OPX_15;
    fieldScriptOpcodes_EX[0x17] = OPX_17;
    fieldScriptOpcodes_EX[0x18] = OP_ADD_TO_CURRENT_PARTY;
    fieldScriptOpcodes_EX[0x19] = OP_REMOVE_FROM_CURRENT_PARTY;
    fieldScriptOpcodes_EX[0x1A] = OP_FINALIZE_PARTY_CHARACTER_LOADING;
    fieldScriptOpcodes_EX[0x1C] = OPX_1C;
    fieldScriptOpcodes_EX[0x1E] = OPX_1E;
    fieldScriptOpcodes_EX[0x21] = OPX_21;
    fieldScriptOpcodes_EX[0x26] = OP_SETUP_SCREEN_DISTORTION;
    fieldScriptOpcodes_EX[0x27] = OP_SCREEN_DISTORTION_FADE_OUT;
    fieldScriptOpcodes_EX[0x3A] = OP_SET_PARTY_FRAME_MASK;
    fieldScriptOpcodes_EX[0x3B] = OP_CLEAR_PARTY_FRAME_MASK;
    fieldScriptOpcodes_EX[0x3C] = OPX_3C;
    fieldScriptOpcodes_EX[0x3D] = OPX_3D;
    fieldScriptOpcodes_EX[0x3E] = OPX_3E;
    fieldScriptOpcodes_EX[0x3F] = OPX_3F;
    fieldScriptOpcodes_EX[0x40] = OPX_40;
    fieldScriptOpcodes_EX[0x41] = OP_SET_ON_GEAR;
    fieldScriptOpcodes_EX[0x42] = OP_SET_OFF_GEAR;
    fieldScriptOpcodes_EX[0x45] = OPX_45;
    fieldScriptOpcodes_EX[0x47] = OPX_47;
    fieldScriptOpcodes_EX[0x4A] = OP_LOAD_SPECIAL_2D_ANIMATION;
    fieldScriptOpcodes_EX[0x4B] = OP_WAIT_LOADING_SPECIAL_2D_ANIMATION;
    fieldScriptOpcodes_EX[0x4D] = OPX_4D;
    fieldScriptOpcodes_EX[0x4E] = OP_FREE_SPECIAL_2D_ANIMATION;
    fieldScriptOpcodes_EX[0x50] = OPX_50;
    fieldScriptOpcodes_EX[0x52] = OP_DISABLE_COMPASS;
    fieldScriptOpcodes_EX[0x53] = OP_ENABLE_PLAYER_CONTROLS;
    fieldScriptOpcodes_EX[0x54] = OPX_54;
    fieldScriptOpcodes_EX[0x5C] = OPX_5C;
    fieldScriptOpcodes_EX[0x5D] = OPX_5D;
    fieldScriptOpcodes_EX[0x5E] = OP_SET_TRANSPARENCY_MODE;
    fieldScriptOpcodes_EX[0x62] = OPX_62;
    fieldScriptOpcodes_EX[0x65] = OPX_65;
    fieldScriptOpcodes_EX[0x66] = OPX_66;
    fieldScriptOpcodes_EX[0x6B] = OPX_6B;
    fieldScriptOpcodes_EX[0x7F] = OPX_7F;
    fieldScriptOpcodes_EX[0x80] = OPX_80;
    fieldScriptOpcodes_EX[0x81] = OPX_81;
    fieldScriptOpcodes_EX[0x82] = OPX_82;
    fieldScriptOpcodes_EX[0x84] = OPX_84;
    fieldScriptOpcodes_EX[0x85] = OPX_85;
    fieldScriptOpcodes_EX[0x86] = OPX_86;
    fieldScriptOpcodes_EX[0x8C] = OPX_8C;
    fieldScriptOpcodes_EX[0x8E] = OPX_8E;
    fieldScriptOpcodes_EX[0x8F] = OPX_8F;
    fieldScriptOpcodes_EX[0x90] = OPX_90;
    fieldScriptOpcodes_EX[0x91] = OPX_91;
    fieldScriptOpcodes_EX[0x92] = OPX_92;
    fieldScriptOpcodes_EX[0x93] = OPX_93;
    fieldScriptOpcodes_EX[0x94] = OPX_94;
    fieldScriptOpcodes_EX[0x95] = OPX_95;
    fieldScriptOpcodes_EX[0x96] = OP_CREATE_PARTICLE_EFFECT_FOR_CURRENT_ACTOR;
    fieldScriptOpcodes_EX[0x97] = OPX_97;
    fieldScriptOpcodes_EX[0x98] = OPX_98;
    fieldScriptOpcodes_EX[0x9A] = OPX_9A;
    fieldScriptOpcodes_EX[0x9F] = OP_TOGGLE_FRAME_LOCK;
    fieldScriptOpcodes_EX[0xA1] = OP_ASSIGN_GEAR;
    fieldScriptOpcodes_EX[0xA2] = OPX_A2;
    fieldScriptOpcodes_EX[0xA4] = OP_RESTORE_GEAR;
    fieldScriptOpcodes_EX[0xA5] = OPX_A5;
    fieldScriptOpcodes_EX[0xAF] = OPX_AF;
    fieldScriptOpcodes_EX[0xB8] = OPX_B8;
    fieldScriptOpcodes_EX[0xBD] = OPX_BD;
    fieldScriptOpcodes_EX[0xC8] = OPX_C8;
    fieldScriptOpcodes_EX[0xCB] = OPX_CB;
    fieldScriptOpcodes_EX[0xCC] = OPX_CC;
    fieldScriptOpcodes_EX[0xCD] = OP_GET_CURRENT_DISC_NUMBER;
    fieldScriptOpcodes_EX[0xE0] = OPX_E0;
}

class fakeConstructor
{
public:
    fakeConstructor()
    {
        initOpcodes();
    }
} gFakeConstructor;

