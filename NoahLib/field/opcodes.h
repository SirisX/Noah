#pragma once

void OP_STOP();
void OP_JUMP();
void OP_JUMP_IF();
void OP_4();
void OP_CALL();
void OP_RUN_ENTITY_SCRIPT_ASYNC();
void OP_RUN_ENTITY_SCRIPT_UNKMODE();
void OP_RUN_ENTITY_SCRIPT_BLOCKING();
void OP_10();
void OP_CALL_IF_IN_TRIGGER();
void OP_INIT_ENTITY_NPC();
void OP_UPDATE_CHARACTER_INFINITLY();
void OP_RETURN();
void OP_10();
void OP_CUSTOM_FIELD_TRANSITION();
void OP_DISABLE_RANDOM_BATTLES();
void OP_INIT_ENTITY_PC();
void OP_15();
void OP_17();
void OP_SET_BOUNDING_VOLUME();
void OP_SET_ACTOR_POSITION_2D();
void OP_SET_ENTITY_WALKMESH_LAYER();
void OP_SET_CURRENT_ACTOR_ELEVATION();
void OP_SET_CURRENT_ACTOR_POSITION_3D();
void OP_UPDATE_ELEVATION();
void OP_1F();
void OP_SET_CURRENT_ACTOR_FLAGS();
void OP_21();
void OP_22();
void OP_23();
void OP_24();
void OP_25();
void OP_WAIT();
void OP_RESET_CHARACTER();
void OP_28();
void OP_DELETE_ENTITY();
void OP_2A();
void OP_PLAY_ANIMATION();
void OP_GET_ACTOR_POSITION();
void OP_GET_ACTOR_FACING_ANGLE();
void OP_JUMP_IF_PAD_MASK();
void OP_SET_VAR_FROM_S16();
void OP_SET_VAR_TRUE();
void OP_SET_VAR_FALSE();
void OP_SET_VAR_ADD();
void OP_SET_VAR_SUBSTRACT();
void OP_SET_VARBIT_TRUE();
void OP_SET_VARBIT_FALSE();
void OP_SET_VAR_INC();
void OP_SET_VAR_DEC();
void OP_SET_VAR_RANDOM();
void OP_44();
void OP_46();
void OP_WALK_AND_CHANGE_FIELD();
void OP_SPRITE_WALK_TO_POSITION_AND_WAIT();
void OP_4B();
void OP_4C();
void OP_52();
void OP_53();
void OP_55();
void OP_WORLDMAP();
void OP_57(); 
void OP_SET_ANGLE_AXIS_ROTATION();
void OP_WALK_RANDOM_DIRECTION();
void OP_5A();
void resetFieldScriptActor();
void OP_5C();
void OP_PLAY_ANIMATION_EX();
void OP_WAIT_ANIMATION_END();
void OP_SET_CURRENT_ACTOR_CARDINAL_DIRECTION();
void OP_60();
void OP_SET_CAMERA_TARGET_OVERRIDE();
void OP_SET_DESIRED_CAMERA_TARGET_TO_ENTITY();
void OP_SET_DESIRED_CAMERA_TARGET();
void OP_64();
void OP_SET_CAMERA_POSITION_OVERRIDE();
void OP_67();
void OP_SET_CURRENT_ACTOR_ROTATION();
void OP_SET_ACTOR_ROTATION2();
void OP_ROTATE_ACTOR_CLOCKWISE();
void OP_ROTATE_ACTOR_ANTICLOCKWISE();
void OP_GET_ANGLE_COS();
void OP_GET_ANGLE_SIN();
void OP_ROTATE_TO_ACTOR();
void OP_ROTATE_AWAY_FROM_ACTOR();
void OP_72();
void OP_PLAY_SOUND_EFFECT();
void OP_PLAY_MUSIC();
void OP_76();
void OP_RESTORE_HP();
void OP_RESTORE_MP();
void OP_IF_GAMEPROGRESS_LESS();
void OP_IF_GAMEPROGRESS_GREATER();
void OP_IF_GAMEPROGRESS_EQUAL();
void OP_SET_GAMEPROGRESS();
void OP_IF_CHARACTER_IN_PARTY();
void OP_92();
void OP_ADD_ENDITY_TO_MECHA_LIST();
void OP_CHANGE_FIELD_WHEN_READY2();
void OP_99();
void OP_9A();
void OP_SET_CAMERA_INTERPOLATION_RATE();
void OP_WAIT_DIALOG();
void OP_9D();
void OP_SET_SCREEN_GEOMETRY();
void OP_SET_DOLLY_STOP();
void OP_A2();
void OP_SET_DESIRED_CAMERA_POSITION();
void OP_A4();
void OP_UPDATE_CHARACTER();
void OP_SETUP_MULTICHOICE();
void OP_A8();
void OP_ROTATE_CAMERA_RELATIVE();
void OP_SETUP_CAMERA_INTERPOLATION_STEPS();
void OP_START_FADE_IN();
void OP_START_FADE_TO_BLACK();
void OP_B5();
void OP_B6();
void OP_B7();
void OP_B8();
void OP_IF_CHARACTER_IN_AVAILABLE_PARTY();
void OP_ADD_FROM_AVAILABLE_PARTY();
void OP_REMOVE_FROM_AVAILABLE_PARTY();
void OP_INIT_ENTITY_SCRIPT();
void OP_ROTATION_3D_YAW_SUBSTRACT();
void OP_ROTATION_3D_PITCH_ADD();
void OP_ROTATION_3D_PITCH_SUBSTRACT();
void OP_ROTATION_3D_ROLL_ADD();
void OP_ROTATION_3D_ROLL_SUBSTRACT();
void OP_OPEN_DOOR();
void OP_CLOSE_DOOR();
void OP_INCREASE_FIELD_EXECUTION_MAX_CYCLES();
void OP_C8();
void OP_IF_PLAYER_IN_TRIGGER2();
void OP_IF_PLAYER_IN_TRIGGER();
void OP_CD();
void OP_CE();
void OP_CF();
void OP_SET_DIALOG_WINDOW_PARAM();
void OP_SHOW_DIALOG_WINDOW_MODE0();
void OP_SHOW_DIALOG_WINDOW_FOR_OTHER_ACTOR_MODE0();
void OP_D6();
void OP_SET_OBJECT_SWIVEL_ON_X_AXIS();
void OP_SET_OBJECT_SWIVEL_ON_Z_AXIS();
void OP_DA();
void OP_DB();
void OP_DC();
void OP_DE();
void OP_DF();
void OP_VRAM_OPERATION();
void OP_SETUP_FOG();
void OP_SET_CAMERA_LIMITS();
void OP_SET_BACKGROUND_CLEAR_COLOR();
void ORBIT();
void OP_EE();
void OP_WAIT_CAMERA();
void OP_SETUP_SCREEN_EFFECT1();
void OP_SETUP_CAMERA_SHAKE();
void OP_F3();
void OP_CLOSE_CURRENT_ACTOR_DIALOG();
void OP_SHOW_DIALOG_WINDOW_FOR_CURRENT_ACTOR_MODE3();
void OP_F6();
void OP_SET_FLAGS_DYNAMIC();
void OP_F9();
void OP_FB();
void OP_FC();
void OP_EXTENDED_OPCODE();
void OP_FF();

void OP_RAND_ROTATION();
void OP_IF_CHARACTER_IN_VIEW();
void OP_SET_CURRENT_ACTOR_SCALE();
void OPX_04();
void OPX_07();
void OP_SET_3D_SCALE();
void OPX_09();
void OPX_0A(); 
void OPX_0C();
void OP_SET_DIALOG_PORTRAIT();
void OP_SET_MUSIC_PARAMS();
void OPX_10();
void OPX_13();
void OPX_15();
void OPX_17();
void OP_ADD_TO_CURRENT_PARTY();
void OP_REMOVE_FROM_CURRENT_PARTY();
void OP_FINALIZE_PARTY_CHARACTER_LOADING();
void OPX_1C();
void OP_SWITCH_MAP_TO_GEARS();
void OPX_21();
void OPX_23();
void OP_SETUP_SCREEN_DISTORTION();
void OP_SCREEN_DISTORTION_FADE_OUT();
void OP_SET_PARTY_FRAME_MASK();
void OP_CLEAR_PARTY_FRAME_MASK();
void OP_MECHA_PLAY_ANIMATION();
void OPX_3D();
void OPX_3E();
void OPX_3F();
void OPX_40();
void OP_SET_ON_GEAR();
void OP_SET_OFF_GEAR();
void OPX_45();
void OPX_47();
void OP_LOAD_SPECIAL_2D_ANIMATION();
void OP_WAIT_LOADING_SPECIAL_2D_ANIMATION();
void OPX_4D();
void OP_ENABLE_MENU();
void OP_FREE_SPECIAL_2D_ANIMATION();
void OP_DISABLE_MENU();
void OP_DISABLE_COMPASS();
void OP_ENABLE_PLAYER_CONTROLS();
void OPX_54();
void OP_OPEN_LOAD_GAME_MENU();
void OPX_5B();
void OP_LOAD_NEW_MECHA();
void OPX_5D();
void OP_SET_TRANSPARENCY_MODE();
void OPX_5F();
void OP_SET_2D_BACKGROUND();
void OPX_61();
void OPX_62();
void OPX_65();
void OPX_66();
void OP_SET_LINK_OT(); 
void OPX_6B();
void OPX_7F();
void OPX_80();
void OPX_81();
void OPX_82();
void OP_SET_BOOTMODE();
void OPX_84();
void OPX_85();
void OPX_86();
void OP_WAIT_MENU();
void OP_SETUP_MECHA_FIELD_ARGS();
void OPX_8C();
void OPX_8E();
void OP_RESET_PARTICLE_CONFIGS();
void OP_SETUP_PARTICLE();
void OP_SETUP_PARTICLE_POSITION();
void OP_SETUP_LIFE1();
void OP_SETUP_LIFE2();
void OP_SETUP_SCALE();
void OP_SETUP_PARTICLE_COLOR();
void OP_CREATE_PARTICLE_EFFECT_FOR_CURRENT_ACTOR();
void OPX_97();
void OPX_98();
void OPX_99();
void OPX_9A();
void OP_TRANSITION_EFFECT_1();
void OP_TRANSITION_EFFECT_2();
void OP_TRANSITION_EFFECT_3();
void OP_TOGGLE_FRAME_LOCK();
void OP_ASSIGN_GEAR();
void OPX_A2(); 
void OP_RESTORE_GEAR();
void OP_GET_CURRENT_DISC_NUMBER();
void OP_SET_MAX_MECHA();
void OP_SETUP_PARTICLE_ANGLE();
void OPX_AF();
void OPX_B5();
void OPX_B8();
void OPX_BD();
void OPX_C8();
void OPX_CB();
void OPX_CC();
void OP_SET_PAUSE_DISABLED();

