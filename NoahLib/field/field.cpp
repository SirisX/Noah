#include "noahLib.h"
#include "field.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"
#include "kernel/gameState.h"
#include "walkMesh.h"
#include "fieldModel.h"
#include "kernel/isoManager.h"
#include "fieldDebugger/fieldInspector.h"
#include "fieldScriptSupport.h"
#include "opcodeTables.h"
#include "fieldGraphicObject.h"
#include "sprite/spriteSetup.h"
#include "kernel/trigo.h"
#include "dialogWindows.h"
#include "kernel/gte.h"
#include "compass.h"
#include "kernel/TIM.h"

#include "SDL_keyboard.h"

struct sFieldVramMapping
{
	s16 m0_vramX;
	s16 m2_vramY;
	s16 m4;
	s16 m6;
};
std::array<sFieldVramMapping, 32> fieldVramMapping;

std::vector<u8> rawFieldBundleForDebug;
std::vector<u8> rawFieldBundle;
std::vector<u8> rawFieldModels;
std::vector<u8> rawFieldScriptData;
std::vector<u8> rawFieldTriggerData;
std::vector<u8> rawFieldDialogBundle;
std::vector<u8> rawFieldWalkMesh;
std::vector<u8> rawFieldActorSetupParams;

// this is malloc/free in original code, I keep it around for debugger
std::vector<u8> rawFieldImageBundle;
std::vector<u8> rawFieldImageBundle2;


std::array<s16, 4> fieldInitVar1;

s32 totalObjects;

s32 fieldMapNumber = -1;
s32 currentFieldId0 = -1;
s32 currentFieldId1 = -1;

s32 totalActors;

std::array<s32, 3> currentParty;

std::vector<sFieldEntity> actorArray;

std::vector<sFieldActorSetupParams> fieldActorSetupParams;

s16 numWalkMesh = 0;
std::array<s32, 5> walkMeshNumTrianglePerBlock;
u8* walkMeshVar1Raw;
std::vector<u32>* walkMeshVar1;
std::array<u8*, 5> walkMeshVerticesRaw;
std::array<u8*, 5> walkMeshTriangleRaw;
std::array<std::vector<SVECTOR>*, 5> walkMeshVertices;
std::array<std::vector<sWalkMesh::sTriangleData>*, 5> walkMeshTriangle;
s32 walkMeshVar4 = 0;

s16 isFogSetup = 0;
s16 cameraTan = 0;

int playerControlledEntity = 0;

s16 entityMoveSpeedFactor = 0;

int g_frameOddOrEven = 0;

std::array<std::vector<u8>, 3> partyCharacterBuffersRaw;
std::array<std::vector<u8>, 3> partyCharacterBuffersCompressed;
std::array<sFieldActorSetupParams, 3> partyCharacterBuffers;
s16 actorCameraTracked = 0;
s32 pcInitVar1 = 0;
std::array<int, 11> PCToActorArray;

s32 fieldPolyCount;
s32 fieldPolyCount2;
s32 worldScaleFactor;
MATRIX worldScaleMatrix;
MATRIX cameraMatrix;
MATRIX cameraMatrix2;
s32 objectClippingMask = 1;
VECTOR fieldObjectRenderingVar1 = { 0,0,0 };
SVECTOR fieldObjectRenderingVar2 = { 0,0,0 };
s32 sceneSCRZ = 0;

const std::array<s8, 12> characterMappingTable = {
	0,1,2,3,4,5,6,7,8,2,6,0
};

s16 pcInitVar2 = 0;
std::array<int, 3> unkPartyTable;


u8 OPE7_param[3];

void fieldModelRelocation(std::vector<u8>::iterator& pModelData)
{
	MissingCode();
}

s32 fieldChangePrevented = -1;
s32 fieldMusicLoadPending = -1;
s32 fieldTransitionMode = 0;

u16 inputAllowedMask = 0xFFFF;
u16 padButtonForField;
u16 padButtonForField2;
sVec2_s16 newPadButtonForScripts[2];
u16 inputAllowedMask2;
u16 newPadButtonForDialogs;
u16 newPadButtonForDialogs2;
u16 newPadButtonForField;
u16 newPadButtonForField2;

struct sInputReplayEntry
{
	u16 m0_newPadButtonForScripts0[16];
	u16 m20_newPadButtonForScripts1[16];
	u16 m40_newPadButtonForDialogs[16];
	u16 m60_newPadButtonForDialogs2[16];
	u16 m80_newPadButtonForField[16];
	u16 mA0_newPadButtonForField2[16];
} inputReplayBuffer;

int inputReplayPosition = 0;
int inputReplayPosition2 = 0;
int inputOverflowed = 0;
int inputReplayOffset = 0;

void resetInputs()
{
	MissingCode();

	inputOverflowed = 0;
	inputReplayPosition = 0;
	inputReplayPosition2 = 0;
	inputReplayOffset = 0;
	newPadButtonForDialogs = 0;
	newPadButtonForDialogs2 = 0;
	newPadButtonForField = 0;
	newPadButtonForField2 = 0;

	MissingCode();

	newPadButtonForScripts[0].vx = 0;
	newPadButtonForScripts[1].vx = 0;

	MissingCode();
}

std::array<sFieldRenderContext, 2> fieldRenderContext;
sFieldRenderContext* pCurrentFieldRenderingContext = nullptr;

SVECTOR cameraProjectionAngles;
SVECTOR cameraRotation;
VECTOR cameraUp = { 0,0,0,0 };

void resetCameraData()
{
	op99Var5 = 8;
	op99Var6 = 8;

	MissingCode();

	cameraEye[0] = 0;
	cameraEye[1] = 0;
	cameraEye[2] = 0;
	cameraAt[0] = 0;
	cameraAt[1] = 0;
	cameraAt[2] = 0;
	cameraUp.vx = 0;
	cameraUp.vy = 0x10000000;
	cameraUp.vz = 0;
	cameraEye2.vx = 0;
	cameraEye2.vy = 0;
	cameraEye2.vz = 0;
	cameraAt2.vx = 0;
	cameraAt2.vy = 0;
	cameraAt2.vz = 0;

	MissingCode();

	cameraInterpolationMode = 0;
	cameraInterpolationFlags = 0;
	cameraInterpolationTargetNumSteps = 0;
	cameraInterpolationPositionNumSteps = 0;
	sceneSCRZ = 0x200;
	sceneDIP = 0x1e;
	sceneScale = 0x1000;

	MissingCode();
}

int linkOTIndex;

void resetFieldDefault()
{
	MissingCode();

	resetInputs();
	inputAllowedMask = 0xffff;
	padButtonForScripts[0].vx = 0;
	padButtonForScripts[1].vx = 0;
	padButtonForDialogs = 0;
	padButtonForDialogs2 = 0;
	padButtonForField = 0;
	padButtonForField2 = 0;

	MissingCode();

	compassDisabled = 0;

	MissingCode();

	linkOTIndex = 0x720;

	MissingCode();

	fieldTransitionMode = 2;

	MissingCode();

	entityMoveSpeedFactor = 0x1000;

	MissingCode();

	fieldChangePrevented = -1;
	op9DVar1 = 2;
	inputAllowedMask2 = 0xFFFF;

	MissingCode();

	dialogWindowFlag1 = 8;

	MissingCode();

	cameraProjectionAngles.vx = 0;
	cameraProjectionAngles.vy = 0;
	cameraProjectionAngles.vz = 0;

	cameraRotation.vx = 0;
	cameraRotation.vy = 0;
	cameraRotation.vz = 0;

	worldScaleFactor = 0x3000;

	createRotationMatrix(&cameraProjectionAngles, &currentProjectionMatrix);
	pCurrentFieldRenderingContext = &fieldRenderContext[0];
	resetCameraData();

	MissingCode();
}

bool g_LogOpcodes = false;
bool g_BreakOnMissingOpcode = false;

void LogOpcode(int actorId, u16 scriptOffset, u16 opcode)
{
	if (g_LogOpcodes)
	{
		printf("Actor0x%02X: offset 0x%04X OP_%02X\n", actorId, scriptOffset, opcode);
	}
}

void LogMissingOpcode(int actorId, u16 scriptOffset, u16 opcode)
{
	if (g_BreakOnMissingOpcode)
	{
		__debugbreak();
	}
}

void uploadFieldImages(std::vector<u8>::iterator& pImageData)
{
	MissingCode();
}

RECT* currentNpcSpriteUploadRect = nullptr;
std::vector<u8>::iterator currentNpcSpriteUploadDataPtr;

void transfertNpcSpriteSheetElement()
{
	MissingCode();
}

void uploadNpcSpriteSheet(std::vector<u8>::iterator& pImageData, int x, int y)
{
	int count = READ_LE_U32(pImageData);

	int xOffset = 0;

	for (int i = 0; i < count; i++)
	{
		int offset = READ_LE_U32(pImageData + 4 + i * 4);
		std::vector<u8>::iterator data = pImageData + offset;

		RECT rect;
		rect.x = x + xOffset;
		rect.y = y;
		rect.w = READ_LE_U16(data);
		rect.h = READ_LE_U16(data + 2);

		xOffset += 0x40;

		currentNpcSpriteUploadDataPtr = data + 4;
		currentNpcSpriteUploadRect = &rect;

		transfertNpcSpriteSheetElement();
	}

	MissingCode();
}

void setupField3d(std::vector<u8>::iterator input)
{
	computeMatrix(&cameraMatrix, (VECTOR*)cameraEye, (VECTOR*)cameraAt, &cameraUp);
	createRotationMatrix(&cameraProjectionAngles, &currentProjectionMatrix);
	MulRotationMatrix(&cameraMatrix, &currentProjectionMatrix);

	MissingCode();
}


void projectPositionOnTriangle(const SVECTOR& vec0, const SVECTOR& vec1, const SVECTOR& vec2, SVECTOR* outputPosition, VECTOR* inputPosition)
{
	VECTOR sStack72;
	VECTOR sStack56;
	VECTOR local_28;

	local_28.vx = vec1.vx - vec0.vx;
	local_28.vy = vec1.vy - vec0.vy;
	local_28.vz = vec1.vz - vec0.vz;
	VectorNormal(&local_28, &sStack72);

	local_28.vx = vec2.vx - vec0.vx;
	local_28.vy = vec2.vy - vec0.vy;
	local_28.vz = vec2.vz - vec0.vz;
	VectorNormal(&local_28, &sStack56);
	OuterProduct12(&sStack72, &sStack56, inputPosition);
	if (inputPosition->vy == 0) {
		outputPosition->vy = 0;
	}
	else {
		outputPosition->vy = vec0.vy + (short)((-(inputPosition->vz * (outputPosition->vz - vec0.vz)) - inputPosition->vx * (outputPosition->vx - vec0.vx)) / inputPosition->vy);
	}
}

s16 findTriangleInWalkMesh(int posX, int posZ, int walkmeshId, SVECTOR* param_4, VECTOR* param_5)
{
	SVECTOR resultPosition;

	resultPosition.vx = (short)posX;
	resultPosition.vy = 0;
	resultPosition.vz = (short)posZ;

	std::vector<sWalkMesh::sTriangleData>::iterator pTriangle = walkMeshTriangle[walkmeshId]->begin();
	std::vector<SVECTOR>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();

	sVec2_s16 refPos;
	refPos.set(posX, posZ);

	for (int i = 0; i < walkMeshNumTrianglePerBlock[walkmeshId]; i++)
	{
		sVec2_s16 pos0;
		sVec2_s16 pos1;
		sVec2_s16 pos2;

		pos0.set(pVertices[pTriangle->m0_verticeIndex[0]].vx, pVertices[pTriangle->m0_verticeIndex[0]].vz);
		pos1.set(pVertices[pTriangle->m0_verticeIndex[1]].vx, pVertices[pTriangle->m0_verticeIndex[1]].vz);
		pos2.set(pVertices[pTriangle->m0_verticeIndex[2]].vx, pVertices[pTriangle->m0_verticeIndex[2]].vz);

		if ((NCLIP(pos0, pos1, refPos) > -1) && (NCLIP(pos1, pos2, refPos) > -1) && (NCLIP(pos2, pos0, refPos) > -1))
		{
			SVECTOR vec0 = pVertices[pTriangle->m0_verticeIndex[0]];
			SVECTOR vec1 = pVertices[pTriangle->m0_verticeIndex[1]];
			SVECTOR vec2 = pVertices[pTriangle->m0_verticeIndex[2]];

			projectPositionOnTriangle(vec0, vec1, vec2, &resultPosition, param_5);
			param_4->vx = resultPosition.vx;
			param_4->vy = resultPosition.vy;
			param_4->vz = resultPosition.vz;

			return i;
		}

		pTriangle++;
	}

	param_4->vx = 0;
	param_4->vy = 0;
	param_4->vz = 0;

	param_5->vx = 0;
	param_5->vy = 0;
	param_5->vz = 0;
	return 0;
}

int getWalkmeshTriangleFlag(sFieldScriptEntity* param_1)
{
	if ((param_1->m4_flags >> (param_1->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
		int triangleId = param_1->m8_currentWalkMeshTriangle[param_1->m10_walkmeshId];
		int materialId = (*walkMeshTriangle[param_1->m10_walkmeshId])[triangleId].mC_indexInWalkmeshData1;
		return (*walkMeshVar1)[materialId];
	}
	return 0;
}

void initFieldScriptEntityValues(int index)
{
	sFieldScriptEntity* pFieldScriptEntity = actorArray[index].m4C_scriptEntity;

	pFieldScriptEntity->m0_flags = 0xB0;
	pFieldScriptEntity->m4_flags = 0x800;
	pFieldScriptEntity->m18[0] = 0x10;
	pFieldScriptEntity->m18[2] = 0x10;
	pFieldScriptEntity->m18[1] = 0x60;
	pFieldScriptEntity->m74 = -1;
	pFieldScriptEntity->m75 = -1;

	pFieldScriptEntity->m40.vx = 0;
	pFieldScriptEntity->m40.vy = 0;
	pFieldScriptEntity->m40.vz = 0;

	pFieldScriptEntity->m30_stepVector.vx = 0;
	pFieldScriptEntity->m30_stepVector.vy = 0;
	pFieldScriptEntity->m30_stepVector.vz = 0;

	pFieldScriptEntity->m60[2] = 0;
	pFieldScriptEntity->m60[0] = 0;
	pFieldScriptEntity->m60[1] = 0;

	pFieldScriptEntity->mD0_targetPositionOffset[0] = 0;
	pFieldScriptEntity->mD0_targetPositionOffset[1] = 0;
	pFieldScriptEntity->mD0_targetPositionOffset[2] = 0;
	pFieldScriptEntity->mE6 = 0;
	pFieldScriptEntity->mEA_currentAnimationPlaying = 0xff;

	pFieldScriptEntity->mE2 = 0;
	pFieldScriptEntity->mCC_scriptPC = 0;
	pFieldScriptEntity->m6E = 0;
	pFieldScriptEntity->m12C_flags &= 0xffffffdf;
	pFieldScriptEntity->m11E = 0x200;
	pFieldScriptEntity->m18[3] = pFieldScriptEntity->m18[0];
	pFieldScriptEntity->m12C_flags &= 0xfffffffc;
	pFieldScriptEntity->mFC[5] = 0x80;
	pFieldScriptEntity->mFC[4] = 0x80;
	pFieldScriptEntity->mFC[3] = 0x80;
	pFieldScriptEntity->mFC[2] = 0x80;
	pFieldScriptEntity->mFC[1] = 0x80;
	pFieldScriptEntity->mFC[0] = 0x80;
	pFieldScriptEntity->m128 = -1;
	pFieldScriptEntity->m12C_flags &= 0xfffcffff;
	pFieldScriptEntity->m130.m0 = 0;
	pFieldScriptEntity->m12C_flags &= 0xf003ffff;

	for (int i = 0; i < 8; i++)
	{
		pFieldScriptEntity->m8C_scriptSlots[i].m2_delay = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m0_scriptPC = -1;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m16 = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m18 = 0xF;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m22 = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m23_walkMode = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m0 = -1;
	}

	delete pFieldScriptEntity->m120_special2dAnimation;
	pFieldScriptEntity->m120_special2dAnimation = nullptr;
	pFieldScriptEntity->m120_special2dAnimationRaw.clear();
	pFieldScriptEntity->mE4_playableCharacterId = 0xFF;
	pFieldScriptEntity->m76 = 0x100;

	pFieldScriptEntity->m82[1] = 0;
	pFieldScriptEntity->m82[0] = 0;
	pFieldScriptEntity->m88[1] = 0;
	pFieldScriptEntity->m88[0] = 0;
	pFieldScriptEntity->m84 = 0;
	pFieldScriptEntity->mCF = 0;
	pFieldScriptEntity->mCE_currentScriptSlot = 0;
	pFieldScriptEntity->mE8 = 0;
	pFieldScriptEntity->m10_walkmeshId = 0;
	pFieldScriptEntity->mEC_elevation = 0;

	pFieldScriptEntity->m134.m7 = 0;

	pFieldScriptEntity->m12C_flags &= 0xffffe03f;

	pFieldScriptEntity->m134.m5 = 0;

	pFieldScriptEntity->m102_rotationCount = rand();

	pFieldScriptEntity->mF4_scale3d[0] = 0x1000;
	pFieldScriptEntity->mF4_scale3d[1] = 0x1000;
	pFieldScriptEntity->mF4_scale3d[2] = 0x1000;
	pFieldScriptEntity->m10D = 0xff;
	pFieldScriptEntity->m80_DialogAvatarFace = -1;
	pFieldScriptEntity->m106_currentRotation = -0x8000;
	pFieldScriptEntity->m104_rotation = -0x8000;
	pFieldScriptEntity->m108_rotation3 = -0x8000;
	pFieldScriptEntity->m124_special2dAnimationId = -1;
	pFieldScriptEntity->mE3 = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[3] = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[2] = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[1] = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[0] = 0;
	pFieldScriptEntity->m12C_flags &= 0xffffffe3;

	SVECTOR local_48[4];
	VECTOR local_88[4];

	for (int i = 0; i < numWalkMesh; i++)
	{
		pFieldScriptEntity->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(actorArray[index].mC_matrix.t[0], actorArray[index].mC_matrix.t[2], i, &local_48[i], &local_88[i]);
		if ((pFieldScriptEntity->m8_currentWalkMeshTriangle[i] != -1) && (walkMeshNumTrianglePerBlock[i] <= pFieldScriptEntity->m8_currentWalkMeshTriangle[i]))
		{
			// seems like this would be a bug
			assert(0);

			walkMeshNumTrianglePerBlock[i] = 0;
			local_88[i].vx = 0;
			local_88[i].vy = 0;
			local_88[i].vz = 0;
			local_48[i].vx = 0;
			local_48[i].vy = 0;
			local_48[i].vz = 0;
		}
	}

	pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);

	pFieldScriptEntity->m50.vx = local_88[pFieldScriptEntity->m10_walkmeshId].vx;
	pFieldScriptEntity->m50.vy = local_88[pFieldScriptEntity->m10_walkmeshId].vy;
	pFieldScriptEntity->m50.vz = local_88[pFieldScriptEntity->m10_walkmeshId].vz;

	if ((actorArray[index].m58_flags & 0x80) == 0) {
		actorArray[index].mC_matrix.t[1] = local_48[pFieldScriptEntity->m10_walkmeshId].vy;
	}

	(pFieldScriptEntity->m20_position).vx = (int)actorArray[index].mC_matrix.t[0] << 0x10;
	(pFieldScriptEntity->m20_position).vy = (int)actorArray[index].mC_matrix.t[1] << 0x10;
	(pFieldScriptEntity->m20_position).vz = (int)actorArray[index].mC_matrix.t[2] << 0x10;

	pFieldScriptEntity->m72_elevation = actorArray[index].mC_matrix.t[1];
}

void initFieldScriptEntity2dSprite(sFieldEntity2dSprite* pSprite)
{
	MissingCode();
}

int numInitializedFieldScriptEntities = 0;

void initFieldScriptEntity(int index)
{
	if (index < totalActors)
	{
		numInitializedFieldScriptEntities++;
		sFieldScriptEntity* pNewFieldScriptEntity = new sFieldScriptEntity;
		actorArray[index].m4C_scriptEntity = pNewFieldScriptEntity;
		memset(pNewFieldScriptEntity, 0, sizeof(sFieldScriptEntity));
		new(pNewFieldScriptEntity) sFieldScriptEntity;

		actorArray[index].m5A = 0;

		if (actorArray[index].m58_flags & 0x2000)
		{
			pNewFieldScriptEntity->m118.resize(32);
			MissingCode();
		}

		initFieldScriptEntityValues(index);
		actorArray[index].m8_2dSprite = new sFieldEntity2dSprite;
		initFieldScriptEntity2dSprite(actorArray[index].m8_2dSprite);
	}
}

void traceNextAlloc(int state)
{
	MissingCode();
}

void initModel1(sModelBlock& pModelBlock, std::vector<sTag*>& outputBuffer1, std::vector<sTag*>& outputBuffer2)
{
	traceNextAlloc(0x25);

	outputBuffer1.resize(pModelBlock.m34_instanceBufferSize, nullptr);
	outputBuffer2.resize(pModelBlock.m34_instanceBufferSize, nullptr);
}

int initModel2Sub0Var0;
int initModel2Sub0Var1;

void initModel2Sub0(void)
{
	initModel2Sub0Var0 = 0;
	initModel2Sub0Var1 = 1;
	return;
}

s32 primD_initSub0Sub0Var1 = 0;
s16 primD_initSub0Sub0Var0 = 0;

void setupPrimTexturePage(ushort param_1)

{
	primD_initSub0Sub0Var0 = (ushort)primD_initSub0Sub0Var1;
	if (initModel2Sub0Var0 == 1) {
		primD_initSub0Sub0Var0 = param_1 & 0xffe0 | primD_initSub0Sub0Var0;
	}
	else {
		if (initModel2Sub0Var0 != 2) {
			primD_initSub0Sub0Var0 = param_1;
			return;
		}
	}
	return;
}

s16 primD_initSub0Sub1Var0 = 0;
s32 primD_initSub0Sub1Var1 = 0;

void setupPrimClut(ushort param_1)
{
	primD_initSub0Sub1Var0 = param_1;
	if (initModel2Sub0Var1 == 0) {
		primD_initSub0Sub1Var0 = primD_initSub0Sub1Var0 & 0xf | (ushort)primD_initSub0Sub1Var1;
	}
	return;
}

int primD_isValid(std::vector<u8>::iterator displayList)
{
	byte bVar1;
	int iVar2;

	bVar1 = READ_LE_U8(displayList + 3);
	iVar2 = 1;
	if ((bVar1 & 0xf0) == 0xc0) {
		if (bVar1 == 0xc4) {
			setupPrimTexturePage(READ_LE_U16(displayList));
			iVar2 = 0;
		}
		else {
			iVar2 = 1;
			if (bVar1 == 0xc8) {
				setupPrimClut(READ_LE_U16(displayList));
				iVar2 = 0;
			}
		}
	}
	return iVar2;
}

std::vector<sTag*>::iterator currentModelInstanceArray8;

std::vector<u8>::iterator currentModelBlockDisplayLists;
std::vector<u8>::iterator g_currentModelBlockSubBlocks;
std::vector<u8>::iterator currentModelBlockNormals;
std::vector<u8>::iterator currentModelBlockVertices;
std::vector<u8>::iterator currentModeBlock18;

void computeFaceNormal(const SVECTOR& v0, const SVECTOR& v1, const SVECTOR& v2, SVECTOR* pNormal)
{
	MissingCode();
}

void NormalColorCol(SVECTOR* faceNormal, std::vector<u8>::iterator displayList, POLY_FT3* pNewPoly)
{
	MissingCode();
}

int prim0_init(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam)
{
	POLY_FT3* pNewPoly = new POLY_FT3;
	*currentModelInstanceArray8 = pNewPoly;
	currentModelInstanceArray8++;

	pNewPoly->m3_size = 4;

	if ((initParam & 1) == 0) {
		if ((initParam & 4) == 0) {
			pNewPoly->r0 = READ_LE_U8(displayList + 0);
			pNewPoly->g0 = READ_LE_U8(displayList + 1);
			pNewPoly->b0 = READ_LE_U8(displayList + 2);
			pNewPoly->code = READ_LE_U8(displayList + 3);
			return 1;
		}
		currentModeBlock18 = currentModeBlock18 + 4;
	}
	else {
		if ((initParam & 2) == 0) {
			SVECTOR faceNormal;
			computeFaceNormal(
				SVECTOR::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock) * 8),
				SVECTOR::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 2) * 8),
				SVECTOR::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 4) * 8),
				&faceNormal
			);
			NormalColorCol(&faceNormal, displayList, pNewPoly);
			pNewPoly->code = READ_LE_U8(displayList + 3);
			return 1;
		}

		*(u32*)&(*currentModeBlock18) = READ_LE_U32(displayList);
		currentModeBlock18 = currentModeBlock18 + 4;

		computeFaceNormal(
			SVECTOR::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock) * 8),
			SVECTOR::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 2) * 8),
			SVECTOR::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 4) * 8),
			(SVECTOR*)&(*currentModeBlock18)
		);
	}
	NormalColorCol((SVECTOR*)&(*currentModeBlock18), displayList, pNewPoly);
	currentModeBlock18 = currentModeBlock18 + 4;
	pNewPoly->code = READ_LE_U8(displayList + 3);
	return 1;
}

int prim4_init(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam)
{
	POLY_FT3* pNewPoly = new POLY_FT3;

	pNewPoly->m3_size = 4;
	pNewPoly->r0 = READ_LE_U8(displayList + 0);
	pNewPoly->g0 = READ_LE_U8(displayList + 1);
	pNewPoly->b0 = READ_LE_U8(displayList + 2);
	pNewPoly->code = READ_LE_U8(displayList + 3);

	*currentModelInstanceArray8 = pNewPoly;
	currentModelInstanceArray8++;
	return 1;
}

int prim5_init(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam)
{
	if (primD_isValid(displayList))
	{
		POLY_FT3* pNewPoly = new POLY_FT3;

		pNewPoly->m3_size = 7;
		pNewPoly->code = READ_LE_U8(displayList + 3);
		pNewPoly->u0 = READ_LE_U8(displayList + 4);
		pNewPoly->v0 = READ_LE_U8(displayList + 5);
		pNewPoly->clut = primD_initSub0Sub1Var0;
		pNewPoly->u1 = READ_LE_U8(displayList + 6);
		pNewPoly->v1 = READ_LE_U8(displayList + 7);
		pNewPoly->tpage = primD_initSub0Sub0Var0;
		pNewPoly->u2 = READ_LE_U8(displayList + 0);
		pNewPoly->v2 = READ_LE_U8(displayList + 1);

		*currentModelInstanceArray8 = pNewPoly;
		currentModelInstanceArray8++;
		return 1;
	}
	return 0;
}

int prim8_init(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam)
{
	POLY_FT3* pNewPoly = new POLY_FT3;

	pNewPoly->m3_size = 4;
	*currentModelInstanceArray8 = pNewPoly;
	currentModelInstanceArray8++;

	if ((initParam & 1) == 0) {
		if ((initParam & 4) == 0) {
			pNewPoly->r0 = READ_LE_U8(displayList + 0);
			pNewPoly->g0 = READ_LE_U8(displayList + 1);
			pNewPoly->b0 = READ_LE_U8(displayList + 2);
			pNewPoly->code = READ_LE_U8(displayList + 3);
			return 1;
		}
		currentModeBlock18 = currentModeBlock18 + 4;
	}
	else
	{
		assert(0);
	}
	pNewPoly->code = READ_LE_U8(displayList + 3);
	return 1;
}


int primC_init(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam)
{
	POLY_F4* pNewPoly = new POLY_F4;

	pNewPoly->m3_size = 5;
	pNewPoly->r0 = READ_LE_U8(displayList + 0);
	pNewPoly->g0 = READ_LE_U8(displayList + 1);
	pNewPoly->b0 = READ_LE_U8(displayList + 2);
	pNewPoly->code = READ_LE_U8(displayList + 3);

	*currentModelInstanceArray8 = pNewPoly;
	currentModelInstanceArray8++;
	return 1;
}

s32 initFieldDrawEnvsSub0Var0 = 319;
s32 initFieldDrawEnvsSub0Var1 = 0xEE0000;

s32 primD_2Var0 = 2;
std::array<sTag, 4096>* currentOTEntry = nullptr;

void primD_2(std::vector<u8>::iterator meshSubBlock, int count)
{
	std::array<sTag, 4096>& pOT = *currentOTEntry;
	int depthGranularity = primD_2Var0 + 2;

	for (int i = 0; i < count; i++)
	{
		POLY_FT4* pOutputPrim = (POLY_FT4*)*currentModelInstanceArray8;
		currentModelInstanceArray8++;

		std::vector<u8>::iterator pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
		std::vector<u8>::iterator pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
		std::vector<u8>::iterator pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
		std::vector<u8>::iterator pVertices4 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 6) * 8;
		meshSubBlock += 8;

		gte_ldv3(pVertices1, pVertices2, pVertices3);
		gte_rtpt();

		int flagsTriangle0;
		gte_stlzc(&flagsTriangle0);

		sVec2_s16 xy0;
		sVec2_s16 xy1;
		sVec2_s16 xy2;
		gte_stsxy3(&xy0, &xy1, &xy2);

		if (flagsTriangle0 > 0)
		{
			gte_nclip(); // write the normal to mac0

			int normalMagnitude;
			gte_getMAC0(&normalMagnitude);

			if (normalMagnitude > 0) // is first triangle facing screen
			{
				gte_ldv0(pVertices4); // load last vertex and compute 3rd point
				gte_rtps();

				int flagsTriangle1;
				gte_stlzc(&flagsTriangle1);

				sVec2_s16 xy3;
				gte_stsxy2(&xy3);

				if (flagsTriangle1 > 0)
				{
					//if (!(((((initFieldDrawEnvsSub0Var1 <= xy0.asS32() && (initFieldDrawEnvsSub0Var1 <= xy1.asS32())) && (initFieldDrawEnvsSub0Var1 <= xy2.asS32())) && (initFieldDrawEnvsSub0Var1 <= xy3.asS32())))))
					{
						pOutputPrim->x0y0 = xy0;
						pOutputPrim->x1y1 = xy1;
						pOutputPrim->x2y2 = xy2;
						pOutputPrim->x3y3 = xy3;

						//if (((initFieldDrawEnvsSub0Var0 <= (xy0.vx)) && (initFieldDrawEnvsSub0Var0 <= (xy1.vx))) && (initFieldDrawEnvsSub0Var0 <= (xy2.vx) && (xy3.vx) < initFieldDrawEnvsSub0Var0))
						{
							int sz0, sz1, sz2, sz3;
							gte_stsz4(&sz0, &sz1, &sz2, &sz3);
							if (sz0 && sz1 && sz2 && sz3) {
								int polyz = std::min<int>(sz0, sz1);
								polyz = std::min<int>(sz2, polyz);
								polyz = std::min<int>(sz3, polyz);
								fieldPolyCount = fieldPolyCount + 1;
								assert(polyz);

								sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
								pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
								pOutputPrim->m0_pNext = sDestTag;
								pOutputPrim->m3_size = 9;
							}
						}
					}
				}
			}
		}
	}
}
int primD_init(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam)
{
	if (primD_isValid(displayList))
	{
		POLY_FT4* pNewPoly = new POLY_FT4;

		pNewPoly->m3_size = 9;
		pNewPoly->r0 = READ_LE_U8(displayList + 0);
		pNewPoly->g0 = READ_LE_U8(displayList + 1);
		pNewPoly->b0 = READ_LE_U8(displayList + 2);
		pNewPoly->code = READ_LE_U8(displayList + 3);
		pNewPoly->u0 = READ_LE_U8(displayList + 4);
		pNewPoly->v0 = READ_LE_U8(displayList + 5);
		pNewPoly->clut = primD_initSub0Sub1Var0;
		pNewPoly->u1 = READ_LE_U8(displayList + 6);
		pNewPoly->v1 = READ_LE_U8(displayList + 7);
		pNewPoly->tpage = primD_initSub0Sub0Var0;
		pNewPoly->u2 = READ_LE_U8(displayList + 8);
		pNewPoly->v2 = READ_LE_U8(displayList + 9);
		pNewPoly->u3 = READ_LE_U8(displayList + 10);
		pNewPoly->v3 = READ_LE_U8(displayList + 11);

		*currentModelInstanceArray8 = pNewPoly;
		currentModelInstanceArray8++;
		return 1;
	}
	return 0;
}

typedef void(*t_primRenderFunc)(std::vector<u8>::iterator meshSubBlock, int count);
typedef int(*t_primInitFunc)(std::vector<u8>::iterator displayList, std::vector<u8>::iterator meshBlock, int initParam);

struct sPolyTypeRenderDefinition
{
	t_primRenderFunc m0_type0;
	t_primRenderFunc m4_type1;
	t_primRenderFunc m8_type2;
	t_primRenderFunc mC_type3;
	t_primRenderFunc m10_type4;
	t_primRenderFunc m14_type5;
	t_primInitFunc m18_init;
	int m1C_size;
	int m20_sizeDisplayList;
	int m24_sizeInstanceArray;
};

const std::array<sPolyTypeRenderDefinition, 17> polyRenderDefs = {{
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim0_init,	8,	0x4,	0x04}, // 0x0
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x8,	0x20}, // 0x1
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x1C}, // 0x2
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x8,	0x28}, // 0x3
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim4_init,	8,	0x4,	0x14}, // 0x4
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim5_init,	8,	0x8,	0x20}, // 0x5
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x1C}, // 0x6
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x8,	0x28}, // 0x7
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim8_init,	8,	0x4,	0x18}, // 0x8
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x28}, // 0x9
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x24}, // 0xA
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x34}, // 0xB
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	primC_init,	8,	0x4,	0x18}, // 0xC
	{	nullptr,	nullptr,	primD_2,	nullptr,	nullptr,	nullptr,	primD_init,	8,	0xC,	0x28}, // 0xD
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x24}, // 0xE
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x34}, // 0xF
	{	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x20}, // 0x10
}};

void initModel5(sModelBlock* pModelBlock)
{
	MissingCode();
}

void* initModelDynamicVertices(sModelBlock* pModelBlock)
{
	MissingCode();
	return nullptr;
}

void initModel2(sModelBlock* pModelBlock, std::vector<sTag*>& outputBuffer, int param_3)
{
	currentModelInstanceArray8 = outputBuffer.begin();

	if ((((pModelBlock->m0_flags & 1) == 0) && (pModelBlock->m30 != 0)) && (param_3 != 0)) {
		traceNextAlloc(0x26);
		pModelBlock->m18.resize(pModelBlock->m30);
		pModelBlock->m0_flags = pModelBlock->m0_flags | 1;
	}

	currentModelBlockDisplayLists = pModelBlock->m_model->mRawData.begin() + pModelBlock->m14_offsetDisplayList;
	g_currentModelBlockSubBlocks = pModelBlock->m_model->mRawData.begin() + pModelBlock->m10_offsetMeshBlocks;
	currentModelBlockNormals = pModelBlock->m_model->mRawData.begin() + pModelBlock->mC_offsetNormals;
	currentModelBlockVertices = pModelBlock->m_model->mRawData.begin() + pModelBlock->m8_offsetVertices;
	currentModeBlock18 = pModelBlock->m18.begin();

	int initParam;
	switch (param_3)
	{
	case 0:
		initParam = 0;
		break;
	case 1:
		initParam = 1;
		break;
	case 2:
		if (pModelBlock->m0_flags & 2)
		{
			initParam = 1;
			if (pModelBlock->m0_flags & 1)
			{
				initParam = 4;
			}
		}
		else
		{
			if (pModelBlock->m0_flags & 1)
			{
				initParam = 1;
			}
			else
			{
				initParam = 3;
				pModelBlock->m0_flags |= 2;
			}
		}
		break;
	case 3:
		initParam = 3;
		pModelBlock->m0_flags |= 2;
		break;
	default:
		assert(0);
	}

	fieldPolyCount2 += pModelBlock->m4_numPrims;

	int currentMeshBlockCount = pModelBlock->m6_numMeshBlock;
	while (currentMeshBlockCount = currentMeshBlockCount - 1, currentMeshBlockCount != 0xffffffff) {
		int primType = READ_LE_U8(g_currentModelBlockSubBlocks);
		int numPrims = READ_LE_U16(g_currentModelBlockSubBlocks + 2) - 1;

		t_primInitFunc pInitFunction = polyRenderDefs[primType].m18_init;
		g_currentModelBlockSubBlocks += 4;
		while (numPrims != -1)
		{
			if (pInitFunction(currentModelBlockDisplayLists, g_currentModelBlockSubBlocks, (int)initParam) == 0)
			{
				currentModelBlockDisplayLists += 4;
			}
			else
			{
				g_currentModelBlockSubBlocks += polyRenderDefs[primType].m1C_size;
				//currentModelInstanceArray8 += polyRenderDefs[primType].m24_sizeInstanceArray;
				currentModelBlockDisplayLists += polyRenderDefs[primType].m20_sizeDisplayList;
				numPrims = numPrims + -1;

			}
		}
	}

	initModel2Sub0();
}

int fieldScriptEntityAlreadyInitialized = 0;

// 0: scenarioFlag
std::array<s16, 1024> fieldVars;

void setVar(int varIndex, s16 value)
{
	fieldVars[varIndex / 2] = value;
}

void setVarsForCurrentParty()
{
	setVar(0x3E, currentParty[0]);
	setVar(0x40, currentParty[1]);
	setVar(0x42, currentParty[2]);
}

sFieldEntity* pCurrentFieldEntity;
int fieldScriptInitVar0;
int currentScriptFinished;

u16 getScriptEntryPoint(int entityId, int scriptIndex)
{
	return READ_LE_U16(rawFieldScriptData.begin() + (entityId * 0x20 + scriptIndex) * 2 + 0x84);
}

int breakCurrentScript;
int fieldExectuteMaxCycles;

int fieldDebugDisable = 0;
int renderCharsDisabled = 0;

void sprintf_screen(const char* format, ...)
{
	MissingCode();
}

void initModel3(int, int)
{
	MissingCode();
}

void execSpriteCallbacks2()
{
	MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int clutYEntry, sFieldActorSetupParams* pSetup, int param_4, int clutXEntry, int param_6, int param_7)
{
	initModel3(8, 0);
	actorArray[actorId].m4C_scriptEntity->m127 = clutYEntry;
	actorArray[actorId].m4C_scriptEntity->m126 = param_6;
	actorArray[actorId].m4C_scriptEntity->m134.m0 = clutXEntry;
	actorArray[actorId].m4C_scriptEntity->m130.m28 = param_4;
	actorArray[actorId].m4C_scriptEntity->m134.m4 = param_7;

	if (param_4 == 0)
	{
		s16 vramX = fieldVramMapping[clutYEntry].m0_vramX;
		s16 vramY = fieldVramMapping[clutYEntry].m2_vramY;

		if (clutXEntry == 0)
		{
			if (actorArray[actorId].m5A & 1) {
				deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
			}

			actorArray[actorId].m4_pVramSpriteSheet = createFieldEntitySub4(pSetup, 0x100, ((clutYEntry + 480) * 0x10000) >> 0x10, vramX, vramY, 0x40);
		}
		else
		{
			if ((actorArray[actorId].m5A & 1U) != 0) {
				deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
			}
			actorArray[actorId].m4_pVramSpriteSheet = createFieldEntitySub4Ext(pSetup, (short)((clutXEntry * 0x10 + 0x100) * 0x10000 >> 0x10), (short)((uint)((clutYEntry + 0x1e0) * 0x10000) >> 0x10), vramX, vramY, 0x40, clutXEntry);
		}
	}
	else
	{
		int iVar4;
		short sVar5;

		if ((actorArray[actorId].m5A & 1U) != 0) {
			deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
		}
		if (param_4 == 1) {
			iVar4 = clutYEntry + 0xe0;
			sVar5 = 0x280;
		}
		else {
			iVar4 = clutYEntry + 0xe3;
			sVar5 = 0x2a0;
		}
		actorArray[actorId].m4_pVramSpriteSheet = createFieldEntitySub4(pSetup, 0x100, (short)((uint)(iVar4 * 0x10000) >> 0x10), sVar5, (short)clutYEntry * 0x40 + 0x100, 8);
		OP_INIT_ENTITY_SCRIPT_sub0Sub3(actorArray[actorId].m4_pVramSpriteSheet, 0x20);
	}

	actorArray[actorId].m5A |= 1;

	sFieldEntitySub4* pFieldEntitySub4 = actorArray[actorId].m4_pVramSpriteSheet;

	int temp0;
	int temp1;
	int temp2;
	OP_INIT_ENTITY_SCRIPT_sub0Sub4(pFieldEntitySub4, 0, &temp0, &temp1, &temp2);
	OP_INIT_ENTITY_SCRIPT_sub0Sub5(pFieldEntitySub4, 3);

	pFieldEntitySub4->m2C_scale = 0xC00;
	pFieldEntitySub4->m82 = 0x2000;

	if (!fieldScriptEntityAlreadyInitialized)
	{
		pFieldEntitySub4->m0_position.vx = actorArray[actorId].m4C_scriptEntity->m20_position.vx;
		pFieldEntitySub4->m0_position.vy = actorArray[actorId].m4C_scriptEntity->m20_position.vy;
		pFieldEntitySub4->m0_position.vz = actorArray[actorId].m4C_scriptEntity->m20_position.vz;
		pFieldEntitySub4->mC.vy = 0;
		pFieldEntitySub4->mC.vx = 0;
		pFieldEntitySub4->mC.vy = 0;
		pFieldEntitySub4->mC.vz = 0;
		pFieldEntitySub4->m1C = 0x10000;
		pFieldEntitySub4->m84 = actorArray[actorId].mC_matrix.t[1] & 0xFFFF;
		if (!param_4)
		{
			actorArray[actorId].m4C_scriptEntity->m18[1] = temp1 * 2;
		}
		else
		{
			actorArray[actorId].m4C_scriptEntity->m18[1] = 0x40;
		}
	}
	else
	{
		assert(0);
	}

	if (isFogSetup)
	{
		pFieldEntitySub4->m40 |= 0x40000;
	}

	OP_INIT_ENTITY_SCRIPT_sub0Sub6(pFieldEntitySub4, 0);
	OP_INIT_ENTITY_SCRIPT_sub0Sub7(pFieldEntitySub4, 0);
	initModel3(8, 0);

	pFieldEntitySub4->m7C->m14_actorId = actorId;
	OP_INIT_ENTITY_SCRIPT_sub0Sub8(pFieldEntitySub4, &fieldActorCallback);

	if (!param_7)
	{
		OP_INIT_ENTITY_SCRIPT_sub0Sub9(pFieldEntitySub4);
		execSpriteCallbacks2();
		if (pFieldEntitySub4->m7C->mC == 0xff) {
			(actorArray[actorId].m4C_scriptEntity)->mEA_currentAnimationPlaying = 0xff;
			(actorArray[actorId].m4C_scriptEntity)->m4_flags = (actorArray[actorId].m4C_scriptEntity)->m4_flags | 0x1000000;
			pFieldEntitySub4->m0_position.vx = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vx;
			pFieldEntitySub4->m0_position.vy = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vy;
			pFieldEntitySub4->m0_position.vz = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vz;
		}
	}

	actorArray[actorId].mC_matrix.t[0] = actorArray[actorId].m4C_scriptEntity->m20_position.vx >> 16;
	actorArray[actorId].mC_matrix.t[1] = actorArray[actorId].m4C_scriptEntity->m20_position.vy >> 16;
	actorArray[actorId].mC_matrix.t[2] = actorArray[actorId].m4C_scriptEntity->m20_position.vz >> 16;

	actorArray[actorId].m2C_matrixBackup.t = actorArray[actorId].mC_matrix.t;

	pFieldEntitySub4->m84 = actorArray[actorId].mC_matrix.t[1] & 0xFFFF;

	pFieldEntitySub4->m0_position.vx = actorArray[actorId].m4C_scriptEntity->m20_position.vx;
	pFieldEntitySub4->m0_position.vy = actorArray[actorId].m4C_scriptEntity->m20_position.vy;
	pFieldEntitySub4->m0_position.vz = actorArray[actorId].m4C_scriptEntity->m20_position.vz;
	fieldScriptInitVar0++;
}

void OP_INIT_ENTITY_SCRIPT_sub1()
{
	actorArray[currentFieldActorId].mC_matrix.t[0] = actorArray[currentFieldActorId].m2C_matrixBackup.t[0] = pCurrentFieldScriptActor->m20_position.vx >> 16;
	actorArray[currentFieldActorId].mC_matrix.t[1] = actorArray[currentFieldActorId].m2C_matrixBackup.t[1] = pCurrentFieldScriptActor->m20_position.vy >> 16;
	actorArray[currentFieldActorId].mC_matrix.t[2] = actorArray[currentFieldActorId].m2C_matrixBackup.t[2] = pCurrentFieldScriptActor->m20_position.vz >> 16;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = pCurrentFieldScriptActor->m20_position.vx;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = pCurrentFieldScriptActor->m20_position.vy;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = pCurrentFieldScriptActor->m20_position.vz;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vy = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m84 = pCurrentFieldScriptActor->m20_position.vy >> 16;
	pCurrentFieldScriptActor->m72_elevation = pCurrentFieldScriptActor->m20_position.vy >> 16;
}

int asyncLoadingVar1 = 0xFF;
int asyncLoadingVar2 = 0;

int isAsyncLoadingProhibited()
{
	MissingCode();
	return 0;
}

int findFreePartySlot(int param_1, int* param_2)
{
	int iVar1;

	iVar1 = 0;
	while (true) {
		if (currentParty[iVar1] == param_1) {
			return -1;
		}
		if (currentParty[iVar1])
			break;
		iVar1 = iVar1 + 1;

		if (2 < iVar1) {
			return -1;
		}
	}
	*param_2 = iVar1;
	return 0;
}

int asyncPartyCharacterLoadingCharacterIndex = 0;
int asyncPartyCharacterLoadingIndex = 0;
int fieldExecuteVar1 = 0;
std::array<int, 3> asyncPartyCharacterLoadingTable;
std::vector<u8> asyncPartyCharacterLoadingBuffer;

void copyPartySlotFromNext(uint param_1)
{
	s32* piVar7 = &partyToFieldEntityArrayMapping[param_1 + 1];
	int iVar13 = *piVar7;
	if (iVar13 == 0xff) {
		asyncPartyCharacterLoadingTable[param_1] = asyncPartyCharacterLoadingTable[param_1 + 1];
		currentParty[param_1] = currentParty[param_1 + 1];
		partyToFieldEntityArrayMapping[param_1] = *piVar7;
		currentParty[param_1 + 1] = 0xff;
		asyncPartyCharacterLoadingTable[param_1 + 1] = 0xff;
		*piVar7 = 0xff;
	}
	else {
		partyCharacterBuffers[param_1] = partyCharacterBuffers[param_1 + 1];
		asyncPartyCharacterLoadingTable[param_1] = asyncPartyCharacterLoadingTable[param_1 + 1];
		currentParty[param_1] = currentParty[param_1 + 1];
		partyToFieldEntityArrayMapping[param_1] = partyToFieldEntityArrayMapping[param_1 + 1];
		sFieldScriptEntity* psVar3 = actorArray[iVar13].m4C_scriptEntity;
		if ((psVar3->m126 & 0x80) == 0) {
			OP_INIT_ENTITY_SCRIPT_sub0(iVar13, param_1, &partyCharacterBuffers[param_1], 1, 0, param_1, 1);
		}
		else {
			OP_INIT_ENTITY_SCRIPT_sub0(iVar13, psVar3->m127, &fieldActorSetupParams[psVar3->m126 & 0x7f], psVar3->m130.m28, psVar3->m134.m0, actorArray[iVar13].m4C_scriptEntity->m126, actorArray[iVar13].m4C_scriptEntity->m134.m4);
		}
		currentParty[param_1 + 1] = 0xff;
		asyncPartyCharacterLoadingTable[param_1 + 1] = 0xff;
		partyToFieldEntityArrayMapping[param_1 + 1] = 0xff;
	}
	return;
}

void emptyPartySlot(int param_1)
{
	uint* puVar1;
	ushort* puVar2;
	ushort uVar3;
	int iVar4;
	sFieldScriptEntity* psVar5;
	sFieldScriptEntity* psVar6;
	sFieldEntity* psVar7;
	int iVar8;
	int* piVar9;

	psVar7 = pCurrentFieldEntity;
	psVar5 = pCurrentFieldScriptActor;
	iVar4 = currentFieldActorId;
	piVar9 = &partyToFieldEntityArrayMapping[param_1];
	if (*piVar9 != 0xff) {
		pCurrentFieldEntity = &actorArray[*piVar9];
		uVar3 = pCurrentFieldScriptActor->mCC_scriptPC;
		pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
		initFieldScriptEntityValues(*piVar9);
		iVar8 = *piVar9;
		currentFieldActorId = iVar8;
		actorArray[iVar8].m58_flags = actorArray[iVar8].m58_flags & 0xf07f | 0x200;
		OP_INIT_ENTITY_SCRIPT_sub0(iVar8, 0, &partyCharacterBuffers[0], 1, 0, 0, 1);
		psVar6 = pCurrentFieldScriptActor;
		pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 1;
		puVar1 = &pCurrentFieldScriptActor->m4_flags;
		breakCurrentScript = 0;
		puVar2 = &pCurrentFieldScriptActor->mCC_scriptPC;
		currentFieldActorId = iVar4;
		pCurrentFieldScriptActor = psVar5;
		pCurrentFieldEntity = psVar7;
		*puVar2 = uVar3;
		psVar6->m4_flags = *puVar1 | 0x100000;
		psVar6->m0_flags = psVar6->m0_flags | 0x20000;
		psVar6->m4_flags = psVar6->m4_flags | 0x400;
		*piVar9 = 0xff;
	}
	currentParty[param_1] = 0xff;
	asyncPartyCharacterLoadingTable[param_1] = 0xff;
	return;
}

int getGearForCharacter(int param_1)
{
	return pKernelGameState->m294[param_1].m78_partyData_gearNum;
}

void startPartyCharacterASyncLoading(int partyCharacter, int partySlot)
{
	size_t sVar1;
	void* pvVar2;
	int iVar3;
	int iVar4;

	asyncPartyCharacterLoadingCharacterIndex = partyCharacter;
	asyncPartyCharacterLoadingIndex = partySlot;
	setCurrentDirectory(4, 0);
	if (fieldExecuteVar1 == 0) {
		waitReadCompletion(0);
	}
	iVar3 = partyCharacter + 5;
	if ((fieldMapNumber & 0xc000) == 0) {
		sVar1 = getFileSizeAligned(iVar3);
		asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = partyCharacter;

		asyncPartyCharacterLoadingBuffer.resize(sVar1);
	}
	else {
		iVar3 = getGearForCharacter(partyCharacter);
		iVar4 = iVar3 + 0x10;
		if (iVar3 == 0xff) {
			iVar4 = 0x10;
		}
		iVar3 = iVar4 + 5;
		sVar1 = getFileSizeAligned(iVar3);
		asyncPartyCharacterLoadingBuffer.resize(sVar1);
		asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = iVar4;
	}
	readFile(iVar3, asyncPartyCharacterLoadingBuffer, 0, 0x80);
	if (fieldExecuteVar1 == 0) {
		waitReadCompletion(0);
	}
	asyncLoadingVar1 = 1;
	return;
}

void setCurrentActor2DPosition(int posX, int posZ)
{
	std::array<VECTOR, 4> alStack136;
	std::array<SVECTOR, 4> auStack72;
	for (int i = 0; i < numWalkMesh; i++)
	{
		pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(posX, posZ, i, &auStack72[i], &alStack136[i]);
	}

	pCurrentFieldScriptActor->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pCurrentFieldScriptActor);
	pCurrentFieldScriptActor->m50 = alStack136[pCurrentFieldScriptActor->m10_walkmeshId];


	actorArray[currentFieldActorId].mC_matrix.t[0] = posX;
	actorArray[currentFieldActorId].m2C_matrixBackup.t[0] = posX;

	actorArray[currentFieldActorId].mC_matrix.t[1] = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;
	actorArray[currentFieldActorId].m2C_matrixBackup.t[1] = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;

	actorArray[currentFieldActorId].mC_matrix.t[2] = posZ;
	actorArray[currentFieldActorId].m2C_matrixBackup.t[2] = posZ;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m84 = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;

	pCurrentFieldScriptActor->m20_position.vx = posX << 16;
	pCurrentFieldScriptActor->m20_position.vz = posZ << 16;
	pCurrentFieldScriptActor->m20_position.vy = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy << 16;
	pCurrentFieldScriptActor->m72_elevation = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = pCurrentFieldScriptActor->m20_position.vx;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = pCurrentFieldScriptActor->m20_position.vy;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = pCurrentFieldScriptActor->m20_position.vz;

	pCurrentFieldScriptActor->m40.vx = 0;
	pCurrentFieldScriptActor->m40.vy = 0;
	pCurrentFieldScriptActor->m40.vz = 0;

	pCurrentFieldScriptActor->m30_stepVector.vx = 0;
	pCurrentFieldScriptActor->m30_stepVector.vy = 0;
	pCurrentFieldScriptActor->m30_stepVector.vz = 0;

	pCurrentFieldScriptActor->mD0_targetPositionOffset.vx = 0;
	pCurrentFieldScriptActor->mD0_targetPositionOffset.vy = 0;
	pCurrentFieldScriptActor->mD0_targetPositionOffset.vz = 0;

	pCurrentFieldScriptActor->m60[0] = 0;
	pCurrentFieldScriptActor->m60[1] = 0;
	pCurrentFieldScriptActor->m60[2] = 0;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vx = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vy = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vz = 0;

	pCurrentFieldScriptActor->mF0 = 0;
	pCurrentFieldScriptActor->mEC_elevation = 0;
	pCurrentFieldScriptActor->m72_elevation = pCurrentFieldScriptActor->m20_position.vy >> 0x10;
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffbffff | 0x400000;
}


const std::array<u16, 8> actorDirectionTable = {
	0x8C00,  0x8E00,  0x8000,  0x8200,
	0x8400,  0x8600,  0x8800,  0x8A00
};

const std::array<u16, 8> actorDirectionTable3 = {
	0x8C00,  0x8400,  0x8800,  0x8000,
	0x8A00,  0x8E00,  0x8600,  0x8200
};

int getCharacter(int param_1)
{
	int iVar1;

	iVar1 = currentParty[2];
	if ((((param_1 != 0xff) && (iVar1 = currentParty[1], param_1 != 0xfe)) && (iVar1 = currentParty[0], param_1 != 0xfd)) && (iVar1 = 0xff, param_1 != 0xfc)) {
		iVar1 = param_1;
	}
	return iVar1;
}

int findCharacterInParty(int param_1)
{
	int iVar1;
	int* piVar2;

	iVar1 = 0;
	if (param_1 != 0xff) {
		do {
			if (currentParty[iVar1] == 0xff) {
				return 0xffffffff;
			}
			if (currentParty[iVar1] == param_1) {
				return iVar1;
			}
			iVar1 = iVar1 + 1;
		} while (iVar1 < 3);
	}
	return 0xffffffff;
}

void updateScriptActor3dRotation(int index)
{
	MissingCode();
}

int spriteWalkToPositionOrActor(int param_1)
{
	if ((actorArray[currentFieldActorId].m4C_scriptEntity->m4_flags & 0x2000) == 0)
	{
		if (actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 == 0)
		{
			actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 = 0x4000000 / pCurrentFieldScriptActor->m76;
		}
	}
	else
	{
		actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 = 0x8000000 / pCurrentFieldScriptActor->m76;
	}

	int local_30;
	int local_28;

	int lVar13 = 0;
	int lVar14 = 0;

	switch (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode)
	{
	case 0:
		local_30 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
		local_28 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
		break;
	case 1:
		local_30 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]) + pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
		local_28 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]) + pCurrentFieldScriptActor->mD0_targetPositionOffset[2];
		break;
	case 2:
		if (readCharacter(1) == 0xFF)
		{
			return 0;
		}
		lVar14 = length1d(actorArray[readCharacter(1)].m4C_scriptEntity->m18[3] + pCurrentFieldScriptActor->m18[3]);
		local_30 = actorArray[readCharacter(1)].m4C_scriptEntity->m20_position.vx >> 16;
		local_28 = actorArray[readCharacter(1)].m4C_scriptEntity->m20_position.vz >> 16;
		if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == playerControlledEntity)
		{
			pCurrentFieldScriptActor->m0_flags |= 0x200000;
		}
		break;
	case 3:
		local_30 = pCurrentFieldScriptActor->mD0_targetPositionOffset[0] + ((getAngleSin(getImmediateOrVariableUnsigned(1) & 0xfff) << 0xc) >> 0xc);
		local_28 = pCurrentFieldScriptActor->mD0_targetPositionOffset[2] + (getAngleCos(getImmediateOrVariableUnsigned(1) & 0xfff) * -0x1000 >> 0xc);
		break;
	default:
		assert(0);
	}

	sVec3 targetPosition;
	targetPosition[0] = local_30 - (pCurrentFieldScriptActor->m20_position.vx >> 16);
	targetPosition[2] = local_28 - (pCurrentFieldScriptActor->m20_position.vz >> 16);
	targetPosition[1] = 0;

	pCurrentFieldScriptActor->m0_flags |= 0x400000;

	// Reached destination yet?
	if ((pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0) ||
		(length2d(targetPosition[0], targetPosition[2]) <= length1d(actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 >> 0xf) + 1 + lVar14))
	{
		//reached location
		if (param_1 == 0)
		{
			pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = fp_atan2(targetPosition);
		}
		else
		{
			if ((pCurrentFieldScriptActor->m0_flags & 0x8000) == 0)
			{
				pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m106_currentRotation | 0x8000;
			}
			else
			{
				pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m11C | 0x8000;
			}
		}

		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xFFFF;
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
		pCurrentFieldScriptActor->m0_flags &= 0xfddff7ff;
		return 0;
	}
	else
	{
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0--;
		pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = fp_atan2(targetPosition);
		breakCurrentScript = 1;
		return -1;
	}
}

void resetFieldScriptActor()
{
	breakCurrentScript = 1;
	pCurrentFieldScriptActor->m30_stepVector.vx = 0;
	pCurrentFieldScriptActor->m30_stepVector.vy = 0;
	pCurrentFieldScriptActor->m30_stepVector.vz = 0;
	pCurrentFieldScriptActor->m40.vx = 0;
	pCurrentFieldScriptActor->m40.vy = 0;
	pCurrentFieldScriptActor->m40.vz = 0;
	pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation | 0x8000;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vx = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vz = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 = 0;
}


void SetGeomScreen(s32 h)
{
	setCopControlWord(2, 0xd000, h);
}

void SetGeomOffset(int ofx, int ofy)
{
	setCopControlWord(2, 0xc000, ofx << 0x10);
	setCopControlWord(2, 0xc800, ofy << 0x10);
}

s32 opA0_var0 = 0;
s32 opA0_var1 = 0;

s32 desiredCameraPosition[3] = { 0,0,0 };

s32 cameraTargetOverride[3] = { 0,0,0 };
s32 desiredCameraTarget[3] = { 0,0,0 };
s32 cameraPositionOverride[3] = { 0,0,0 };
s32 cameraInterpolationTargetStep[3] = { 0,0,0 };
s32 cameraInterpolationTargetStartPosition[3] = { 0,0,0 };
s32 cameraInterpolationPositionStep[3] = { 0,0,0 };
s32 cameraInterpolationStartPosition[3] = { 0,0,0 };
s32 cameraEye[3] = { 0,0,0 };
s32 cameraAt[3] = { 0,0,0 };
VECTOR cameraEye2;
VECTOR cameraAt2;
u16 cameraInterpolationFlags = 0;
s16 cameraInterpolationTargetNumSteps = 0;
s32 cameraInterpolationPositionNumSteps = 0;

s16 camera2Tan = 0;
void setCurrentActorRotation2(s16 param_1)
{
	int iVar1;
	sFieldScriptEntity* psVar2;
	ushort uVar3;

	psVar2 = pCurrentFieldScriptActor;
	iVar1 = fieldExecuteVar1;
	uVar3 = param_1 - camera2Tan & 0xfffU | 0x8000;
	pCurrentFieldScriptActor->m104_rotation = uVar3;
	psVar2->m106_currentRotation = uVar3;
	if (iVar1 == 0) {
		psVar2->m108_rotation3 = uVar3;
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}


int getCurrentActorRotation()
{
	return (pCurrentFieldScriptActor->m106_currentRotation + 0x100 >> 9) + 2U & 7;
}

void setCurrentActorTargetRotation(s16 param_1)
{
	if (fieldExecuteVar1 == 0) {
		pCurrentFieldScriptActor->m104_rotation = param_1 | 0x8000;
		pCurrentFieldScriptActor->m106_currentRotation = param_1 | 0x8000;
		pCurrentFieldScriptActor->m108_rotation3 = param_1 | 0x8000;
	}
	pCurrentFieldScriptActor = pCurrentFieldScriptActor;
	pCurrentFieldScriptActor->m104_rotation = param_1 | 0x8000U;
	pCurrentFieldScriptActor->m106_currentRotation = param_1 | 0x8000U;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}


int playMusicAuthorized = 0;
int musicVar1 = -1;
int musicVar2 = 0;
int currentlyPlayingMusic = 255;

void clearMusic()
{
	MissingCode();
}

void clearMusic2()
{
	MissingCode();
}

int isLoadCompleted()
{
	MissingCode();
	return 0;
}

void playMusic(int musicId, int)
{
	MissingCode();
}

s32 fieldExecuteVar2 = 0;
s32 fieldExecuteVar3 = 0;
s16 fieldRandomBattleVar = 0;


void setCurrentActorElevation(short param_1)
{
	sFieldScriptEntity* psVar1;

	psVar1 = pCurrentFieldScriptActor;
	(pCurrentFieldScriptActor->m20_position).vy = (uint)param_1 << 0x10;
	psVar1->mEC_elevation = param_1;
	psVar1->m72_elevation = param_1;
	return;
}




s16 OPX47Var = 0;
void OPX_47()
{
	OPX47Var = getImmediateOrVariableUnsigned(1);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

s32 load2dAnimVar = 0;
s32 loadCompleted = 0;


void setupSpecialAnimation(sFieldEntitySub4* param_1, sFieldActorSetupParams* param_2)
{
	param_1->m4C_specialAnimation = param_2;
	return;
}




void updateGearState(int param_1)

{
	ushort uVar2;
	int uVar1;

	uVar2 = (ushort)fieldMapNumber;
	if (param_1 == 1) {
		setVar(0x30, uVar2 & 0xfff);
		setVar(0x32, 0);
		uVar1 = 0x34;
	}
	else {
		if (param_1 < 2) {
			if (param_1 != 0) {
				return;
			}
			setVar(0x2a, uVar2 & 0xfff);
			setVar(0x2c, 0);
			uVar1 = 0x2e;
		}
		else {
			if (param_1 != 2) {
				return;
			}
			setVar(0x36, uVar2 & 0xfff);
			setVar(0x38, 0);
			uVar1 = 0x3a;
		}
	}
	setVar(uVar1, 0);
	return;
}

u8 OPX_50Param = 0;
u8 compassDisabled = 0;
u16 OPX_80Params[8] = { 0,0,0,0,0,0,0,0 };
s32 OPX_81Params[3] = { 0,0,0 };
s8 OPX_82Param0[4] = { 0,0,0,0 };
s8 OPX_82Param1[4] = { 0,0,0,0 };
s8 OPX_82Param2[4] = { 0,0,0,0 };
s16 OPX_82Param3[3] = { 0,0,0 };
s16 OPX_82Param4 = 0;
s32 OPX_86Param = 0;
s32 OPX8E_param0 = 0;
s32 OPX8E_param1 = 0;

int getCurrentDiscNumber()
{
	MissingCode();
	return 0;
}



s8 OPX_E0Param = 0;

void OPX_13Sub(int)
{
	MissingCode();
}


sVec2_s16 padButtonForScripts[2];


int isScriptAlreadyRunning(sFieldScriptEntity* pEntity, int scriptIndex)
{
	for (int i = 0; i < 8; i++)
	{
		if (pEntity->m8C_scriptSlots[i].m3_scriptIndex == scriptIndex)
		{
			return -1;
		}
	}
	return 0;
}


void setupRGBCalcMode(int, int, int, int, int, int)
{
	MissingCode();
}

void executeFieldScript(int param)
{
	breakCurrentScript = 0;
	fieldExectuteMaxCycles = param;
	int cycles = 0;
	if (param > 0)
	{
		while (cycles <= 0x400)
		{
			//printf("PC: 0x%04X ", pCurrentFieldScriptActor->mCC_scriptPC);
			u8 opcodeId = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC];

			LogOpcode(currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, opcodeId);

			if (fieldScriptOpcodes[opcodeId] == nullptr)
			{
				LogMissingOpcode(currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, opcodeId);
				breakCurrentScript = 1;
				return;
			}

			//_CrtCheckMemory();
			fieldScriptOpcodes[opcodeId]();
			//_CrtCheckMemory();

			if (currentScriptFinished == 0) {
				fieldExectuteMaxCycles = 0xffff;
			}
			if (fieldExecuteVar1 != 0) {
				if (fieldExecuteVar2 == 0) {
					return;
				}
				if (fieldExecuteVar3 == 0) {
					return;
				}
				if (fieldChangePrevented == 0) {
					return;
				}
			}
			if ((breakCurrentScript == 1) && (currentScriptFinished == 1)) {
				return;
			}

			cycles++;
			if (cycles >= fieldExectuteMaxCycles)
			{
				return;
			}
		}

		if (fieldDebugDisable == 0) {
			sprintf_screen("EVENTLOOP ERROR ACT=%d\n", currentFieldActorId);
		}
	}
}

void startAllEntityScripts()
{
	if (fieldScriptEntityAlreadyInitialized == 0)
	{
		setVar(0x10, 0);
		setVarsForCurrentParty();

		for (int i = 0; i < totalActors; i++)
		{
			// that was probably some macro
			pCurrentFieldEntity = &actorArray[i];
			pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
			currentFieldActorId = i;
			pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 2); // the update script

			// Does the entry point have any code?
			if ((READ_LE_U16(pCurrentFieldScriptFile + pCurrentFieldScriptActor->mCC_scriptPC) == 0))
			{
				pCurrentFieldScriptActor->m4_flags |= 0x4000000;
			}

			// again, this time for the init script
			pCurrentFieldEntity = &actorArray[i];
			pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
			currentFieldActorId = i;
			pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 0); // the init script

		}

		// execute the init script
		for (int i = 0; i < totalActors; i++)
		{
			pCurrentFieldEntity = &actorArray[i];
			pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
			fieldScriptInitVar0 = 0;
			currentScriptFinished = 0;
			currentFieldActorId = i;

			executeFieldScript(0xFFFF);

			if (fieldScriptInitVar0 == 0)
			{
				OP_INIT_ENTITY_SCRIPT_sub0(i, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 0);
				pCurrentFieldScriptActor->m4_flags |= 0x800;
			}
		}
	}
	else
	{
		assert(0);
	}
}

struct sShapeTransfert
{
	RECT m0_rect;
	sPS1Pointer m8_pData;
	sShapeTransfert* mC_pNext;
	// size 0x10
};

int shapeTransfertTableSize = 0;
sShapeTransfert* shapeTransfertBuffer[2] = { nullptr, nullptr };
void* shapeTransfertTemporaryBuffersLinkedLists[2] = { nullptr, nullptr };

sShapeTransfert* shapeTransfertTableCurrentEntry;
int shapeTransfertDoubleBufferIndex = 0;
sShapeTransfert* shapeTransfertTableStart;
sShapeTransfert* shapeTransfertTableEnd;
std::array<sShapeTransfert*, 2>shapeTransfertTable;

void resetSpriteTransfertList()
{
	spriteTransfertListHead = nullptr;
}

void allocateShapeTransfert(int param_1)
{
	shapeTransfertTableSize = param_1;
	shapeTransfertBuffer[0] = new sShapeTransfert[param_1];
	shapeTransfertBuffer[1] = new sShapeTransfert[param_1];
	shapeTransfertTemporaryBuffersLinkedLists[1] = nullptr;
	shapeTransfertTemporaryBuffersLinkedLists[0] = nullptr;
	shapeTransfertTable[0] = nullptr;
	resetSpriteTransfertList();
	return;
}

void addToShapeTransfertTable(sPS1Pointer pData, short x, short y, short w, short h)
{
	sShapeTransfert** ppsVar1;
	int iVar2;
	sShapeTransfert* psVar3;

	psVar3 = shapeTransfertTableCurrentEntry;
	iVar2 = shapeTransfertDoubleBufferIndex;
	if (shapeTransfertTableCurrentEntry + 1 < shapeTransfertTableEnd) {
		shapeTransfertTableCurrentEntry->m0_rect.x = x;
		shapeTransfertTableCurrentEntry->m0_rect.y = y;
		shapeTransfertTableCurrentEntry->m0_rect.w = w;
		shapeTransfertTableCurrentEntry->m0_rect.h = h;
		shapeTransfertTableCurrentEntry->m8_pData = pData;
		ppsVar1 = &shapeTransfertTableCurrentEntry->mC_pNext;
		shapeTransfertTableCurrentEntry = shapeTransfertTableCurrentEntry + 1;
		*ppsVar1 = shapeTransfertTable[iVar2];
		shapeTransfertTable[iVar2] = psVar3;
	}
	return;
}

struct sRGB32
{
	s32 R;
	s32 G;
	s32 B;
};

struct sScreenEffectSlot
{
	TILE m18_tile[2];
	sRGB32 m38_Current;
	sRGB32 m44_step;
	s16 m50_mode;
	s16 m52_isActive;
	s16 m54_duration;
};

std::array<sScreenEffectSlot, 2> screenEffects;

void setupRGBFaderSlot0_fadeIn(int)
{
	MissingCode();
}

void setupRGBCalcSlot0_fadeToBlack(int)
{
	MissingCode();
}

void resetRGBFaderToBlack(int index)
{
	TILE* p = &screenEffects[index].m18_tile[0];
	SetTile(p);
	SetSemiTrans(p, 1);
	screenEffects[index].m18_tile[0].w = 0x140;
	screenEffects[index].m18_tile[0].h = 0xe0;
	screenEffects[index].m18_tile[0].x0 = 0;
	screenEffects[index].m18_tile[0].y0 = 0;
	screenEffects[index].m18_tile[1] = screenEffects[index].m18_tile[0];

	screenEffects[index].m52_isActive = 0;
	screenEffects[index].m54_duration = 0;
	screenEffects[index].m38_Current.B = 0;
	screenEffects[index].m38_Current.G = 0;
	screenEffects[index].m38_Current.R = 0;
	screenEffects[index].m50_mode = 2;
}

void resetAllRGBFadersToBlack()
{
	resetRGBFaderToBlack(0);
	resetRGBFaderToBlack(1);
}

void flagAllocation(std::vector<u8>&)
{
	MissingCode();
}

void unflagAllocation(std::vector<u8>&)
{
	MissingCode();
}

void setupObjectRenderModesSub0(sFieldEntitySub0* param_1)
{
	short sVar1;
	short sVar2;
	sModelBlock* psVar3;
	int iVar4;
	int iVar5;
	int iVar6;
	int iVar7;

	psVar3 = &(*param_1->m4_pModelBlock);
	sVar1 = psVar3->m22;
	sVar2 = psVar3->m24;
	iVar6 = (int)psVar3->m28 - (int)psVar3->m20;
	iVar5 = (int)psVar3->m2A - (int)sVar1;
	iVar4 = (int)psVar3->m2C - (int)sVar2;
	iVar7 = iVar6;
	if (iVar6 < iVar5) {
		iVar7 = iVar5;
	}
	if (iVar7 < iVar4) {
		iVar7 = iVar4;
	}
	param_1->m18.vx = (short)(iVar6 / 2) + psVar3->m20;
	param_1->m18.vy = (short)(iVar5 / 2) + sVar1;
	param_1->m18.vz = (short)(iVar4 / 2) + sVar2;
	param_1->m20 = (short)iVar7 * 2 + 1;
}

void setupObjectRenderModes()
{
	for (int i=0; i<totalObjects; i++)
	{
		sFieldEntity* pFieldEntity = &actorArray[i];
		if ((pFieldEntity->m58_flags & 0x40) == 0) {
			setupObjectRenderModesSub0(pFieldEntity->m0);
			if (isFogSetup == 0) {
				if ((pFieldEntity->m58_flags & 0xc) == 0) {
					if ((pFieldEntity->m58_flags & 0x4000) == 0) {
						if ((pFieldEntity->m58_flags & 0x10) == 0) {
							pFieldEntity->m0->m12_renderMode = 0;
						}
						else {
							pFieldEntity->m0->m12_renderMode = 2;
						}
					}
					else {
						pFieldEntity->m0->m12_renderMode = 3;
					}
				}
				else {
					pFieldEntity->m0->m12_renderMode = 1;
				}
			}
			else {
				if ((pFieldEntity->m58_flags & 0x10) == 0) {
					pFieldEntity->m0->m12_renderMode = 4;
				}
				else {
					pFieldEntity->m0->m12_renderMode = 5;
				}
			}
		}
	}
}

SVECTOR renderModelRotationAngles;
MATRIX renderModelRotationMatrix;
s32 cameraDeltaTan;

MATRIX currentProjectionMatrix;

s32 updateCameraInterpolationVar0 = 0;
s32 updateCameraInterpolationVar1 = 0;

void setPolyUV(POLY_FT4* poly, ushort u0, ushort v0, ushort u1, ushort v1, ushort u2, ushort v2, ushort u3, ushort v3)
{
	if ((int)((uint)u0 << 0x10) < 0) {
		u0 = 0;
	}
	if ((int)((uint)u1 << 0x10) < 0) {
		u1 = 0;
	}
	if ((int)((uint)u2 << 0x10) < 0) {
		u2 = 0;
	}
	if ((int)((uint)u3 << 0x10) < 0) {
		u3 = 0;
	}
	if ((int)((uint)v0 << 0x10) < 0) {
		v0 = 0;
	}
	if ((int)((uint)v1 << 0x10) < 0) {
		v1 = 0;
	}
	if ((int)((uint)v2 << 0x10) < 0) {
		v2 = 0;
	}
	if ((int)((uint)v3 << 0x10) < 0) {
		v3 = 0;
	}
	if (0xff < (short)u0) {
		u0 = 0xff;
	}
	if (0xff < (short)u1) {
		u1 = 0xff;
	}
	if (0xff < (short)u2) {
		u2 = 0xff;
	}
	if (0xff < (short)u3) {
		u3 = 0xff;
	}
	if (0xff < (short)v0) {
		v0 = 0xff;
	}
	if (0xff < (short)v1) {
		v1 = 0xff;
	}
	if (0xff < (short)v2) {
		v2 = 0xff;
	}
	if (0xff < (short)v3) {
		v3 = 0xff;
	}
	poly->u0 = u0;
	poly->v0 = v0;
	poly->u1 = u1;
	poly->v1 = v1;
	poly->u2 = u2;
	poly->v2 = v2;
	poly->u3 = u3;
	poly->v3 = v3;
}

void initFieldData()
{
	resetFieldDefault();

	for (int i = 0; i < 32; i++)
	{
		fieldVramMapping[i].m0_vramX = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 0);
		fieldVramMapping[i].m2_vramY = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 2);
		fieldVramMapping[i].m4 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 4);
		fieldVramMapping[i].m6 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 6);
	}

	MissingCode();

	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			initCompassPoly(&fieldCompassVar2[i*4+j], j, i, 0); // main compass
		}
	}

	initCompassPoly(&fieldCompassVar2[0x10], 4, 4, 1); // letters
	initCompassPoly(&fieldCompassVar2[0x11], 5, 5, 1);
	initCompassPoly(&fieldCompassVar2[0x12], 6, 6, 1);
	initCompassPoly(&fieldCompassVar2[0x13], 7, 7, 1);

	initCompassPoly(&fieldCompassVar2[0x14], 8, 8, 1); // arrow

	initCompassShadowPoly();

	{
		{

			int rawFieldImageBundleSize = READ_LE_U32(&rawFieldBundle[0x10C]);
			rawFieldImageBundle.resize(rawFieldImageBundleSize + 0x10);
			fieldDecompress(rawFieldImageBundleSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x130]), rawFieldImageBundle);

			int numImages = READ_LE_U32(rawFieldImageBundle.begin());
			for (int i = 0; i < numImages; i++)
			{
				int offset = READ_LE_U32(rawFieldImageBundle.begin() + i * 4);
				uploadFieldImages(rawFieldImageBundle.begin() + offset);
			}
		}

		{
			int rawFieldImageBundle2Size = READ_LE_U32(&rawFieldBundle[0x11C]);
			rawFieldImageBundle2.resize(rawFieldImageBundle2Size + 0x10);
			fieldDecompress(rawFieldImageBundle2Size + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x140]), rawFieldImageBundle2);

			int numImages = READ_LE_U32(rawFieldImageBundle2.begin());
			for (int i = 0; i < numImages; i++)
			{
				int offset = READ_LE_U32(rawFieldImageBundle2.begin() + 4 + i * 4);

				if (fieldVramMapping[i].m6 == 0)
				{
					int x = fieldVramMapping[i].m0_vramX;
					int y = fieldVramMapping[i].m2_vramY;

					uploadNpcSpriteSheet(rawFieldImageBundle2.begin() + offset, x, y);
				}

			}
		}

		DrawSync(0); // needs to be done to transfert to vram before the bundles get deallocated
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x114]);
		rawFieldModels.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x138]), rawFieldModels);

		int numModels = READ_LE_U32(rawFieldModels.begin());
		gCurrentFieldModels.clear();
		gCurrentFieldModels.resize(numModels);
		for (int i = 0; i < numModels; i++)
		{
			int offset = READ_LE_U32(rawFieldModels.begin() + 4 + i * 4);
			int nextOffset;

			if (i < numModels - 1)
			{
				nextOffset = READ_LE_U32(rawFieldModels.begin() + 4 + (i + 1) * 4);
			}
			else
			{
				nextOffset = rawFieldModels.size();
			}
			fieldModelRelocation(rawFieldModels.begin() + offset);
			gCurrentFieldModels[i].init(rawFieldModels.begin() + offset, nextOffset - offset);
		}
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x120]);
		rawFieldScriptData.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x144]), rawFieldScriptData);
		totalActors = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
		pCurrentFieldScriptFile = rawFieldScriptData.begin() + 0x84 + totalActors * 0x40;
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x12C]);
		rawFieldTriggerData.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x150]), rawFieldTriggerData);
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x128]);
		rawFieldDialogBundle.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x14C]), rawFieldDialogBundle);
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x110]);
		rawFieldWalkMesh.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x134]), rawFieldWalkMesh);
		walkMesh.init(rawFieldWalkMesh);
	}

	{
		std::vector<u8>::iterator walkMeshIterator = rawFieldWalkMesh.begin();
		numWalkMesh = READ_LE_U32(walkMeshIterator);
		assert(numWalkMesh < 5);
		walkMeshIterator += 4;

		for (int i = 0; i < 4; i++)
		{
			u32 block_size = READ_LE_U32(walkMeshIterator);
			assert((block_size % 0xE) == 0);
			walkMeshNumTrianglePerBlock[i] = block_size / 0xE;

			walkMeshIterator += 4;
		}

		int iVar4 = 0;
		walkMeshVar1Raw = &(rawFieldWalkMesh[0]) + READ_LE_U32(walkMeshIterator);
		walkMeshIterator += 4;

		if (numWalkMesh > 0) {
			std::array<u8*, 5>::iterator piVar11 = walkMeshVerticesRaw.begin();
			std::array<u8*, 5>::iterator ppuVar8 = walkMeshTriangleRaw.begin();
			do {
				iVar4 = iVar4 + 1;
				*ppuVar8 = &rawFieldWalkMesh[0] + READ_LE_U32(walkMeshIterator);
				*piVar11 = &rawFieldWalkMesh[0] + READ_LE_U32(walkMeshIterator + 4);
				ppuVar8++;
				piVar11++;
				walkMeshIterator = walkMeshIterator + 2 * 4;
			} while (iVar4 < numWalkMesh);
		}
		walkMeshVar4 = (walkMeshTriangleRaw[0] - walkMeshVar1Raw) / 4;

		//////////////////////////////////////////////
		// Not in disassembly

		for (int i = 0; i < numWalkMesh; i++)
		{
			walkMeshTriangle[i] = &walkMesh.m_blocks[i].m_triangles;
			walkMeshVertices[i] = &walkMesh.m_blocks[i].m_vertices;
		}
		walkMeshVar1 = &walkMesh.m_materials;
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x118]);
		rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x13C]), rawFieldActorSetupParams);

		u32 count = READ_LE_U32(rawFieldActorSetupParams.begin());
		fieldActorSetupParams.resize(count);
		for (int i = 0; i < count; i++)
		{
			fieldActorSetupParams[i].init(rawFieldActorSetupParams.begin() + READ_LE_U32(4 + rawFieldActorSetupParams.begin() + 4 * i));
		}
	}

	fieldInitVar1[0] = 1;
	fieldInitVar1[1] = 1;
	fieldInitVar1[2] = 1;
	fieldInitVar1[3] = 1;

	setupField3d(rawFieldBundle.begin() + 0x154);

	totalObjects = READ_LE_U16(&rawFieldBundle[0x18C]);
	actorArray.resize(totalObjects);

	for (int i = 0; i < totalObjects; i++)
	{
		memset(&actorArray[i], 0, sizeof(sFieldEntity));
	}

	std::vector<u8>::iterator fieldEntitySetup = rawFieldBundle.begin() + 0x190;
	for (int i = 0; i < totalObjects; i++)
	{
		actorArray[i].m58_flags = READ_LE_U16(fieldEntitySetup);
		actorArray[i].m50_modelRotation.vx = READ_LE_S16(fieldEntitySetup + 2);
		actorArray[i].m50_modelRotation.vy = READ_LE_S16(fieldEntitySetup + 4);
		actorArray[i].m50_modelRotation.vz = READ_LE_S16(fieldEntitySetup + 6);

		actorArray[i].m2C_matrixBackup.t[0] = actorArray[i].mC_matrix.t[0] = READ_LE_S16(fieldEntitySetup + 8);
		actorArray[i].m2C_matrixBackup.t[1] = actorArray[i].mC_matrix.t[1] = READ_LE_S16(fieldEntitySetup + 10);
		actorArray[i].m2C_matrixBackup.t[2] = actorArray[i].mC_matrix.t[2] = READ_LE_S16(fieldEntitySetup + 12);

		// Is this a 3d model?
		if (!(actorArray[i].m58_flags & 0x40))
		{
			u16 modelIndex = READ_LE_U16(fieldEntitySetup + 14);

			actorArray[i].m0 = new sFieldEntitySub0;
			sModel& pModel = gCurrentFieldModels[modelIndex];
			std::vector<sModelBlock>::iterator pModelBlock = pModel.m10_blocks.begin();

			actorArray[i].m0->m4_pModelBlock = pModelBlock;

			initModel1(*pModelBlock, actorArray[i].m0->m8[0], actorArray[i].m0->m8[1]);
			initModel2(&(*actorArray[i].m0->m4_pModelBlock), actorArray[i].m0->m8[0], (actorArray[i].m58_flags & 0xC) >> 2);
			actorArray[i].m0->m8[1] = actorArray[i].m0->m8[0];
			if (actorArray[i].m58_flags & 0x2000)
			{
				initModel3(3, 0);
				actorArray[i].m0->m14 = initModelDynamicVertices(&(*actorArray[i].m0->m4_pModelBlock));
				initModel3(8, 0);
			}
			initModel5(&(*actorArray[i].m0->m4_pModelBlock));
		}
		else
		{
			actorArray[i].m58_flags |= 0x20;
			actorArray[i].m50_modelRotation.vx = 0;
			actorArray[i].m50_modelRotation.vy = 0;
			actorArray[i].m50_modelRotation.vz = 0;

		}

		initFieldScriptEntity(i);

		fieldEntitySetup += 16;
	}

	MissingCode();
	
	initDialogWindows();
	resetAllRGBFadersToBlack();

	unflagAllocation(rawFieldBundle);
	rawFieldBundleForDebug = rawFieldBundle;
	rawFieldBundle.clear();

	initModel3(5, 0);

	allocateShapeTransfert(0x3c00);

	MissingCode();

	OPX_80Params[2] = 0x140;
	OPX_80Params[7] = 0;
	OPX_82Param2[1] = 0;
	OPX_82Param2[0] = 0;
	OPX_82Param1[2] = 0;
	OPX_82Param1[1] = 0;
	OPX_82Param1[0] = 0;
	OPX_82Param0[2] = 0;
	OPX_82Param0[1] = 0;
	OPX_82Param0[0] = 0;
	OPX_81Params[0] = 0;
	OPX_81Params[2] = 0x1000;
	OPX_82Param3[2] = 0;
	OPX_82Param3[1] = 0;
	OPX_82Param3[0] = 0;
	OPX_80Params[6] = 0;
	OPX_80Params[5] = 0;
	OPX_80Params[4] = 0;
	OPX_80Params[3] = 0;
	OPX_80Params[1] = 0;
	OPX_80Params[0] = 0;
	OPX_82Param4 = 0;
	OPX_81Params[1] = 0;
	OPX_82Param2[2] = 0x20;
	fieldExecuteVar1 = 0;
	startAllEntityScripts();
	fieldExecuteVar1 = 1;
	createRotationMatrix(&renderModelRotationAngles, &renderModelRotationMatrix);
	renderModelRotationMatrix.t[2] = 0;
	renderModelRotationMatrix.t[1] = 0;
	renderModelRotationMatrix.t[0] = 0;

	if (OPX_82Param4 != 0)
	{
		MissingCode();
	}
	initModel3(8, 0);

	cameraAt2.vx = actorArray[actorCameraTracked].mC_matrix.t[0] << 16;
	cameraAt2.vy = actorArray[actorCameraTracked].mC_matrix.t[1] << 16;
	cameraAt2.vz = actorArray[actorCameraTracked].mC_matrix.t[2] << 16;

	for (int i=0; i<totalObjects; i++)
	{
		createRotationMatrix(&actorArray[i].m50_modelRotation, &actorArray[i].mC_matrix);
		actorArray[i].m2C_matrixBackup = actorArray[i].mC_matrix;
	}
	MissingCode();

	fieldIdForDebugger = currentFieldId0 / 2; // hack!

	currentFieldId1 = -1;
	currentFieldId0 = -1;
	setupObjectRenderModes();
	MissingCode();


}

int isCDBusy()
{
	MissingCode();
	return 0;
}



void loadRawFieldBundle(int fieldId)
{
	rawFieldBundle.resize(getFileSizeAligned(184 + fieldId));
	flagAllocation(rawFieldBundle);

	readFile(184 + fieldId, rawFieldBundle, 0, 0x80);
}

int loadNewField(int fieldId, int param)
{
	if ((currentFieldId1 == param) && (currentFieldId0 == fieldId))
	{
		return 0;
	}

	if (isCDBusy())
	{
		return -1;
	}

	waitReadCompletion(0);
	if (currentFieldId1 != -1)
	{
		unflagAllocation(rawFieldBundle);
		rawFieldBundleForDebug = rawFieldBundle;
		rawFieldBundle.clear();
	}
	loadRawFieldBundle(fieldId);

	currentFieldId0 = fieldId;
	currentFieldId1 = param;

	return -1;
}

void loadInitialField()
{
	waitReadCompletion(0);

	while (loadNewField((fieldMapNumber & 0xFFF) * 2, 0));
}


void* allocateBufferForVramUpload(int)
{
	MissingCode();
	return nullptr;
}

std::array<u8, 2048 * 512> gVram;
void clearModelTextureCache();

void loadImageFileToVram(int fileId, void* allocation, int, int, int, int, int, int, int, int)
{
	// this is completely different from the original code that was done async and using the cd callbacks
	MissingCode();

	// make sure we recreate all texture after loading this
	clearModelTextureCache();

	std::vector<u8> data;
	c_isoManager::getCurrentDisc()->readData(getFileStartSector(fileId), getFileSize(fileId), data);

	int offset = 0;
	while (offset < data.size() - 0x14)
	{
		std::vector<u8>::iterator texture = data.begin() + offset;
		u32 type = READ_LE_U16(texture + 0x00);
		u16 pos_x = READ_LE_U16(texture + 0x04);
		u16 pos_y = READ_LE_U16(texture + 0x06);
		u16 move_x = READ_LE_U16(texture + 0x08);
		u16 move_y = READ_LE_U16(texture + 0x0a);
		u16 width = READ_LE_U16(texture + 0x0c);
		u16 height = READ_LE_U16(texture + 0x0e);
		u16 chunks = READ_LE_U16(texture + 0x12);

		if (width > 2048 || height > 512 || width == 0 || height == 0) {
			return;
		}
		int blockSize = 0x1C + chunks * 2;
		offset += (blockSize + 2047) & ~2047;
		for (int i = 0; i < chunks; i++) {
			height = READ_LE_U16(texture + 0x1C + i * 2);
			for (int j = 0; j < height; j++) {
				memcpy(&gVram[0] + (pos_y + move_y + j) * 2048 + (pos_x + move_x) * 2, ((u8*)&data[0]) + offset + j * width * 2, width * 2);
			}
			pos_y += height;
			blockSize = width * height * 2;
			offset += (blockSize + 2047) & ~2047;
		}
	}
}

int fieldGraphicsUploaded = 0;
void loadFieldGraphics()
{
	if (!fieldGraphicsUploaded)
	{
		fieldGraphicsUploaded = 1;
		void* fieldGraphicsAllocation = allocateBufferForVramUpload(4);
		loadImageFileToVram((fieldMapNumber & 0xfff) * 2 + 0xb9, fieldGraphicsAllocation, 0, 0, 0, 0, 0, 0, 0, 0);
	}
}

void setFieldDrawEnvClip(short x, short y, short w, short h)
{
	fieldRenderContext[0].m0_drawEnv.clip.x = x;
	fieldRenderContext[0].m0_drawEnv.clip.y = y;
	fieldRenderContext[0].m0_drawEnv.clip.w = w;
	fieldRenderContext[0].m0_drawEnv.clip.h = h;
	fieldRenderContext[1].m0_drawEnv.clip.x = x;
	fieldRenderContext[1].m0_drawEnv.clip.y = y + 0x100;
	fieldRenderContext[1].m0_drawEnv.clip.w = w;
	fieldRenderContext[1].m0_drawEnv.clip.h = h;
}

void setupFieldDisplayEnv(void)
{
	fieldRenderContext[0].mB8_displayEnv.screen.x = 0;
	fieldRenderContext[0].mB8_displayEnv.screen.y = 10;
	fieldRenderContext[0].mB8_displayEnv.screen.w = 0x100;
	fieldRenderContext[0].mB8_displayEnv.screen.h = 0xd8;
	fieldRenderContext[1].mB8_displayEnv.screen.x = 0;
	fieldRenderContext[1].mB8_displayEnv.screen.y = 10;
	fieldRenderContext[1].mB8_displayEnv.screen.w = 0x100;
	fieldRenderContext[1].mB8_displayEnv.screen.h = 0xd8;
}

void initFieldDrawEnvsSub0(int param_1, int param_2)
{
	initFieldDrawEnvsSub0Var0 = param_1;
	initFieldDrawEnvsSub0Var1 = (param_2 + -1) * 0x10000;
	return;
}

void initFieldDrawEnvs()
{
	fieldDrawEnvsInitialized = 1;
	DrawSync(0);
	VSync(0);
	InitGeom();
	SetGeomOffset(0xa0, 0x70);
	SetDefDrawEnv(&fieldRenderContext[0].m0_drawEnv, 0, 0, 0x140, 0xe0);
	SetDefDrawEnv(&fieldRenderContext[1].m0_drawEnv, 0, 0x100, 0x140, 0xe0);
	SetDefDrawEnv(&fieldRenderContext[0].m5C_backgroundRect, 0, 0, 0x140, 0xe0);
	SetDefDrawEnv(&fieldRenderContext[1].m5C_backgroundRect, 0, 0x100, 0x140, 0xe0);
	SetDefDispEnv(&fieldRenderContext[0].mB8_displayEnv, 0, 0x100, 0x140, 0xe0);
	SetDefDispEnv(&fieldRenderContext[1].mB8_displayEnv, 0, 0, 0x140, 0xe0);
	setFieldDrawEnvClip(0, 0, 0x140, 0xe0);
	setupFieldDisplayEnv();
	fieldRenderContext[0].m0_drawEnv.r0 = 0;
	fieldRenderContext[0].m0_drawEnv.g0 = 0;
	fieldRenderContext[0].m0_drawEnv.b0 = 0;
	fieldRenderContext[1].m0_drawEnv.r0 = 0;
	fieldRenderContext[1].m0_drawEnv.g0 = 0;
	fieldRenderContext[1].m0_drawEnv.b0 = 0;
	fieldRenderContext[0].m0_drawEnv.dtd = 1;
	fieldRenderContext[1].m0_drawEnv.dtd = 1;
	VSync(0);
	PutDispEnv(&fieldRenderContext[1].mB8_displayEnv);
	PutDrawEnv(&fieldRenderContext[1].m0_drawEnv);
	initFieldDrawEnvsSub0(0x140, 0xf0);
}

void setupFieldRenderingContext(void)
{
	if (fieldDebugDisable == 0) {
		assert(0);
	}
	g_frameOddOrEven = (g_frameOddOrEven + 1) % 2;
	pCurrentFieldRenderingContext = &fieldRenderContext[g_frameOddOrEven];
	ClearOTagR(&fieldRenderContext[g_frameOddOrEven].m80D4_uiOT[0], 8);
	return;
}

int fieldUseGroundOT = 0;

void clearFieldOrderingTable()
{
	setupFieldRenderingContext();
	ClearOTagR(&pCurrentFieldRenderingContext->mCC_OT[0], 0x1000);
	if (fieldUseGroundOT != 0) {
		ClearOTagR(&pCurrentFieldRenderingContext->m40D0_secondaryOT[0], 0x1000);
	}
}

const std::vector<u8> fontPalettes = {
	{
		 0x0,  0x0, 0xBD, 0xF7,
		0x86, 0xC0, 0xBD, 0xF7,
		 0x0,  0x0, 0xBD, 0xF7,
		0x86, 0xC0, 0xBD, 0xF7,
		 0x0,  0x0, 0xBD, 0xF7,
		0x86, 0xC0, 0xBD, 0xF7,
		 0x0,  0x0, 0xBD, 0xF7,
		0x86, 0xC0, 0xBD, 0xF7,
		 0x0,  0x0,  0x0,  0x0,
		 0x0,  0x0,  0x0,  0x0,
		0xBD, 0xF7, 0xBD, 0xF7,
		0xBD, 0xF7, 0xBD, 0xF7,
		0x86, 0xC0, 0x86, 0xC0,
		0x86, 0xC0, 0x86, 0xC0,
		0xBD, 0xF7, 0xBD, 0xF7,
		0xBD, 0xF7, 0xBD, 0xF7,
	}
};

void initFontPalettes(short param_1, short param_2)
{
	RECT local_18;

	local_18.w = 0x20;
	local_18.h = 1;
	local_18.x = param_1;
	local_18.y = param_2;
	LoadImage(&local_18, &fontPalettes[0]);
	textSpriteMode0 = GetClut((int)param_1, (int)param_2);
	textSpriteMode1 = GetClut(param_1 + 0x10, (int)param_2);
	return;
}

void initFontSystem()
{
	if (fieldDebugDisable == 0) {
		assert(0);
	}

	initFontPalettes(0x100, 0xf0);
}

void bootField()
{
	initFontSystem();

	MissingCode();

	setCurrentDirectory(4, 0);
	loadInitialField();

	MissingCode();

	initFieldDrawEnvs();

	MissingCode();

	g_frameOddOrEven = 1;

	MissingCode();

	clearFieldOrderingTable();

	MissingCode();

	waitReadCompletion(0);
	setCurrentDirectory(4, 0);
	initFieldData();
	loadFieldGraphics();

	MissingCode();

	if (fieldGraphicsUploaded)
	{
		MissingCode();
		fieldGraphicsUploaded = 0;
		MissingCode();
	}

	MissingCode();

	resetInputs();
	fieldMusicLoadPending = 0;

	MissingCode();
}

void allocatePartyCharacterBuffers()
{
	initModel3(8, 0);

	partyCharacterBuffersRaw[0].resize(0x14000);
	partyCharacterBuffersRaw[1].resize(0x14000);
	partyCharacterBuffersRaw[2].resize(0x14000);

	flagAllocation(partyCharacterBuffersRaw[0]);
	flagAllocation(partyCharacterBuffersRaw[1]);
	flagAllocation(partyCharacterBuffersRaw[2]);
}

void waitForReadFinished(void)

{
	int iVar1;

	do {
		iVar1 = isCDBusy();
	} while (iVar1 != 0);
	waitReadCompletion(0);
	return;
}

int characterOrGearsLoadingInProgress = 0;
int fieldRequestedGears = 0;
int typeOfPlayableCharacterLoaded = 0;

void finalizeLoadPlayableCharacters()
{
	if (characterOrGearsLoadingInProgress != 0) {
		waitForReadFinished();

		for (int i = 0; i < 3; i++)
		{
			unflagAllocation(partyCharacterBuffersRaw[i]);
			if (currentParty[i] != -1)
			{
				unflagAllocation(partyCharacterBuffersCompressed[i]);
				decompress(partyCharacterBuffersCompressed[i].begin(), partyCharacterBuffersRaw[i]);
				partyCharacterBuffersCompressed[i].clear();

				partyCharacterBuffers[i].init(partyCharacterBuffersRaw[i].begin());
			}
		}

		characterOrGearsLoadingInProgress = 0;
	}
}

void refinalizePlayableCharacters(int)
{
	MissingCode();
}

void setupFieldCurrentPartyFromKernelAsGears()
{
	MissingCode();
}

struct sLoadingBatchCommands
{
	u16 m0_fileIndex;
	std::vector<u8>* m4_loadPtr;
};

sLoadingBatchCommands playableCharacterLoadingBatchCommands[11];

void batchStartLoadingFiles(sLoadingBatchCommands* pCommands, int param_2)
{
	// TODO: this is a quick and dirty implementation, not how the original code worked
	while (pCommands->m4_loadPtr)
	{
		readFile(pCommands->m0_fileIndex, *pCommands->m4_loadPtr, 0, 0);

		pCommands++;
	}
}

void setupFieldCurrentPartyFromKernelAsCharacters()
{
	pKernelGameState = &gameState;

	for (int i = 0; i < 3; i++)
	{
		currentParty[i] = -1;
		if (gameState.m1D34_currentParty[i] != -1)
		{
			currentParty[i] = gameState.m1D34_currentParty[i];
		}
	}

	int loadSlot = 0;
	for (int i = 0; i < 3; i++)
	{
		if (currentParty[i] != -1)
		{
			playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = currentParty[i] + 5;
			asyncPartyCharacterLoadingTable[i] = currentParty[i];
			partyCharacterBuffersCompressed[i].resize(getFileSizeAligned(currentParty[i] + 5));
			playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = &partyCharacterBuffersCompressed[i];
			flagAllocation(partyCharacterBuffersCompressed[i]);
			loadSlot++;
		}
	}

	playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = 0;
	playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = nullptr;

	batchStartLoadingFiles(playableCharacterLoadingBatchCommands, 0);
	typeOfPlayableCharacterLoaded = 1;
}

void startLoadingPlayableCharacters()
{
	waitForReadFinished();
	if (characterOrGearsLoadingInProgress == 1) {
		finalizeLoadPlayableCharacters();
		if (fieldScriptEntityAlreadyInitialized != 0) {
			return;
		}
	}
	else {
		if (fieldScriptEntityAlreadyInitialized != 0) {
			refinalizePlayableCharacters(0);
			return;
		}
	}
	fieldRequestedGears = (fieldMapNumber & 0xc000) != 0;
	characterOrGearsLoadingInProgress = 0;
	if (fieldRequestedGears) {
		if (typeOfPlayableCharacterLoaded != 2) {
			setupFieldCurrentPartyFromKernelAsGears();
			characterOrGearsLoadingInProgress = 1;
		}
	}
	else {
		if (typeOfPlayableCharacterLoaded != 1) {
			setupFieldCurrentPartyFromKernelAsCharacters();
			characterOrGearsLoadingInProgress = 1;
		}
	}
}

void freeFieldData()
{
	MissingCode();

	actorArray.clear();
	rawFieldTriggerData.clear();
	rawFieldDialogBundle.clear();
	rawFieldScriptData.clear();
	rawFieldWalkMesh.clear();
	walkMesh.clear();
	rawFieldModels.clear();
	gCurrentFieldModels.clear();
	rawFieldActorSetupParams.clear();

	MissingCode();
}

void transitionFields()
{
	MissingCode();

	freeFieldData();

	MissingCode();

	switch (fieldTransitionMode)
	{
	case 2:
		MissingCode();
		startLoadingPlayableCharacters();
		finalizeLoadPlayableCharacters();
		initFieldData();
		loadFieldGraphics();
		if (fieldGraphicsUploaded)
		{
			MissingCode();
			fieldGraphicsUploaded = 0;
			MissingCode();
		}
		MissingCode();
		break;
	default:
		assert(0);
		break;
	}

	fieldTransitionMode = 2;
	MissingCode();

}

void ClearCacheAfterOverlayLoad()
{
	MissingCode();
}

char noUpdatesToPartyMemebers = 0;

int onlyUpdateDirector = 0;
void exectueEntitiesUpdateFunction()
{
	int numEntitiesToUpdate = totalActors;
	if (onlyUpdateDirector == 1)
	{
		numEntitiesToUpdate = 1;
	}

	MissingCode();

	numDialogWindowsCreatedThisFrame = 0;
	for (int i = 0; i < numEntitiesToUpdate; i++)
	{
		sFieldEntity* pFieldEntity = &actorArray[i];
		if (((pFieldEntity->m58_flags & 0xf00) != 0) && ((pFieldEntity->m4C_scriptEntity->m4_flags & 0x100000) == 0))
		{
			MissingCode();

			pCurrentFieldScriptActor = pFieldEntity->m4C_scriptEntity;
			pCurrentFieldScriptActor->m0_flags &= 0xfeffffff;
			currentFieldActorId = i;
			pCurrentFieldEntity = pFieldEntity;

			int maxScriptIndex = 0xF;
			if (noUpdatesToPartyMemebers)
			{
				assert(0);
			}
			for (int j = 0; j < 8; j++)
			{
				if (pCurrentFieldScriptActor->m8C_scriptSlots[j].m4_flags.m18 <= maxScriptIndex)
				{
					pCurrentFieldScriptActor->mCE_currentScriptSlot = j;
					maxScriptIndex = pCurrentFieldScriptActor->m8C_scriptSlots[j].m4_flags.m18;
				}
			}
			if (maxScriptIndex == 0xF)
			{
				pCurrentFieldScriptActor->m8C_scriptSlots[0].m0_scriptPC = getScriptEntryPoint(i, 1);
				pCurrentFieldScriptActor->mCE_currentScriptSlot = 0;
				pCurrentFieldScriptActor->m8C_scriptSlots[0].m4_flags.m18 = 7;
			}
			currentScriptFinished = 1;
			pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m0_scriptPC;
			if (!(pCurrentFieldScriptActor->m0_flags & 1))
			{
				executeFieldScript(8);
			}
			pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m0_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC;
		}
	}
}

int entityUpdateVar0 = 0;
int entityUpdateVar1 = 0;

std::array<s16, 4> deltaMoveTable0 = {
	0x4, 0x8, 0x10, 0x20
};

std::array<s16, 8> deltaMoveTable1 = {
	0xC00, 0xE00, 0x0, 0x200,
	0x400, 0x600, 0x800, 0xA00
};

s16 deltaMoveVar0 = 0;

void computeDeltaMove(sVec3* param_1, int param_2, uint param_3)
{
	int iVar1;
	int iVar2;

	iVar2 = param_2 * 0x10 * entityMoveSpeedFactor >> 0xc;
	iVar1 = getAngleSin(param_3 & 0xfff);
	param_1->vx = iVar1 * iVar2;
	iVar1 = getAngleCos(param_3 & 0xfff);
	param_1->vy = 0;
	param_1->vz = -(iVar1 * iVar2);
	return;
}

void updateEntityEventCode2(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity)
{
	int triangleFlags = 0;
	if (((pFieldScriptEntity->m4_flags >> ((int)(short)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) && (noUpdatesToPartyMemebers == '\0')) {
		triangleFlags = pFieldScriptEntity->m14_currentTriangleFlag;
	}

	sVec3 deltaMove;
	computeDeltaMove(&deltaMove, deltaMoveTable0[(triangleFlags >> 9) & 3], deltaMoveTable1[(triangleFlags >> 11) & 0x7] + deltaMoveVar0 & 0xfff);

	int iVar9 = 0;
	int iVar12 = 0;

	if ((pFieldScriptEntity->m0_flags & 0x41800) == 0)
	{
		if ((triangleFlags & 0x420000) != 0) {
			VECTOR local_60;
			local_60.vx = -((pFieldScriptEntity->m50).vx * (pFieldScriptEntity->m50).vy) >> 0xf;
			local_60.vz = -((pFieldScriptEntity->m50).vz * (pFieldScriptEntity->m50).vy) >> 0xf;
			if (local_60.vx == 0) {
				local_60.vx = 1;
			}
			local_60.vy = 1;
			if (local_60.vz == 0) {
				local_60.vz = 1;
			}
			VECTOR local_50;
			VectorNormal(&local_60, &local_50);
			if (local_50.vx == 0) {
				local_50.vx = 1;
			}
			if (local_50.vy == 0) {
				local_50.vy = 1;
			}
			if (local_50.vz == 0) {
				local_50.vz = 1;
			}
			int iVar13 = pFieldScriptEntity->mF0 >> 0x11;
			int iVar8 = 0xc;
			iVar9 = local_50.vx * iVar13 * 0x10;
			iVar12 = local_50.vz * iVar13 * 0x10;
			if ((triangleFlags & 0x400000) != 0) {
				iVar8 = 0x18;
			}
			iVar13 = iVar8 << 0x10;
			if (pFieldScriptEntity->mF0 >> 0x10 < iVar8) {
				iVar13 = pFieldScriptEntity->mF0 + pFieldEntity->m4_pVramSpriteSheet->m1C;
			}
			pFieldScriptEntity->mF0 = iVar13;
			pFieldEntity->m4_pVramSpriteSheet->mC.vy = pFieldScriptEntity->mF0 >> 1;
		}
		if ((triangleFlags & 0x400000) != 0) {
			pFieldScriptEntity->m40.vx += iVar9;
			pFieldScriptEntity->m40.vz += iVar12;
			pFieldScriptEntity->m104_rotation = pFieldScriptEntity->m104_rotation | 0x8000;
		}
		if (pFieldScriptEntity->m74 == -1)
		{
			if ((triangleFlags & 0x20000) != 0) {
				(pFieldScriptEntity->m40).vx = (pFieldScriptEntity->m40).vx + iVar9;
				(pFieldScriptEntity->m40).vz = (pFieldScriptEntity->m40).vz + iVar12;
			}
			if (triangleFlags & 0x8000)
			{
				pFieldScriptEntity->m40.vx += deltaMove.vx;
				pFieldScriptEntity->m40.vy += deltaMove.vy;
				pFieldScriptEntity->m40.vz += deltaMove.vz;
			}
		}
	}
	else
	{
		if (triangleFlags & 0x4000)
		{
			pFieldScriptEntity->m40.vx += deltaMove.vx;
			pFieldScriptEntity->m40.vy += deltaMove.vy;
			pFieldScriptEntity->m40.vz += deltaMove.vz;
		}
	}

	if (pFieldScriptEntity->m74 != -1)
	{
		assert(0);
	}

	if ((pFieldScriptEntity->m4_flags & 0x22000) == 0x22000) {
		assert(0);
	}
}

s16 playerCanRun = 1;
s32 updateEntityEventCode3Var0 = 0;
s32 updateEntityEventCode3Var1 = 0;
s32 updateEntityEventCode3Var2 = 0;
int updateEntityEventCode3Sub0(sFieldScriptEntity* param_1)
{
	int iVar1;

	iVar1 = -1;
	if (((((((param_1->m14_currentTriangleFlag & 0x420000U) == 0) && (iVar1 = -1, updateEntityEventCode3Var1 == 0)) && (iVar1 = -1, param_1->m30_stepVector.vx == 0)) &&
		((iVar1 = -1, param_1->m30_stepVector.vy == 0 && (iVar1 = -1, param_1->m30_stepVector.vz == 0)))) &&
		((iVar1 = -1, updateEntityEventCode3Var2 == 1 && ((iVar1 = -1, param_1->m74 == 0xff && (iVar1 = -1, (param_1->m0_flags & 0x401800) == 0)))))) &&
		(((param_1->m4_flags & 1) == 0 || (iVar1 = -1, param_1->m10_walkmeshId != 0)))) {
		if (((param_1->m4_flags & 2) == 0) || (param_1->m10_walkmeshId != 1)) {
			iVar1 = 0;
			if ((param_1->m4_flags & 4) != 0) {
				iVar1 = -(uint)(param_1->m10_walkmeshId == 2);
			}
		}
		else {
			iVar1 = -1;
		}
	}
	return iVar1;
}

void updateEntityEventCode3Sub1(sFieldEntitySub4* param_1, ushort rotation, sFieldEntity* param_3)
{
	int iVar1;
	sFieldScriptEntity* psVar2;
	int iVar3;
	uint uVar4;

	if ((param_3->m58_flags & 0x40) == 0) {
		uVar4 = 0x40000 / (ushort)param_3->m4C_scriptEntity->m76;
		if ((rotation & 0x8000) != 0) {
		LAB_Field__800821b0:
			(param_1->mC).vx = 0;
			(param_1->mC).vz = 0;
			goto LAB_Field__800821bc;
		}
	}
	else {
		if (((int)(short)rotation & 0x8000U) != 0) goto LAB_Field__800821b0;
		psVar2 = param_3->m4C_scriptEntity;
		uVar4 = psVar2->m4_flags;
		if ((uVar4 & 0x2000) == 0) {
			if ((uVar4 & 0x80000) == 0) {
				OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, (int)(short)rotation);
			}
			else {
				iVar3 = ((int)(0x40000 / (ushort)psVar2->m76) >> 8) * 0x20;
				iVar1 = getAngleSin(rotation & 0xfff);
				(param_1->mC).vx = (iVar1 * iVar3 >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[0];
				iVar1 = getAngleCos(rotation & 0xfff);
				(param_1->mC).vz = (-(iVar1 * iVar3) >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[2];
				param_1->m18 = 0x4000000 / (ushort)param_3->m4C_scriptEntity->m76;
			}
			goto LAB_Field__800821bc;
		}
		if ((uVar4 & 0x20000) != 0) {
			uVar4 = psVar2->m12C_flags;
			assert(0);
			//(param_1->mC).vx = *(int*)(*(int*)(&DAT_801e8670 + (uVar4 >> 0xb & 0x1c)) + 0x128) * -0x10000;
			//(param_1->mC).vz = *(int*)(*(int*)(&DAT_801e8670 + (uVar4 >> 0xb & 0x1c)) + 0x130) * -0x10000;
			goto LAB_Field__800821bc;
		}
		uVar4 = 0x80000 / (ushort)psVar2->m76;
	}
	iVar3 = ((int)uVar4 >> 8) * 0x20;
	iVar1 = getAngleSin(rotation & 0xfff);
	(param_1->mC).vx = (iVar1 * iVar3 >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[0];
	iVar1 = getAngleCos(rotation & 0xfff);
	(param_1->mC).vz = (-(iVar1 * iVar3) >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[2];

LAB_Field__800821bc:
	(param_1->mC).vx = (param_1->mC).vx & 0xfffff000;
	(param_1->mC).vz = (param_1->mC).vz & 0xfffff000;
}

int updateEntityEventCode3Sub2(sVec3* param_1, sFieldScriptEntity* param_2)
{
	long lVar2;
	std::vector<u16>* pasVar3;

	if ((param_2->m12C_flags & 0x1000) != 0) {
		sVec2_s16 position;
		position.set(((param_2->m20_position).vx + param_1->vx >> 0x10), ((param_2->m20_position).vz + param_1->vz >> 0x10));

		std::vector<sVec2_s16>& pasVar3 = param_2->m114_movementBoundingZone;
		std::array<sVec2_s16, 4> boundingZone;
		boundingZone[0].set(pasVar3[0].vx, pasVar3[0].vx);
		boundingZone[1].set(pasVar3[1].vx, pasVar3[1].vx);
		boundingZone[2].set(pasVar3[2].vx, pasVar3[2].vx);
		boundingZone[3].set(pasVar3[3].vx, pasVar3[3].vx);

		if ((NCLIP(boundingZone[0], boundingZone[1], position) < 0) || (NCLIP(boundingZone[1], boundingZone[2], position) < 0) || (NCLIP(boundingZone[2], boundingZone[3], position) < 0)) {
			return -1;
		}
		else {
			lVar2 = NCLIP(boundingZone[2], boundingZone[0], position);
			return lVar2 >> 0x1f;
		}
	}
	return 0;
}

int updateEntityEventCode3Sub3Sub1(sVec3* param_1, VECTOR* param_2, sFieldScriptEntity* param_3, std::array<SVECTOR, 2>& param_4, SVECTOR* param_5, int param_6, uint* param_7)
{
	MissingCode();
	return 0;
}

uint updateEntityEventCode3Sub3Sub2(short param_1, std::array<SVECTOR, 2>& param_2, sVec3* param_3)

{
	short sVar1;
	short sVar2;
	long lVar3;
	uint uVar4;
	uint uVar5;
	VECTOR local_38;
	VECTOR local_28;

	lVar3 = ratan2(param_2[1].vz - param_2[0].vz, param_2[1].vx - param_2[0].vx);
	uVar4 = -lVar3 & 0xfff;
	uVar5 = (0xc00U - (int)param_1 & 0xfff) + uVar4 & 0xfff;
	if (uVar5 - 0x80 < 0xf01) {
		if (uVar5 < 0x800) {
			local_38.vx = param_2[0].vx - param_2[1].vx;
			sVar1 = param_2[0].vz;
			sVar2 = param_2[1].vz;
			uVar4 = uVar4 + 0x800 & 0xfff;
		}
		else {
			local_38.vx = param_2[1].vx - param_2[0].vx;
			sVar1 = param_2[1].vz;
			sVar2 = param_2[0].vz;
		}
		local_38.vy = 0;
		local_38.vz = (int)sVar1 - (int)sVar2;
		VectorNormal(&local_38, &local_28);
		lVar3 = length2d(param_3->vx >> 0xc, param_3->vz >> 0xc);

		param_3->vx = local_28.vx * lVar3;
		param_3->vy = 0;
		param_3->vz = local_28.vz * lVar3;
	}
	else {
		param_3->vx = 0;
		param_3->vy = 0;
		param_3->vz = 0;
	}
	return uVar4;
}

int updateEntityEventCode3Sub4Sub1(sVec3* deltaStep, VECTOR* position, sFieldScriptEntity* pFieldScriptEntity, std::array<SVECTOR, 2>& param_4, SVECTOR* param_5, int param_6)
{
	int collisionFlag;
	int uVar2;

	int triangleId = (int)pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
	std::vector<sWalkMesh::sTriangleData>& pWalkMeshTriangles = *walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
	std::vector<SVECTOR>& pWalkMeshVertices = *walkMeshVertices[pFieldScriptEntity->m10_walkmeshId];

	if (triangleId == -1) {
		uVar2 = -1;
	}
	else {
		param_5->vx = (position->vx + deltaStep->vx) >> 0x10;
		param_5->vy = 0;
		param_5->vz = (position->vz + deltaStep->vz) >> 0x10;

		sVec2_s16 startPosition;
		sVec2_s16 endPosition;
		startPosition.set((position->vx >> 0x10), (position->vz >> 0x10));
		endPosition.set((position->vx + deltaStep->vx) >> 0x10, (position->vz + deltaStep->vz) >> 0x10);

		u32 mask = 0;

		if ((pFieldScriptEntity->m4_flags >> ((int)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
			mask = -(uint)(noUpdatesToPartyMemebers == '\0');
		}

		int tempTriangleId;

		sFieldScriptEntity* local_58 = pFieldScriptEntity;
		int iterationCount = 0;
		for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
		{
			sWalkMesh::sTriangleData* pTriangle = &pWalkMeshTriangles[triangleId];

			sVec2_s16 vert0;
			sVec2_s16 vert1;
			sVec2_s16 vert2;

			vert0.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[0]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[0]].vz);
			vert1.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[1]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[1]].vz);
			vert2.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[2]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[2]].vz);

			tempTriangleId = triangleId;

			collisionFlag = 0;
			if (NCLIP(vert0, vert1, endPosition) < 0) {
				collisionFlag |= 1;
			}

			if (NCLIP(vert1, vert2, endPosition) < 0) {
				collisionFlag |= 2;
			}

			if (NCLIP(vert2, vert0, endPosition) < 0) {
				collisionFlag |= 4;
			}

			if (collisionFlag < 8) {
				switch (collisionFlag)
				{
				case 0:
					mask = 0xFF;
					break;
				case 1:
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
					break;
				case 2:
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
					break;
				case 3:
					if (NCLIP(vert1, endPosition, startPosition) < 0)
					{
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
						collisionFlag = 1;
					}
					else {
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
						collisionFlag = 2;
					}
					break;
				case 4:
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
					break;
				case 5:
					if (NCLIP(vert0, endPosition, startPosition) < 0)
					{
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
						collisionFlag = 4;
					}
					else {
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
						collisionFlag = 1;
					}
					break;
				case 6:
					if (NCLIP(vert2, endPosition, startPosition) < 0)
					{
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
						collisionFlag = 2;
					}
					else {
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
						collisionFlag = 4;
					}
					break;
				case 7:
					triangleId = -1;
					break;
				default:
					assert(0);
				}
			}

			// todo: does this really happen to be -1?
			if (triangleId == -1)
			{
				return -1;
			}
			u32 uVar4 = ((*walkMeshVar1)[pWalkMeshTriangles[triangleId].mC_indexInWalkmeshData1]) & mask;
			if ((((local_58->m0_flags >> 9 & 3 & uVar4 >> 3) != 0) || ((local_58->m0_flags >> 8 & 7 & uVar4 >> 5) != 0)) || (((uVar4 & 0x800000) != 0 && (local_58->m10_walkmeshId == 0)))) {
				triangleId = -1;
				break;
			}

			if (triangleId == -1)
				break;
		}

		if ((triangleId == -1) || (iterationCount == 0x20)) {

			switch (collisionFlag)
			{
			case 1:
				param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
				param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
				break;
			case 2:
				param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
				param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
				break;
			case 4:
				param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
				param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
				break;
			default:
				return -1;
			}
			uVar2 = -1;
		}
		else {
			if (param_6 == -1) {
				uVar2 = 0;
			}
			else {
				//psVar6 = (short*)(puVar9 + iVar8 * 0xe);
				assert(0);
				//FUN_Field__8007b07c((short*)(puVar10 + *psVar6 * 8), (short*)(puVar10 + psVar6[1] * 8), (short*)(puVar10 + psVar6[2] * 8), param_5, &sStack104);
				uVar2 = 0;
			}
		}
	}
	return uVar2;
}

int updateEntityEventCode3Sub4(sVec3* position, sFieldScriptEntity* param_2, std::array<SVECTOR, 2>& param_3, s16 angle)
{
	VECTOR* psVar4;
	sVec3 local_60;
	SVECTOR auStack80;

	local_60.vx = position->vx + getAngleSin(angle & 0xfff) * 0x40;
	local_60.vz = position->vz + getAngleCos(angle & 0xfff) * -0x40;

	psVar4 = &param_2->m20_position;

	if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1) {
		local_60.vx = position->vx;
		local_60.vy = position->vy;
		local_60.vz = position->vz;
		updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
	}
	else {
		int newAngle = angle - 0x100U & 0xfff;
		local_60.vx = position->vx + getAngleSin(newAngle) * 0x40;
		local_60.vz = position->vz + getAngleCos(newAngle) * -0x40;
		if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1)
		{
			local_60.vx = position->vx;
			local_60.vy = position->vy;
			local_60.vz = position->vz;
			updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
		}
		else
		{
			int newAngle = angle + 0x100U & 0xfff;
			local_60.vx = position->vx + getAngleSin(newAngle) * 0x40;
			local_60.vz = position->vz + getAngleCos(newAngle) * -0x40;
			if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1)
			{
				local_60.vx = position->vx;
				local_60.vy = position->vy;
				local_60.vz = position->vz;
				updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
			}
			else
			{
				local_60.vx = position->vx;
				local_60.vy = position->vy;
				local_60.vz = position->vz;
			}

		}
	}

	if (updateEntityEventCode3Sub4Sub1(&local_60, &param_2->m20_position, param_2, param_3, &auStack80, 0) == -1) {
		return -1;
	}
	else {
		if ((param_2->m0_flags & 0x40000) == 0) {
			if (((int)auStack80.vy << 0x10 < (param_2->m20_position).vy) && (updateEntityEventCode3Var1 == 0))
				return -1;
		}
		else {
			auStack80.vy = param_2->mEC_elevation;
		}

		position->vx = local_60.vx;
		position->vy = auStack80.vy * 0x10000 - param_2->m20_position.vy;
		position->vz = local_60.vz;

		param_2->m72_elevation = (param_2->m20_position.vy + position->vy) >> 0x10;
		return 0;
	}
}

int updateEntityEventCode3Sub3(sVec3* param_1, sFieldScriptEntity* param_2, std::array<SVECTOR, 2>& param_3, short angle)
{
	long lVar2;
	sVec3 local_88;
	SVECTOR local_68;
	VECTOR local_58;
	VECTOR local_48;
	uint local_38;

	local_88.vx = param_1->vx + getAngleSin(angle - 0x100U & 0xfff) * 0x40;
	local_88.vz = param_1->vz + getAngleCos(angle - 0x100U & 0xfff) * -0x40;
	if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1) {
		local_88.vx = param_1->vx;
		local_88.vy = param_1->vy;
		local_88.vz = param_1->vz;
		updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
	}
	else {
		local_88.vx = param_1->vx + getAngleSin(angle + 0x100U & 0xfff) * 0x40;
		local_88.vz = param_1->vz + getAngleCos(angle + 0x100U & 0xfff) * -0x40;
		if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1)
		{
			local_88.vx = param_1->vx;
			local_88.vy = param_1->vy;
			local_88.vz = param_1->vz;
			updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
		}
		else
		{
			local_88.vx = param_1->vx + getAngleSin(angle & 0xfff) * 0x40;
			local_88.vz = param_1->vz + getAngleCos(angle & 0xfff) * -0x40;
			if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1)
			{
				local_88.vx = param_1->vx;
				local_88.vy = param_1->vy;
				local_88.vz = param_1->vz;
				updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
			}
			else
			{
				local_88.vx = param_1->vx;
				local_88.vy = param_1->vy;
				local_88.vz = param_1->vz;
			}
		}
	}
	if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, 0, &local_38) == -1) {
		return -1;
	}
	if ((((param_2->m20_position.vy) >> 16) <= local_68.vy) && ((local_38 & 0x200000) == 0)) {
		if ((local_38 & 0x420000) == 0) {
			if (((param_2->m20_position.vy) >> 16) + 0x40 <= (int)local_68.vy)
			{
				param_1->vx = local_88.vx;
				param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
				param_1->vz = local_88.vz;
				param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
				return 0;
			}
		}
		else {
			if ((param_2->m14_currentTriangleFlag & 0x420000U) == 0)
			{
				param_1->vx = local_88.vx;
				param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
				param_1->vz = local_88.vz;
				param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
				return 0;
			}
		}
	}
	local_58.vx = -local_88.vx >> 8;
	local_58.vz = -local_88.vz >> 8;
	local_58.vy = local_68.vy * 0x10000 - (param_2->m20_position).vy >> 8;
	VectorNormal(&local_58, &local_48);
	lVar2 = length2d(local_88.vx >> 8, local_88.vz >> 8);

	local_88.vx = -(lVar2 * local_48.vx) >> 4;
	local_88.vy = lVar2 * local_48.vy >> 4;
	local_88.vz = -(lVar2 * local_48.vz) >> 4;
	if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, 0, &local_38) == -1) {
		return -1;
	}
	param_2->m0_flags = param_2->m0_flags | 0x4000000;

	param_1->vx = local_88.vx;
	param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
	param_1->vz = local_88.vz;
	param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
	return 0;
}

s16 updateEntityEventCode4Var0 = 0;
s16 updateEntityEventCode4Var1 = 0;

void updateEntityEventCode4(sFieldEntitySub4* param_1, int param_2, sFieldEntity* param_3)
{
	sFieldScriptEntity* psVar1;

	if ((param_3->m58_flags & 0x40) != 0) {
		if ((param_2 != 3) && (updateEntityEventCode4Var0 == 0)) {
			param_3->m4C_scriptEntity->m0_flags = param_3->m4C_scriptEntity->m0_flags & 0xfffff7ff;
		}
		if (param_2 == 0xff) {
			param_2 = 0;
		}
		if (param_2 != updateEntityEventCode4Var1) {
			param_3->m4C_scriptEntity->m0_flags = param_3->m4C_scriptEntity->m0_flags & 0xfffff7ff;
		}
		psVar1 = param_3->m4C_scriptEntity;
		if ((psVar1->m4_flags & 0x2000) == 0) {
			if ((psVar1->m4_flags & 0x1000000) == 0) {
				OP_INIT_ENTITY_SCRIPT_sub0Sub6(param_1, param_2);
			}
		}
		else {
			assert(0);
		}
	}
	return;
}

void updateEntityEventCode3(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity)
{
	int rotation = pFieldScriptEntity->m104_rotation;
	std::array<SVECTOR, 2> auStack56;

	if ((pFieldScriptEntity->m0_flags & 0x1000000) != 0) {
		updateEntityEventCode3Var0 = index;
		return;
	}

	int walkSpeed = 1;
	if ((((pFieldScriptEntity->m0_flags & 0x4000) != 0) && ((padButtonForScripts[0].vx & 0x40) != 0)) && (playerCanRun == 1)) {
		walkSpeed = 2;
	}
	if (((pFieldScriptEntity->m0_flags & 0x1800) != 0) && (pFieldScriptEntity->mE8 != walkSpeed)) {
		if (pFieldScriptEntity->mE8 == 1) {
			walkSpeed = 1;
		}
		else {
			if (pFieldScriptEntity->mE8 == 2) {
				walkSpeed = 2;
			}
		}
	}

	updateEntityEventCode3Var0 = index;

	if (8 < pFieldScriptEntity->mE3) {
		pFieldScriptEntity->mE3--;
	}

	int moveMask = (pFieldScriptEntity->m40).vx | (pFieldScriptEntity->m40).vy | (pFieldScriptEntity->m40).vz;
	if (updateEntityEventCode3Sub0(pFieldScriptEntity) == -1)
	{
		moveMask = 1;
	}

	sVec3 stepVector;

	bool bMoved = false;

	if (((((int)(short)rotation & 0x8000U) == 0) || (moveMask != 0)) || ((pFieldScriptEntity->m0_flags & 0x40800) != 0)) {
		if (((int)(short)rotation & 0x8000U) == 0) {
			updateEntityEventCode3Sub1(pFieldEntity->m4_pVramSpriteSheet, (int)(short)rotation, pFieldEntity);
			stepVector.vx = pFieldEntity->m4_pVramSpriteSheet->mC.vx + pFieldScriptEntity->m40.vx;
			stepVector.vy = pFieldEntity->m4_pVramSpriteSheet->mC.vy + pFieldScriptEntity->m40.vy;
			stepVector.vz = pFieldEntity->m4_pVramSpriteSheet->mC.vz + pFieldScriptEntity->m40.vz;
			pFieldScriptEntity->m106_currentRotation = rotation;
		}
		else {
			stepVector.vx = (pFieldScriptEntity->m40).vx;
			stepVector.vy = (pFieldScriptEntity->m40).vy;
			stepVector.vz = (pFieldScriptEntity->m40).vz;
			rotation = pFieldScriptEntity->m106_currentRotation & 0xfff;
		}

		if (updateEntityEventCode3Sub2(&stepVector, pFieldScriptEntity) != 0)
		{
			bMoved = false;
		}
		else
		{
			if ((stepVector.vx != 0) || (stepVector.vz != 0)) {
				int lVar2 = ratan2(stepVector.vz, stepVector.vx);
				rotation = -(short)lVar2 & 0xfff;
			}

			int iVar1 = -1;
			if (pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId] != -1) {
				int backupFlags = pFieldScriptEntity->m0_flags;
				if (index == playerControlledEntity) {
					if (partyToFieldEntityArrayMapping[1] != 0xff) {
						pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags | (actorArray[partyToFieldEntityArrayMapping[1]].m4C_scriptEntity)->m0_flags & 0x600;
					}
					if (partyToFieldEntityArrayMapping[2] != 0xff) {
						pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags | (actorArray[partyToFieldEntityArrayMapping[2]].m4C_scriptEntity)->m0_flags & 0x600;
					}
				}
				if ((((pFieldScriptEntity->m0_flags & 0x41800) == 0) && (pFieldScriptEntity->m74 == -1)) && (updateEntityEventCode3Var1 == 0)) {
					iVar1 = updateEntityEventCode3Sub3(&stepVector, pFieldScriptEntity, auStack56, rotation);
				}
				else {
					iVar1 = updateEntityEventCode3Sub4(&stepVector, pFieldScriptEntity, auStack56, rotation);
				}
				pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfffff9ff | backupFlags & 0x600;
			}
			if (iVar1 == -1)
			{
				bMoved = false;
			}
		}
	}
	else
	{
		walkSpeed = pFieldScriptEntity->mE6;
		pFieldScriptEntity->m104_rotation = pFieldScriptEntity->m104_rotation | 0x8000;

		bMoved = false;
	}

	if (!bMoved)
	{
		pFieldScriptEntity->mF0 = 0x10000;
		(pFieldScriptEntity->m40).vx = 0;
		(pFieldScriptEntity->m40).vy = 0;
		(pFieldScriptEntity->m40).vz = 0;
		stepVector.vx = 0;
		stepVector.vy = 0;
		stepVector.vz = 0;
		(pFieldEntity->m4_pVramSpriteSheet->mC).vx = 0;
		(pFieldEntity->m4_pVramSpriteSheet->mC).vz = 0;
		pFieldScriptEntity->m106_currentRotation = pFieldScriptEntity->m106_currentRotation | 0x8000;
	}

	pFieldScriptEntity->m4_flags = pFieldScriptEntity->m4_flags & 0xffffefff;

	int sVar4 = pFieldScriptEntity->mE8;// TODO: can this be a different value?

	if ((pFieldScriptEntity->m0_flags & 0x800) == 0) {
		if (((int)pFieldScriptEntity->m104_rotation & 0x8000U) != 0) {
			walkSpeed = pFieldScriptEntity->mE6;
		}
		moveMask = getWalkmeshTriangleFlag(pFieldScriptEntity);
		sVar4 = walkSpeed;
		if ((moveMask & 0x200000) != 0) {
			if (((int)pFieldScriptEntity->m104_rotation & 0x8000U) != 0) {
				sVar4 = 6;
				if (pFieldScriptEntity->mE8 != 6) goto LAB_Field__800830ac;
				pFieldScriptEntity->m4_flags = pFieldScriptEntity->m4_flags | 0x1000;
			}
			sVar4 = 6;
		}
	}
	else
	{
		assert(0);
	}

LAB_Field__800830ac:
	if (pFieldScriptEntity->mEA_currentAnimationPlaying != 0xff) {
		sVar4 = pFieldScriptEntity->mEA_currentAnimationPlaying;
	}
	if (((int)pFieldScriptEntity->mE8 != (int)sVar4) && ((pFieldScriptEntity->m0_flags & 0x2000000) == 0)) {
		pFieldScriptEntity->mE8 = sVar4;
		updateEntityEventCode4(pFieldEntity->m4_pVramSpriteSheet, (int)sVar4, pFieldEntity);
	}
	if ((pFieldScriptEntity->m14_currentTriangleFlag & 0x100U) != 0) {
		stepVector.vx = stepVector.vx >> 1;
		stepVector.vz = stepVector.vz >> 1;
	}
	pFieldScriptEntity->m30_stepVector.vx = stepVector.vx;
	pFieldScriptEntity->m30_stepVector.vy = stepVector.vy;
	pFieldScriptEntity->m30_stepVector.vz = stepVector.vz;
	pFieldScriptEntity->m40.vx = 0;
	pFieldScriptEntity->m40.vy = 0;
	pFieldScriptEntity->m40.vz = 0;
}

void EntityMoveCheck0(uint entityIndex, sFieldEntity* pEntity, sFieldScriptEntity* pScriptEntity)
{
	MissingCode();
}

struct sEntityMoveCheck1StackStruct
{
	s32 m0[4];
	s32 m10[4];
	s32 m20[4];
	s16 m30[4];
	VECTOR m38[4];
	s32 m78_position[3];
	s16 m88[4];
};

void EntityMoveCheck1Sub1Sub1(int* param_1, int* param_2, int param_3)

{
	int iVar1;

	iVar1 = *param_1;
	if (param_3 < 0) {
		*param_1 = iVar1;
	}
	else {
		*param_1 = iVar1;
		iVar1 = iVar1 + param_3;
	}
	*param_2 = iVar1;
	return;
}

s32 EntityMoveCheck1Sub1(sFieldScriptEntity* pFieldScriptEntity, int walkmeshId, s32* param_3, VECTOR* param_4, s16* param_5, s32* param_6)
{
	if (pFieldScriptEntity->m8_currentWalkMeshTriangle[walkmeshId] == -1) {
		return -1;
	}

	int triangleId = pFieldScriptEntity->m8_currentWalkMeshTriangle[walkmeshId];

	sVec2_s16 refPos;
	sVec2_s16 refPos2;

	SVECTOR projectedPosition;
	projectedPosition.vx = (pFieldScriptEntity->m20_position.vx + pFieldScriptEntity->m30_stepVector.vx) >> 16;
	projectedPosition.vy = 0;
	projectedPosition.vz = (pFieldScriptEntity->m20_position.vz + pFieldScriptEntity->m30_stepVector.vz) >> 16;

	refPos.set((pFieldScriptEntity->m20_position.vx + pFieldScriptEntity->m30_stepVector.vx) >> 16, (pFieldScriptEntity->m20_position.vz + pFieldScriptEntity->m30_stepVector.vz) >> 16);
	refPos2.set(pFieldScriptEntity->m20_position.vx >> 16, pFieldScriptEntity->m20_position.vz >> 16);

	int mask = 0;
	if ((pFieldScriptEntity->m4_flags >> (walkmeshId + 3U & 0x1f) & 1) == 0) {
		mask = -(uint)(noUpdatesToPartyMemebers == '\0');
	}

	int oldWalkmeshId = walkmeshId;

	int iterationCount = 0;
	for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
	{
		sWalkMesh::sTriangleData& pTriangle = (*walkMeshTriangle[walkmeshId])[triangleId];
		std::vector<sWalkMesh::sTriangleData>::iterator pWalkMeshTriangles = walkMeshTriangle[walkmeshId]->begin();
		std::vector<SVECTOR>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();

		sVec2_s16 vert0;
		sVec2_s16 vert1;
		sVec2_s16 vert2;

		vert0.set(pVertices[pTriangle.m0_verticeIndex[0]].vx, pVertices[pTriangle.m0_verticeIndex[0]].vz);
		vert1.set(pVertices[pTriangle.m0_verticeIndex[1]].vx, pVertices[pTriangle.m0_verticeIndex[1]].vz);
		vert2.set(pVertices[pTriangle.m0_verticeIndex[2]].vx, pVertices[pTriangle.m0_verticeIndex[2]].vz);

		int collisionFlag = 0;
		if (NCLIP(vert0, vert1, refPos) < 0) {
			collisionFlag |= 1;
		}

		if (NCLIP(vert1, vert2, refPos) < 0) {
			collisionFlag |= 2;
		}

		if (NCLIP(vert2, vert0, refPos) < 0) {
			collisionFlag |= 4;
		}

		if (collisionFlag < 8) {
			switch (collisionFlag)
			{
			case 0:
				mask = 0xFF;
				break;
			case 1:
				triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
				break;
			case 2:
				triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
				break;
			case 3:
				if (NCLIP(vert1, refPos, refPos2) < 0)
				{
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
				}
				else {
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
				}
				break;
			case 4:
				triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
				break;
			case 5:
				if (NCLIP(vert0, refPos, refPos2) < 0)
				{
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
				}
				else {
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
				}
				break;
			case 6:
				if (NCLIP(vert2, refPos, refPos2) < 0)
				{
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
				}
				else {
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
				}
				break;
			case 7:
				triangleId = -1;
				break;
			default:
				assert(0);
			}
		}

		if (triangleId == -1)
		{
			return -1;
		}
	}
	if (iterationCount == 0x20)
	{
		return -1;
	}

	std::vector<SVECTOR>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();
	sWalkMesh::sTriangleData& pTriangle = (*walkMeshTriangle[walkmeshId])[triangleId];

	projectPositionOnTriangle(pVertices[pTriangle.m0_verticeIndex[0]], pVertices[pTriangle.m0_verticeIndex[1]], pVertices[pTriangle.m0_verticeIndex[2]], &projectedPosition, param_4);

	*param_5 = triangleId;

	int triangleParamD = pTriangle.mD * 4;
	if (triangleParamD < 0)
	{
		triangleParamD = 0;
	}

	if (pFieldScriptEntity->m10_walkmeshId == oldWalkmeshId)
	{
		if (((*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask & 0x800000) != 0)
		{
			*param_3 = 0x7fffffff;
			*param_6 = 0x7fffffff;
			return 0;
		}

		if (((pFieldScriptEntity->m30_stepVector.vx == 0) && (pFieldScriptEntity->m30_stepVector.vy == 0)) && (pFieldScriptEntity->m30_stepVector.vz == 0)) {
			*param_3 = projectedPosition.vy;
		}
		else
		{
			*param_3 = pFieldScriptEntity->m72_elevation;
		}
	}
	else {
		if (((*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask & 0x800000) != 0)
		{
			*param_3 = 0x7fffffff;
			*param_6 = 0x7fffffff;
			return 0;
		}

		*param_3 = projectedPosition.vy;
	}

	EntityMoveCheck1Sub1Sub1(param_3, param_6, triangleId);
	return 0;
}

s8 EntityMoveCheck1Var1 = 0;
int EntityMoveCheck1(int entityIndex, int mask, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity, uint param_5)
{
	sFieldEntitySub4* psVar17 = actorArray[entityIndex].m4_pVramSpriteSheet;
	if (entityIndex == playerControlledEntity) {
		inputAllowedMask = 0xFFFF;
	}
	if ((pFieldScriptEntity->m0_flags & 0x1000000) != 0) {
		return -1;
	}
	if ((pFieldScriptEntity->m4_flags & 0x200000) != 0) {
		return -1;
	}
	if (((pFieldScriptEntity->m0_flags & 0x10000) != 0) ||
		((((entityIndex != playerControlledEntity || (EntityMoveCheck1Var1 != '\x01')) && ((psVar17->mC).vy == 0)) &&
			(!updateEntityEventCode3Sub0(pFieldScriptEntity) && (psVar17->m84 == (pFieldScriptEntity->m20_position.vy >> 16)))))) {
		return -1;
	}

	sEntityMoveCheck1StackStruct stackVar;

	stackVar.m78_position[0] = pFieldScriptEntity->m20_position.vx;
	stackVar.m78_position[1] = pFieldScriptEntity->m20_position.vy;
	stackVar.m78_position[2] = pFieldScriptEntity->m20_position.vz;

	int sVar2 = pFieldScriptEntity->m10_walkmeshId;

	for (int i = 0; i < 4; i++)
	{
		stackVar.m88[i] = pFieldScriptEntity->m8_currentWalkMeshTriangle[i];
	}

	for (int i = 0; i < 4; i++)
	{
		stackVar.m20[i] = i;
		stackVar.m0[i] = 0x7fffffff;
		stackVar.m10[i] = 0x7fffffff;
	}

	int iVar3 = 0;
	for (iVar3 = 0; iVar3 < numWalkMesh; iVar3++)
	{
		if (EntityMoveCheck1Sub1(pFieldScriptEntity, iVar3, &stackVar.m0[iVar3], &stackVar.m38[iVar3], &stackVar.m30[iVar3], &stackVar.m10[iVar3]));
		{
			break;
		}
	}

	if ((pFieldScriptEntity->m4_flags & 1) != 0) {
		stackVar.m0[0] = 0x7fffffff;
		stackVar.m10[0] = 0x7fffffff;
	}
	if ((pFieldScriptEntity->m4_flags & 2) != 0) {
		stackVar.m0[1] = 0x7fffffff;
		stackVar.m10[1] = 0x7fffffff;
	}
	if ((pFieldScriptEntity->m4_flags & 4) != 0) {
		stackVar.m0[2] = 0x7fffffff;
		stackVar.m10[2] = 0x7fffffff;
	}

	int mask2 = 0x7fffffff; // TODO: what is the initial value?
	int iVar13 = 0;
	int iVar4 = stackVar.m0[pFieldScriptEntity->m10_walkmeshId];
	do {
		int iVar12 = 0;
		int psVar7 = 0;
		s32* piVar11 = stackVar.m10;
		s32* ppsVar14 = stackVar.m20;
		do {
			ppsVar14 = ppsVar14++;
			piVar11 = piVar11 + 1;
			s32 iVar10 = stackVar.m0[psVar7 + 1];
			if (iVar10 < mask2) {
				s32 iVar5 = stackVar.m10[psVar7];
				stackVar.m0[psVar7 + 1] = mask2;
				stackVar.m0[psVar7] = iVar10;
				iVar10 = *piVar11;
				*piVar11 = iVar5;
				stackVar.m10[psVar7] = iVar10;
				mask2 = *(uint*)ppsVar14;
				*ppsVar14 = stackVar.m20[psVar7];
				stackVar.m20[psVar7] = mask2;
			}
			iVar12 = iVar12 + 1;
			psVar7++;
		} while (iVar12 < 2);
		iVar13 = iVar13 + 1;
	} while (iVar13 < 2);

	if (iVar3 == numWalkMesh - 1)
	{
		assert(0);
	}

	pFieldScriptEntity->mF0 = 0;

	if (updateEntityEventCode3Var1 == 0) {
		if (param_5 != 0) {
			if (actorArray[entityIndex].m4_pVramSpriteSheet->m84 < mask + 10) {
				pFieldScriptEntity->m74 = 0xff;
			}
			actorArray[entityIndex].m4_pVramSpriteSheet->m84 = (short)mask;
			(pFieldScriptEntity->m20_position).vy = mask << 0x10;
		}
	}
	else {
		if (param_5 < 2) {
			actorArray[entityIndex].m4_pVramSpriteSheet->m84 = (short)mask;
		}
	}

	auto psVar18 = actorArray[entityIndex].m4_pVramSpriteSheet;
	if ((pFieldScriptEntity->m0_flags & 0x40000) != 0) {
		(pFieldScriptEntity->m20_position).vy = (int)pFieldScriptEntity->mEC_elevation << 0x10;
		(psVar18->mC).vy = 0;
	}
	(pFieldScriptEntity->m20_position).vy = (pFieldScriptEntity->m20_position).vy + (psVar18->mC).vy;
	int uVar6 = getWalkmeshTriangleFlag(pFieldScriptEntity);
	if (pFieldScriptEntity->m10_walkmeshId != sVar2) {
		pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfbffffff;
	}
	if ((pFieldScriptEntity->m0_flags & 0x4000000) == 0) {
		int sVar1 = (pFieldScriptEntity->m20_position).vy >> 16;
		if (psVar18->m84 <= sVar1) goto LAB_Field__80085104;
		if (psVar18->m84 != sVar1) {
			(psVar18->mC).vy = (psVar18->mC).vy + psVar18->m1C;
		}
		pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags | 0x1000;
		pFieldScriptEntity->mF0 = (psVar18->mC).vy;
	}
	else {
	LAB_Field__80085104:
		if ((uVar6 & 0x420000) == 0) {
			pFieldScriptEntity->mF0 = 0;
		}
		if (0 < (psVar18->mC).vy) {
			(psVar18->mC).vy = 0;
		}
		pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xffbfefff;
		(pFieldScriptEntity->m20_position).vy = (int)psVar18->m84 << 0x10;
	}
	pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfbffffff;
	iVar3 = 0;
	if (0 < numWalkMesh + -1) {
		iVar4 = pFieldScriptEntity->m20_position.vy >> 16;
		int psVar8 = 0;
		do {
			if (((stackVar.m0[psVar8] < iVar4) && ((int)(iVar4 - (uint)(ushort)pFieldScriptEntity->m18[1]) < stackVar.m10[psVar8])) && (stackVar.m0[psVar8] != stackVar.m10[0])) break;
			iVar3 = iVar3 + 1;
			psVar8++;
		} while (iVar3 < numWalkMesh + -1);
	}
	if ((iVar3 == numWalkMesh + -1) &&
		((iVar3 = (*walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId])[pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId]].mD * 4, -1 < iVar3 ||
			(iVar3 + psVar18->m84 <= ((pFieldScriptEntity->m20_position.vy >> 16) - (uint)(ushort)pFieldScriptEntity->m18[1]))))) {
		psVar18->m0_position.vx = (pFieldScriptEntity->m20_position).vx;
		psVar18->m0_position.vy = (pFieldScriptEntity->m20_position).vy;
		psVar18->m0_position.vz = (pFieldScriptEntity->m20_position).vz;
		actorArray[entityIndex].mC_matrix.t[0] = pFieldScriptEntity->m20_position.vx >> 16;
		actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
		actorArray[entityIndex].mC_matrix.t[2] = pFieldScriptEntity->m20_position.vz >> 16;
		iVar3 = getWalkmeshTriangleFlag(pFieldScriptEntity);
		pFieldScriptEntity->m14_currentTriangleFlag = iVar3;
	}
	else {
		iVar3 = 0;
		assert(0);
		sEntityMoveCheck1StackStruct* psVar8 = &stackVar;
		(pFieldScriptEntity->m20_position).vx = stackVar.m78_position[0];
		pFieldScriptEntity->m10_walkmeshId = sVar2;
		pFieldScriptEntity->mF0 = 0;
		(pFieldScriptEntity->m20_position).vz = stackVar.m78_position[2];
		auto psVar10 = pFieldScriptEntity;
		do {
			auto psVar17 = psVar8->m88;
			psVar8 = (sEntityMoveCheck1StackStruct*)((int)psVar8->m0 + 2);
			iVar3 = iVar3 + 1;
			psVar10->m8_currentWalkMeshTriangle[0] = *psVar17;
			psVar10 = (sFieldScriptEntity*)((int)&psVar10->m0_flags + 2);
		} while (iVar3 < 4);
		if (psVar18->m84 != *(short*)((int)&(pFieldScriptEntity->m20_position).vy + 2)) {
			(psVar18->mC).vy = (psVar18->mC).vy + psVar18->m1C;
		}
		if ((psVar18->mC).vy < 0) {
			(psVar18->mC).vy = 0;
			(pFieldScriptEntity->m20_position).vy = stackVar.m78_position[1];
		}
		psVar18->m0_position.vx = (pFieldScriptEntity->m20_position).vx;
		psVar18->m0_position.vy = (pFieldScriptEntity->m20_position).vy;
		psVar18->m0_position.vz = (pFieldScriptEntity->m20_position).vz;
		actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
	}
LAB_Field__80085490:
	MissingCode();
	//initFollowStructForPlayer(entityIndex);
	return 0;
}

void updateScriptAndMoveEntities()
{
	entityUpdateVar0 = -1;

	exectueEntitiesUpdateFunction();

	for (int i = 0; i < totalActors; i++)
	{
		actorArray[i].m4C_scriptEntity->m68_oldPosition[0] = actorArray[i].m4C_scriptEntity->m20_position.vx >> 16;
		actorArray[i].m4C_scriptEntity->m68_oldPosition[1] = actorArray[i].m4C_scriptEntity->m20_position.vy >> 16;
		actorArray[i].m4C_scriptEntity->m68_oldPosition[2] = actorArray[i].m4C_scriptEntity->m20_position.vz >> 16;
	}
	if (fieldDebugDisable == 0)
	{
		assert(0); // "EVENT_CODE"
	}

	entityUpdateVar1 = 0;

	for (int i = 0; i < totalActors; i++)
	{
		sFieldEntity* pFieldEntity = &actorArray[i];
		sFieldScriptEntity* pFieldScriptEntity = pFieldEntity->m4C_scriptEntity;

		if ((actorArray[i].m58_flags & 0xF80) == 0x200)
		{
			if ((pFieldScriptEntity->m0_flags & 0x10001) == 0)
			{
				if ((pFieldScriptEntity->m4_flags & 0x600) != 0x200)
				{
					pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);
					updateEntityEventCode2(i, pFieldEntity, pFieldScriptEntity);
					updateEntityEventCode3(i, pFieldEntity, pFieldScriptEntity);
				}
			}
			else
			{
				MissingCode();
			}
		}
	}

	MissingCode();

	EntityMoveCheck0(playerControlledEntity, &actorArray[playerControlledEntity], actorArray[playerControlledEntity].m4C_scriptEntity);

	MissingCode();

	for (int i = 0; i < totalActors; i++)
	{
		if (actorArray[i].m58_flags & 0xF00)
		{
			sFieldScriptEntity* pFieldScriptEntity = actorArray[i].m4C_scriptEntity;
			if (((((pFieldScriptEntity->m4_flags & 0x600) != 0x200) && ((actorArray[i].m58_flags & 0xf80) == 0x200)) && ((pFieldScriptEntity->m0_flags & 0x10001) == 0)) &&
				((i != playerControlledEntity &&
					EntityMoveCheck1(i, 0x7fffffff, &actorArray[i], pFieldScriptEntity, 0) && (actorArray[i].m4_pVramSpriteSheet->m7C->mC == 1)))) {
				pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfffff7ff;
			}
		}
	}

	MissingCode();
}

int updateAllEntitiesSub2Var0 = 0;

uint stepInterpolateDirectionSub(int currentDirection, int targetDirection, int step)
{
	uint uVar1;

	if ((currentDirection - targetDirection & 0xfff) < 0x800) {
		uVar1 = currentDirection - step;
		if (0x7ff < ((currentDirection - step) - targetDirection & 0xfff)) {
			uVar1 = targetDirection;
		}
	}
	else {
		uVar1 = currentDirection + step;
		if (((currentDirection + step) - targetDirection & 0xfff) < 0x800) {
			uVar1 = targetDirection;
		}
	}
	return uVar1 & 0xfff;
}

uint stepInterpolateDirection(int currentDirection, int targetDirection, int step)
{
	if (updateAllEntitiesSub2Var0 == 0) {
		return stepInterpolateDirectionSub(currentDirection, targetDirection, step);
	}
	else {
		return targetDirection & 0xfff;
	}
}

short cameraShakeEnabled = 0;
VECTOR fieldCameraOffset = { 0,0,0,0 };

void updateCameraInterpolationSub2()
{
	if ((op99Var7 & 0x10) != 0) {
		assert(0);
	}
	if (op9DVar1 != 0) {
		op99Var5 = 1;
		op99Var6 = 1;
		op9DVar1--;
	}

	s32 iVar2;
	s32 iVar1 = op99Var5 * op99Var5;
	s32 iVar3 = op99Var6 * op99Var6;
	if ((cameraEye[0] >> 0x10 != cameraEye2.vx >> 0x10) && (iVar2 = cameraEye2.vx - cameraEye[0] >> 0x10, iVar3 <= iVar2 * iVar2)) {
		cameraEye[0] = cameraEye[0] + (cameraEye2.vx - cameraEye[0]) / op99Var6;
	}
	if ((cameraEye[2] >> 0x10 != cameraEye2.vz >> 0x10) && (iVar2 = cameraEye2.vz - cameraEye[2] >> 0x10, iVar3 <= iVar2 * iVar2)) {
		cameraEye[2] = cameraEye[2] + (cameraEye2.vz - cameraEye[2]) / op99Var6;
	}
	if ((cameraEye[1] >> 0x10 != cameraEye2.vy >> 0x10) && (iVar2 = cameraEye2.vy - cameraEye[1] >> 0x10, iVar3 <= iVar2 * iVar2)) {
		cameraEye[1] = cameraEye[1] + (cameraEye2.vy - cameraEye[1]) / op99Var6;
	}
	if ((cameraAt[0] >> 0x10 != cameraAt2.vx >> 0x10) && (iVar3 = cameraAt2.vx - cameraAt[0] >> 0x10, iVar1 <= iVar3 * iVar3)) {
		cameraAt[0] = cameraAt[0] + (cameraAt2.vx - cameraAt[0]) / op99Var5;
	}
	if ((cameraAt[2] >> 0x10 != cameraAt2.vz >> 0x10) && (iVar3 = cameraAt2.vz - cameraAt[2] >> 0x10, iVar1 <= iVar3 * iVar3)) {
		cameraAt[2] = cameraAt[2] + (cameraAt2.vz - cameraAt[2]) / op99Var5;
	}
	if ((cameraAt[1] >> 0x10 != cameraAt2.vy >> 0x10) && (iVar3 = cameraAt2.vy - cameraAt[1] >> 0x10, iVar1 <= iVar3 * iVar3)) {
		cameraAt[1] = cameraAt[1] + (cameraAt2.vy - cameraAt[1]) / op99Var5;
	}

	fieldCameraOffset.vx = 0;
	fieldCameraOffset.vy = 0;
	fieldCameraOffset.vz = 0;

	if (cameraShakeEnabled != 0) {
		assert(0);
	}
}

void updateCameraInterpolationSub0()
{
	MissingCode();
}

int checkCameraCollision(VECTOR*, std::array<s16, 6>&, std::array<s16, 4>&)
{
	MissingCode();
	return 0;
}

int cameraCollisionState = 0;

VECTOR* ApplyMatrixLV(MATRIX* m, VECTOR* v0, VECTOR* v1)

{
	uint uVar1;
	int iVar2;
	uint uVar3;
	int iVar4;
	uint uVar5;
	int iVar6;
	int iVar7;
	int iVar8;
	int iVar9;

	SetRotMatrix(m);
	uVar1 = v0->vx;
	uVar3 = v0->vy;
	uVar5 = v0->vz;
	iVar7 = (int)uVar1 >> 0xf;
	if ((int)uVar1 < 0) {
		iVar7 = -((int)-uVar1 >> 0xf);
		uVar1 = -(-uVar1 & 0x7fff);
	}
	else {
		uVar1 = uVar1 & 0x7fff;
	}
	iVar8 = (int)uVar3 >> 0xf;
	if ((int)uVar3 < 0) {
		iVar8 = -((int)-uVar3 >> 0xf);
		uVar3 = -(-uVar3 & 0x7fff);
	}
	else {
		uVar3 = uVar3 & 0x7fff;
	}
	iVar9 = (int)uVar5 >> 0xf;
	if ((int)uVar5 < 0) {
		iVar9 = -((int)-uVar5 >> 0xf);
		uVar5 = -(-uVar5 & 0x7fff);
	}
	else {
		uVar5 = uVar5 & 0x7fff;
	}
	setCopReg(2, 0x4800, iVar7);
	setCopReg(2, 0x5000, iVar8);
	setCopReg(2, 0x5800, iVar9);
	copFunction(2, 0x41e012);
	iVar7 = getCopReg(2, 0xc800);
	iVar8 = getCopReg(2, 0xd000);
	iVar9 = getCopReg(2, 0xd800);
	setCopReg(2, 0x4800, uVar1);
	setCopReg(2, 0x5000, uVar3);
	setCopReg(2, 0x5800, uVar5);
	copFunction(2, 0x49e012);
	if (iVar7 < 0) {
		iVar7 = iVar7 * 8;
	}
	else {
		iVar7 = iVar7 << 3;
	}
	if (iVar8 < 0) {
		iVar8 = iVar8 * 8;
	}
	else {
		iVar8 = iVar8 << 3;
	}
	if (iVar9 < 0) {
		iVar9 = iVar9 * 8;
	}
	else {
		iVar9 = iVar9 << 3;
	}
	iVar2 = getCopReg(2, 0xc800);
	iVar4 = getCopReg(2, 0xd000);
	iVar6 = getCopReg(2, 0xd800);
	v1->vx = iVar2 + iVar7;
	v1->vy = iVar4 + iVar8;
	v1->vz = iVar6 + iVar9;
	return v1;
}

void computeCameraEyeFromAt(VECTOR* param_1, VECTOR* param_2)
{
	MATRIX MStack80;
	VECTOR local_30;
	VECTOR local_20;

	PushMatrix();
	createRotationMatrix(&cameraRotationBetweenEyeAndAt, &MStack80);
	local_30.vx = param_2->vx - param_1->vx;
	local_30.vy = param_2->vy - param_1->vy;
	local_30.vz = param_2->vz - param_1->vz;
	ApplyMatrixLV(&MStack80, &local_30, &local_20);
	param_1->vx = local_20.vx + param_2->vx;
	param_1->vz = local_20.vz + param_2->vz;
	PopMatrix();
	return;
}

void updateCameraInterpolationSub1(VECTOR* param_1, int elevation)
{
	VECTOR local_48;
	local_48.vx = param_1->vx;
	local_48.vy = 0;
	local_48.vz = param_1->vz;
	std::array<s16, 6> collisionResult;
	std::array<s16, 4> collisionResult2;
	if (checkCameraCollision(&local_48, collisionResult, collisionResult2) == -1) {
		assert(0);
	}
	else {
		cameraAt2.vx = param_1->vx;
		cameraCollisionState = 0;
		cameraAt2.vz = param_1->vz;
		cameraAt2.vy = param_1->vy + -0x200000;
	}

	cameraEye2.vy = (getAngleSin((sceneDIP * 0x5b >> 3) + 0xc00) * sceneSCRZ * -0x20 >> 0x10) * (int)sceneScale * 0x10 + cameraAt2.vy;
	cameraEye2.vx = cameraAt2.vx;
	cameraEye2.vz = (getAngleCos((sceneDIP * 0x5b >> 3) + 0xc00) * sceneSCRZ * 0x20 >> 0x10) * (int)sceneScale * 0x10 + cameraAt2.vz;

	computeCameraEyeFromAt(&cameraEye2, &cameraAt2);

	if ((op99Var7 & 1) != 0) {
		if (op9DVar0 != 0) {
			op9DVar3 = op9DVar3 + op9DVar2;
			sceneScale = (short)((uint)op9DVar3 >> 0x10);
		}
		int uVar3 = (int)op9DVar0 - 1;
		op9DVar0 = (short)uVar3;
		if ((uVar3 & 0xffff) == 0) {
			op99Var7 = op99Var7 & 0xfffe;
		}
	}
	if ((op99Var7 & 8) != 0) {
		assert(0);
	}
}

void updateCameraInterpolation(void)
{
	switch (cameraInterpolationMode)
	{
	case 0:
		updateCameraInterpolationVar1 = 0;
		{
			int iVar1 = op99Var6;
			if ((updateCameraInterpolationVar0 & 3) == 0) {
				iVar1 = 8;
				if (8 < op99Var5) {
					iVar1 = op99Var5 + -2;
				}
				op99Var5 = iVar1;
				iVar1 = 8;
				if (8 < op99Var6) {
					iVar1 = op99Var6 + -2;
				}
			}
			op99Var6 = iVar1;
			updateCameraInterpolationVar0 = updateCameraInterpolationVar0 + 1;
		}
		break;
	case 1:
		updateCameraInterpolationVar0 = 0;
		updateCameraInterpolationVar1 = 0;
		if ((cameraInterpolationFlags & 1) != 0) {
			if (cameraInterpolationTargetNumSteps != 0) {
				cameraInterpolationTargetStartPosition[0] = cameraInterpolationTargetStartPosition[0] + cameraInterpolationTargetStep[0];
				cameraInterpolationTargetStartPosition[1] = cameraInterpolationTargetStartPosition[1] + cameraInterpolationTargetStep[1];
				cameraInterpolationTargetStartPosition[2] = cameraInterpolationTargetStartPosition[2] + cameraInterpolationTargetStep[2];
			}
			if ((--cameraInterpolationTargetNumSteps) == 0) {
				cameraInterpolationFlags &= ~1;
			}
			cameraAt2.vx = cameraInterpolationTargetStartPosition[0];
			cameraAt2.vy = cameraInterpolationTargetStartPosition[1];
			cameraAt2.vz = cameraInterpolationTargetStartPosition[2];
		}
		if ((cameraInterpolationFlags & 2) != 0) {
			if (cameraInterpolationPositionNumSteps != 0) {
				cameraInterpolationStartPosition[0] = cameraInterpolationStartPosition[0] + cameraInterpolationPositionStep[0];
				cameraInterpolationStartPosition[1] = cameraInterpolationStartPosition[1] + cameraInterpolationPositionStep[1];
				cameraInterpolationStartPosition[2] = cameraInterpolationStartPosition[2] + cameraInterpolationPositionStep[2];
			}
			if ((--cameraInterpolationPositionNumSteps) == 0) {
				cameraInterpolationFlags &= ~2;
			}
			cameraEye2.vx = cameraInterpolationStartPosition[0];
			cameraEye2.vy = cameraInterpolationStartPosition[1];
			cameraEye2.vz = cameraInterpolationStartPosition[2];
		}
		updateCameraInterpolationSub2();
		cameraRotationBetweenEyeAndAt.vy &= 0xfff;
		return;
	default:
		assert(0);
	}

	updateCameraInterpolationSub0();
	sFieldEntity* psVar2 = &actorArray[actorCameraTracked];
	VECTOR localPosition;
	localPosition.vx = (psVar2->m4C_scriptEntity->m20_position).vx;
	localPosition.vy = (psVar2->m4C_scriptEntity->m20_position).vy;
	localPosition.vz = (psVar2->m4C_scriptEntity->m20_position).vz;
	updateCameraInterpolationSub1(&localPosition, (int)psVar2->m4C_scriptEntity->m72_elevation);
	if ((op99Var7 & 0x4000) == 0) {
		SVECTOR walkmeshOutput1;
		VECTOR walkmeshOutput2;
		findTriangleInWalkMesh((cameraEye2.vx >> 16), (cameraEye2.vz >> 16), numWalkMesh + -1, &walkmeshOutput1, &walkmeshOutput2);
		if ((int)walkmeshOutput1.vy < (int)(cameraEye2.vy >> 16)) {
			cameraEye2.vy = (int)walkmeshOutput1.vy << 0x10;
		}
	}
	if (cameraInterpolationMode == 2) {
		int lVar3 = length2d((cameraAt2.vx >> 16) - (cameraAt[0] >> 16), (cameraAt2.vz >> 16) - (cameraAt[2] >> 16));
		int lVar4 = length2d((cameraEye2.vx >> 16) - (cameraEye[0] >> 16), (cameraEye2.vz >> 16) - (cameraEye[2] >> 16));
		if ((lVar3 < 0x80) && (lVar4 < 0x80)) {
			cameraInterpolationMode = 0;
		}
	}

	updateCameraInterpolationSub2();
	cameraRotationBetweenEyeAndAt.vy &= 0xfff;
}

struct sCurrentCameraVectors
{
	VECTOR m10_eye;
	VECTOR m20_at;
};

sCurrentCameraVectors* pCurrentCameraVectors = nullptr;

SVECTOR computeProjectionMatrixAngles = { 0,0,0 };
MATRIX computeProjectionMatrixTempMatrix;
MATRIX computeProjectionMatrixTempMatrix2;

void computeProjectionMatrix(void)
{
	VECTOR local_70;
	MATRIX MStack64;
	long alStack32[2];

	createRotationMatrix(&computeProjectionMatrixAngles, &computeProjectionMatrixTempMatrix);
	resetMatrixTranslation(&computeProjectionMatrixTempMatrix);
	CompMatrix(&computeProjectionMatrixTempMatrix, &cameraMatrix, &MStack64);
	copyMatrix(&cameraMatrix, &MStack64);
	createRotationMatrix(&cameraProjectionAngles, &computeProjectionMatrixTempMatrix2);
	resetMatrixTranslation(&computeProjectionMatrixTempMatrix2);
	createRotationMatrix(&cameraProjectionAngles, &currentProjectionMatrix);
	MulRotationMatrix(&cameraMatrix, &currentProjectionMatrix);
	SetRotMatrix(&cameraMatrix);
	SetTransMatrix(&cameraMatrix);
	RotTrans(&cameraRotation, &currentProjectionMatrix.t, alStack32);
	local_70.vx = worldScaleFactor;
	local_70.vy = worldScaleFactor;
	local_70.vz = worldScaleFactor;
	ScaleMatrix(&currentProjectionMatrix, &local_70);
	SetRotMatrix(&currentProjectionMatrix);
	SetTransMatrix(&currentProjectionMatrix);
	return;
}

void setProjectionMatrixForField(void)
{
	computeProjectionMatrix();
	SetRotMatrix(&currentProjectionMatrix);
	SetTransMatrix(&currentProjectionMatrix);
	if (fieldDebugDisable == 0) {
		assert(0);
	}
	return;
}

void updateAllEntities()
{
	updateScriptAndMoveEntities();

	createRotationMatrix(&renderModelRotationAngles, &renderModelRotationMatrix);
	renderModelRotationMatrix.t[2] = 0;
	renderModelRotationMatrix.t[1] = 0;
	renderModelRotationMatrix.t[0] = 0;

	cameraTan = ratan2(cameraAt[2] - cameraEye[2], cameraAt[0] - cameraEye[0]) + -0x400;
	camera2Tan = ratan2(cameraAt2.vz - cameraEye2.vz, cameraAt2.vx - cameraEye2.vx) + -0x400;
	cameraDeltaTan = ratan2(length2d(cameraAt[0] - cameraEye[0] >> 0x10, cameraAt[2] - cameraEye[2] >> 0x10), cameraAt[1] - cameraEye[1] >> 0x10);

	sCurrentCameraVectors cameraVectors;
	pCurrentCameraVectors = &cameraVectors;

	updateCameraInterpolation();

	int lVar6 = fieldCameraOffset.vz;
	int lVar5 = fieldCameraOffset.vy;
	int lVar7 = fieldCameraOffset.vx;
	int iVar9 = cameraEye[2];
	int iVar11 = cameraEye[1];
	int iVar12 = cameraEye[0];
	(pCurrentCameraVectors->m10_eye).vx = cameraEye[0];
	(pCurrentCameraVectors->m10_eye).vx = iVar12 + lVar7;
	iVar12 = cameraAt[0];
	(pCurrentCameraVectors->m10_eye).vy = iVar11;
	(pCurrentCameraVectors->m10_eye).vy = iVar11 + lVar5;
	iVar11 = cameraAt[1];
	(pCurrentCameraVectors->m10_eye).vz = iVar9;
	int iVar4 = cameraAt[2];
	(pCurrentCameraVectors->m20_at).vx = iVar12;
	(pCurrentCameraVectors->m20_at).vx = iVar12 + lVar7;
	(pCurrentCameraVectors->m10_eye).vz = iVar9 + lVar6;
	(pCurrentCameraVectors->m20_at).vy = iVar11;
	(pCurrentCameraVectors->m20_at).vz = iVar4;
	(pCurrentCameraVectors->m20_at).vy = iVar11 + lVar5;
	(pCurrentCameraVectors->m20_at).vz = iVar4 + lVar6;
	if (updateAllEntitiesSub2Var0 == 0) {
		cameraMatrix = cameraMatrix2;
		computeMatrix(&cameraMatrix2, &pCurrentCameraVectors->m10_eye, &pCurrentCameraVectors->m20_at, &cameraUp);
	}
	else {
		computeMatrix(&cameraMatrix, &pCurrentCameraVectors->m10_eye, &pCurrentCameraVectors->m20_at, &cameraUp);
		cameraMatrix2 = cameraMatrix;
	}

	setProjectionMatrixForField();

	for (int i = 0; i < totalActors; i++)
	{
		sFieldEntity* pActor = &actorArray[i];
		if (((pActor->m58_flags & 0xf40) != 0) && ((pActor->m58_flags & 0x20) == 0)) {
			sFieldScriptEntity* psVar10 = pActor->m4C_scriptEntity;
			u32 uVar7 = psVar10->m4_flags;
			if (((uVar7 & 0x100000) == 0) && ((uVar7 & 0x600) != 0x200)) {
				if ((psVar10->m0_flags & 0x8000) == 0) {
					s32 iVar9;
					s32 iVar8;
					s32 sVar5;
					if (((psVar10->m14_currentTriangleFlag & 0x200000U) == 0) || (iVar9 = 0x200, (psVar10->m0_flags & 0x1800) != 0)) {
						sVar5 = OPX47Var;
						if ((uVar7 & 0x2000) == 0) {
							sVar5 = psVar10->m11E;
						}
						iVar9 = (int)sVar5;
						sVar5 = psVar10->m108_rotation3;
						iVar8 = (int)psVar10->m106_currentRotation;
					}
					else {
						iVar8 = (((uint)psVar10->m14_currentTriangleFlag >> 0xb) - 2 & 7) << 9;
						sVar5 = psVar10->m108_rotation3;
					}
					sVar5 = stepInterpolateDirection(sVar5, iVar8, iVar9);
					psVar10->m108_rotation3 = sVar5;
				}
				if (renderCharsDisabled == '\0') {
					if ((psVar10->m4_flags & 0x1000000) == 0) {
						OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub2(pActor->m4_pVramSpriteSheet, cameraTan + pActor->m4C_scriptEntity->m108_rotation3);
					}
					else {
						OP_INIT_ENTITY_SCRIPT_sub0Sub7(pActor->m4_pVramSpriteSheet, pActor->m4C_scriptEntity->m108_rotation3);
					}
				}
			}
		}
	}

	if (fieldDebugDisable == 0) {
		assert(0);
	}
}

enum class eCameraTrackingMode : s16
{
	e0_followPlayer = 0,
};
eCameraTrackingMode cameraTrackingMode = eCameraTrackingMode::e0_followPlayer;

struct sUpdateCameraAtStruct
{
	s32 m0[4];
	s32 m10[4];
	s32 m20[4];
	s32 m30[4];
	s32 m40[4];
	s32 m50[4];
};

void updateCameraAt(VECTOR* pCameraAt)
{
	sUpdateCameraAtStruct local_98;
	for (int i = 0; i < 3; i++)
	{
		local_98.m0[i] = 0xFFFF;
		local_98.m10[i] = -1;
		local_98.m40[i] = 0;
		local_98.m30[i] = 0;
		local_98.m50[i] = 0;
		local_98.m20[i] = 0;
	}

	for (int i = 0; i < totalActors; i++)
	{
		MissingCode();
	}

	MissingCode();
}

void updateCamera()
{
	switch (cameraTrackingMode)
	{
	case eCameraTrackingMode::e0_followPlayer: // follow player
		updateCameraAt(&actorArray[playerControlledEntity].m4C_scriptEntity->m20_position);
		break;
	default:
		assert(0);
	}
}


MATRIX occlusionTestMatrix;

bool isObjectOcluded(sFieldEntitySub0* param_1)
{
	short sVar1;
	short sVar2;
	int iVar3;
	int iVar4;
	VECTOR local_48;
	SVECTOR local_38;
	long lStack48;
	sVec2_s16 local_2c;
	long alStack40[2];

	RotTrans(&param_1->m18, &local_48, &lStack48);
	occlusionTestMatrix.t[0] = local_48.vx;
	occlusionTestMatrix.t[1] = local_48.vy;
	occlusionTestMatrix.t[2] = local_48.vz;
	SetRotMatrix(&occlusionTestMatrix);
	SetTransMatrix(&occlusionTestMatrix);
	sVar1 = param_1->m20;
	local_38.vz = 0;
	local_38.vx = -sVar1;
	local_38.vy = local_38.vx;
	RotTransPers(&local_38, &local_2c, alStack40, &lStack48);
	local_38.vz = 0;
	iVar4 = local_2c.vy;
	sVar2 = local_2c.vx;
	local_38.vx = sVar1;
	local_38.vy = sVar1;
	RotTransPers(&local_38, &local_2c, alStack40, &lStack48);
	iVar3 = -1;
	if (((iVar4 < OPX8E_param1 + 0xe0) && (-OPX8E_param1 < local_2c.vy)) && ((int)sVar2 < OPX8E_param0 + 0x140)) {
		iVar3 = -(-OPX8E_param0 < local_2c.vx ^ 1);
	}

	Hack("Force isObjectOcluded to return false");
	return 0;
	return iVar3;
}

int isObjectClipped(sModelBlock* param_1, uint param_2)
{
	MissingCode();
	return 0;
}

bool submitModelForRendering(sModelBlock* param_1, std::vector<sTag*>& param_2, std::array<sTag, 4096>& OT, int renderMode)
{
	if ((objectClippingMask != 0) && (isObjectClipped(param_1, objectClippingMask))) {
		return 0;
	}

	currentModeBlock18 = param_1->m18.begin();
	currentModelBlockNormals = param_1->m_model->mRawData.begin() + param_1->mC_offsetNormals;
	currentModelBlockVertices = param_1->m_model->mRawData.begin() + param_1->m8_offsetVertices;
	fieldPolyCount2 += param_1->m4_numPrims;
	currentModelInstanceArray8 = param_2.begin();
	currentOTEntry = &OT;

	int numMeshBlockLeft = param_1->m6_numMeshBlock;
	std::vector<u8>::iterator currentModelBlockSubBlocks = param_1->m_model->mRawData.begin() + param_1->m10_offsetMeshBlocks;
	while (numMeshBlockLeft = numMeshBlockLeft - 1, numMeshBlockLeft != 0xffffffff) {
		int primType = READ_LE_U8(currentModelBlockSubBlocks);
		t_primRenderFunc primRenderFunc = nullptr;
		switch (renderMode) {
		case 0:
			primRenderFunc = polyRenderDefs[primType].m0_type0;
			break;
		case 1:
			primRenderFunc = polyRenderDefs[primType].m4_type1;
			break;
		case 2:
			primRenderFunc = polyRenderDefs[primType].m8_type2;
			break;
		case 3:
			primRenderFunc = polyRenderDefs[primType].mC_type3;
			break;
		case 4:
			primRenderFunc = polyRenderDefs[primType].m10_type4;
			break;
		case 5:
			primRenderFunc = polyRenderDefs[primType].m14_type5;
			break;
		default:
			assert(0);
		}
		g_currentModelBlockSubBlocks = currentModelBlockSubBlocks + 4;
		if (primRenderFunc)
		{
			primRenderFunc(g_currentModelBlockSubBlocks, READ_LE_U16(currentModelBlockSubBlocks + 2));
		}
		currentModelBlockSubBlocks = g_currentModelBlockSubBlocks + READ_LE_U16(currentModelBlockSubBlocks + 2) * polyRenderDefs[primType].m1C_size;
	}
	g_currentModelBlockSubBlocks = currentModelBlockSubBlocks;
	return 1;
}

char fieldObjectRenderingVar3 = 0;

void renderObjects()
{
	fieldPolyCount = 0;
	fieldPolyCount2 = 0;

	if (isFogSetup != 0) {
		MissingCode();
	}

	VECTOR scale1 = { 0x800, 0x800, 0x800 };
	VECTOR scale2 = { worldScaleFactor , worldScaleFactor , worldScaleFactor };

	ScaleMatrix(&worldScaleMatrix, &scale2);

	MATRIX fieldLocalWorldMatrix;
	CompMatrix(&currentProjectionMatrix, &renderModelRotationMatrix, &fieldLocalWorldMatrix);

	objectClippingMask = 0;
	fieldObjectRenderingVar1.vy = fieldObjectRenderingVar1.vy + fieldObjectRenderingVar2.vz;
	fieldObjectRenderingVar1.vx = fieldObjectRenderingVar1.vx + fieldObjectRenderingVar2.vx;
	fieldObjectRenderingVar1.vz = fieldObjectRenderingVar1.vz + fieldObjectRenderingVar2.vy;

	for (int i=0; i<totalObjects; i++)
	{
		MATRIX rotationMatrix;
		MATRIX projectedMatrix;

		sFieldEntity* pFieldEntity = &actorArray[i];

		pFieldEntity->m2C_matrixBackup = pFieldEntity->mC_matrix;
		bool forceVisibility = false;

		if(!(pFieldEntity->m58_flags & 0x40))
		{
			if (i < totalActors)
			{
				SVECTOR rotationAxis;
				switch (pFieldEntity->m4C_scriptEntity->m12C_flags & 3)
				{
				case 0:
					if (pFieldEntity->m4C_scriptEntity->m128 == -1)
					{
						if (pFieldEntity->m4C_scriptEntity->m75 == -1)
						{
							goto LAB_Field__80074d80;
						}
						else
						{
							CompMatrix(&currentProjectionMatrix, &actorArray[pFieldEntity->m4C_scriptEntity->m75].m2C_matrixBackup, &rotationMatrix);
							CompMatrix(&rotationMatrix, &actorArray[i].mC_matrix, &projectedMatrix);
							CompMatrix(&actorArray[pFieldEntity->m4C_scriptEntity->m75].m2C_matrixBackup, &actorArray[i].mC_matrix, &actorArray[i].m2C_matrixBackup);
						}
					}
					else
					{
						assert(0);
					}
					break;
				case 1:
					rotationAxis = { pFieldEntity->m4C_scriptEntity->m70_rotationForRendering, 0,0 };
					createRotationMatrix(&rotationAxis, &rotationMatrix);
					MulRotationMatrix(&actorArray[i].mC_matrix, &rotationMatrix);
					rotationMatrix.t = actorArray[i].mC_matrix.t;
					CompMatrix(&currentProjectionMatrix, &rotationMatrix, &projectedMatrix);
					break;
				case 2:
					rotationAxis = { 0, pFieldEntity->m4C_scriptEntity->m70_rotationForRendering,0 };
					createRotationMatrix(&rotationAxis, &rotationMatrix);
					MulRotationMatrix(&actorArray[i].mC_matrix, &rotationMatrix);
					rotationMatrix.t = actorArray[i].mC_matrix.t;
					CompMatrix(&currentProjectionMatrix, &rotationMatrix, &projectedMatrix);
					break;
				case 3:
					rotationAxis = { 0, 0, pFieldEntity->m4C_scriptEntity->m70_rotationForRendering };
					createRotationMatrix(&rotationAxis, &rotationMatrix);
					MulRotationMatrix(&actorArray[i].mC_matrix, &rotationMatrix);
					rotationMatrix.t = actorArray[i].mC_matrix.t;
					CompMatrix(&currentProjectionMatrix, &rotationMatrix, &projectedMatrix);
					break;
				default:
					assert(0); // else projectedMatrix is uninitialized
				}
			}
			else
			{
				if ((fieldObjectRenderingVar3 & 0x7f) == 0) {
					(pFieldEntity->mC_matrix).t[0] = ((int)(pFieldEntity->mC_matrix).t[0] + (int)fieldObjectRenderingVar2.vx);
					(pFieldEntity->mC_matrix).t[1] = ((int)(pFieldEntity->mC_matrix).t[1] + (int)fieldObjectRenderingVar2.vz);
					(pFieldEntity->mC_matrix).t[2] = ((int)(pFieldEntity->mC_matrix).t[2] + (int)fieldObjectRenderingVar2.vy);
				}

LAB_Field__80074d80:
				if ((fieldObjectRenderingVar3 & 0x7f) == 1) {
					(pFieldEntity->mC_matrix).t[0] = ((int)(pFieldEntity->mC_matrix).t[0] + (int)fieldObjectRenderingVar2.vx);
					(pFieldEntity->mC_matrix).t[1] = ((int)(pFieldEntity->mC_matrix).t[1] + (int)fieldObjectRenderingVar2.vz);
					(pFieldEntity->mC_matrix).t[2] = ((int)(pFieldEntity->mC_matrix).t[2] + (int)fieldObjectRenderingVar2.vy);
				}

				forceVisibility = (fieldObjectRenderingVar3 & 0x80) != 0;

				MulMatrix0(&fieldLocalWorldMatrix, &pFieldEntity->mC_matrix, &projectedMatrix);
				setCopControlWord(2, 0x2800, fieldLocalWorldMatrix.t[0]);
				setCopControlWord(2, 0x3000, fieldLocalWorldMatrix.t[1]);
				setCopControlWord(2, 0x3800, fieldLocalWorldMatrix.t[2]);

				setCopReg(2, 0, sVec2_s16::fromValue(pFieldEntity->mC_matrix.t[0], pFieldEntity->mC_matrix.t[1]));
				setCopReg(2, 1, sVec2_s16::fromValue(pFieldEntity->mC_matrix.t[2], 0));
				copFunction(2, 0x480012);
				projectedMatrix.t[0] = getCopReg(2, 0x19);
				projectedMatrix.t[1] = getCopReg(2, 0x1a);
				projectedMatrix.t[2] = getCopReg(2, 0x1b);

				if ((actorArray[i].m58_flags & 3) != 0) {
					if ((actorArray[i].m58_flags & 3) == 1) {
						MulMatrix0(&worldScaleMatrix, &actorArray[i].mC_matrix, &projectedMatrix);
					}
					else {
						copyRotationMatrix(&projectedMatrix, &actorArray[i].mC_matrix);
						ScaleMatrix(&projectedMatrix, &scale2);
					}
					MulRotationMatrix(&computeProjectionMatrixTempMatrix, &projectedMatrix);
				}
			}

			if (pFieldEntity->m0->m12_renderMode == 1)
			{
				assert(0);
			}
			objectClippingMask = 0;

			if (!(actorArray[i].m58_flags & 0x20))
			{
				if ((actorArray[i].m58_flags & 0x2000) && (pFieldEntity->m0->m14))
				{
					assert(0);
				}

				SetRotMatrix(&projectedMatrix);
				SetTransMatrix(&projectedMatrix);

				if (!isObjectOcluded(pFieldEntity->m0) || forceVisibility) {
					SetRotMatrix(&projectedMatrix);
					SetTransMatrix(&projectedMatrix);

					if ((pFieldEntity->m58_flags & 0x8000) == 0) {
						submitModelForRendering(&pFieldEntity->m0->m4_pModelBlock[0], pFieldEntity->m0->m8[g_frameOddOrEven], pCurrentFieldRenderingContext->mCC_OT, pFieldEntity->m0->m12_renderMode);
					}
					else {
						submitModelForRendering(&pFieldEntity->m0->m4_pModelBlock[0], pFieldEntity->m0->m8[g_frameOddOrEven], pCurrentFieldRenderingContext->m40D0_secondaryOT, pFieldEntity->m0->m12_renderMode);
					}
				}
			}
		}
	}

	if (fieldDebugDisable == 0) {
		assert(0);
	}
}

void uploadCharacterSpriteSub1(sFieldEntitySub4* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	sPS1Pointer puVar16 = param_3->m0;
	sPS1Pointer pbVar13 = puVar16 + READ_LE_U16(puVar16 + param_2 * 2);
	int iVar23 = READ_LE_U8(pbVar13 + 3) * param_1->m2C_scale;
	int bVar2 = READ_LE_U8(pbVar13);
	sPS1Pointer local_54 = pbVar13 + 4;
	sPS1Pointer pbVar18 = pbVar13 + (bVar2 & 0x3f) * 2 + 4;
	sFieldEntitySub4_B4_sub* psVar13 = param_1->m20->m30;
	short sVar8 = (short)(iVar23 >> 0xc);
	if (iVar23 < 0) {
		sVar8 = (short)(iVar23 + 0xfff >> 0xc);
	}
	param_1->m36 = sVar8;
	iVar23 = READ_LE_U8(pbVar13 + 1) * param_1->m2C_scale;
	if (iVar23 < 0) {
		iVar23 = iVar23 + 0xfff;
	}
	param_1->m38 = (short)(iVar23 >> 0xc);
	u8 bVar3 = param_1->m3C & 0xFF;
	u16 local_50 = param_3->m4_vramLocation.vy & 0xFF;
	iVar23 = param_1->m28;
	u32 uVar21 = 4;
	u32 uVar22 = 0;

	if ((bVar2 & 0x3f) != 0) {
		do {
			psVar13->m9 = 0;
			psVar13->m8 = 0;
			psVar13->m14 = psVar13->m14 & 0xffffffdf;
			while (true) {
				u8 bVar4 = READ_LE_U8(pbVar18);
				if ((bVar4 & 0x80) == 0) break;
				sPS1Pointer pbVar19 = pbVar18 + 1;
				if ((bVar4 & 0x40) == 0) {
					if ((bVar4 & 4) != 0) {
						psVar13->m14 = psVar13->m14 | 0x20;
					}
					if ((bVar4 & 1) != 0) {
						psVar13->m8 = READ_LE_U8(pbVar19);
						pbVar19 = pbVar18 + 2;
					}
					pbVar18 = pbVar19;
					if ((bVar4 & 2) != 0) {
						bVar4 = READ_LE_U8(pbVar19);
						pbVar18 = pbVar18 + 1;
						psVar13->m9 = bVar4;
					}
				}
				else {
					uVar21 = bVar4 & 7;
					if (param_1->m20->m34 == nullptr) {
						std::array<sFieldEntitySub4_124, 8>* pasVar10 = new std::array<sFieldEntitySub4_124, 8>;
						param_1->m20->m34 = pasVar10;
						OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
					}
					if ((bVar4 & 0x20) != 0) {
						(*param_1->m20->m34)[uVar21].m0 = READ_LE_U8(pbVar19);
						(*param_1->m20->m34)[uVar21].m1 = READ_LE_U8(pbVar18 + 2);
						pbVar19 = pbVar18 + 3;
					}
					pbVar18 = pbVar19;
					if ((bVar4 & 0x10) == 0) {
						(*param_1->m20->m34)[uVar21].m6 = 0;
					}
					else {
						bVar4 = READ_LE_U8(pbVar18);
						pbVar18 = pbVar18 + 1;
						(*param_1->m20->m34)[uVar21].m6 = (ushort)bVar4 << 4;
					}
				}
			}
			sPS1Pointer pbVar19 = puVar16 + READ_LE_U16(local_54);
			u8 bVar4 = READ_LE_U8(pbVar19);
			u32 uVar17 = (uint)bVar4;
			local_54 = local_54 + 2;
			s8 cVar20;
			if ((bVar4 & 1) == 0) {
				psVar13->m14 = psVar13->m14 & 0xfffffff7;
				cVar20 = (char)(((ushort)(param_3->m4_vramLocation).vx & 0x3f) >> 2);
			}
			else {
				psVar13->m14 = psVar13->m14 | 8;
				cVar20 = (char)(((ushort)(param_3->m4_vramLocation).vx & 0x3f) >> 1);
			}
			u8 bVar5 = READ_LE_U8(pbVar18);
			u8 bVar7 = bVar5 >> 4;
			u8 uVar15 = bVar7 & 3;
			psVar13->m10 = iVar23;
			if (((bVar7 & 3) != 0) || (uVar15 = (uint)(bVar3 >> 5), uVar15 != 0)) {
				uVar15 = uVar15 - 1;
				*(byte*)((int)&psVar13->m10 + 3) = *(byte*)((int)&psVar13->m10 + 3) | 2;
			}
			if (((int)uVar17 >> 4 & 1U) == 0) {
				sPS1Pointer iVar12;
				if (((param_1->mA8.m0) == 1) && (iVar12 = param_1->m7C->m18, iVar12.getPointer() != 0)) {
					sPS1Pointer puVar11 = iVar12 + ((uVar17 & 0xe) * 2);
					u16 uVar6 = READ_LE_U16(puVar11 + 2);
					u16 uVar14 = READ_LE_U16(puVar11);
					local_50 = uVar6 & 0xff;
					cVar20 = (char)((uVar14 & 0x3f) >> 2);
					psVar13->mA_tpage = (ushort)((uVar17 & 1) << 7) | (ushort)((uVar15 & 3) << 5) | (short)(uVar6 & 0x100) >> 4 | (ushort)((uVar14 & 0x3ff) >> 6) | (ushort)((uVar6 & 0x200) << 2);
				}
				else {
					u16 uVar6 = (param_3->m4_vramLocation).vy;
					psVar13->mA_tpage =
						(ushort)((uVar17 & 1) << 7) | (ushort)((uVar15 & 3) << 5) | (short)(uVar6 & 0x100) >> 4 | (ushort)((int)((int)(param_3->m4_vramLocation).vx + (uVar17 & 0xe) * 0x20 & 0x3ff) >> 6) |
						(ushort)((uVar6 & 0x200) << 2);
				}
				psVar13->mC_clut = (param_3->m8_clut).vy << 6 | (ushort)((int)((int)(param_3->m8_clut).vx + (bVar5 & 0xf) * 0x10) >> 4) & 0x3f;
			}
			else {
				assert(0);
				/*                pbVar19 = pbVar19 + 1;
								uVar17 = (uint)CONCAT11(*pbVar19, bVar4);
								uVar9 = GetTPage(uVar17 & 1, (int)(short)uVar15, (int)tpageLocationTable[uVar17 & 0xe], (int)tpageLocationTable[(uVar17 & 0xe) + 1]);
								psVar13->mA_tpage = uVar9;
								uVar9 = GetClut((int)uVar17 >> 1 & 0xf0, ((int)uVar17 >> 9 & 0xfU) + 0x1cc);
								psVar13->mC_clut = uVar9;
								*/
			}
			psVar13->m14 = psVar13->m14 & 0xfffffff8U | uVar21;
			psVar13->m4 = cVar20 + READ_LE_U8(pbVar19 + 1);
			psVar13->m5 = (char)local_50 + READ_LE_U8(pbVar19 + 2);
			psVar13->m6 = READ_LE_U8(pbVar19 + 3);
			psVar13->m7 = READ_LE_U8(pbVar19 + 4);
			psVar13->m14 = psVar13->m14 & 0xffffffefU | READ_LE_U8(pbVar18) >> 2 & 0x10;
			if ((bVar2 & 0x80) == 0) {
				psVar13->m0 = READ_LE_S8(pbVar18 + 1);
				psVar13->m2 = READ_LE_S8(pbVar18 + 2);
			}
			else {
				psVar13->m0 = (s16)READ_LE_U8(pbVar18 + 1) | (ushort)(((uint)READ_LE_S8(pbVar18 + 2) << 0x18) >> 0x10);
				pbVar19 = pbVar18 + 3;
				sPS1Pointer pbVar1 = pbVar18 + 4;
				pbVar18 = pbVar18 + 2;
				psVar13->m2 = (ushort)READ_LE_U8(pbVar19) | (ushort)(((uint)READ_LE_U8(pbVar1) << 0x18) >> 0x10);
			}
			psVar13 = psVar13 + 1;
			uVar22 = uVar22 + 1;
			pbVar18 = pbVar18 + 3;
		} while (uVar22 != (bVar2 & 0x3f));
	}
	param_1->m40 = param_1->m40 & 0xffffff03 | (uVar22 & 0x3f) << 2;
}

void uploadCharacterSprite(sFieldEntitySub4* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	param_1->m40 = param_1->m40 & 0xfff5ffff;
	sPS1Pointer local_50 = param_3->m0;
	sFieldEntitySub4_B4_sub* pFieldEntitySub4_B4_sub = param_1->m20->m30;
	if (param_2 < (int)((READ_LE_U16(local_50) & 0x1ff) + 1)) {
		if ((param_1->m3C >> 0x1e & 1) != 0) {
			param_1->m3C = param_1->m3C & 0xbfffffff;
			sPS1Pointer puVar10 = param_3->mC;
			if (READ_LE_U16(puVar10) != 0) {
				u16 local_58 = (param_3->m8_clut).vx;
				u16 local_56 = (param_3->m8_clut).vy;
				u16 local_52 = 1;
				u16 local_54 = READ_LE_U16(puVar10) << 4;

				// Transfer the clut
				sPS1Pointer clutData = puVar10 + (uint)READ_LE_U16(puVar10) * ((param_1->m3C >> 16) & 0xf0) * 2 + 4;
				int width = ((uint)READ_LE_U16(puVar10) << 0x14) >> 0x10;
				addToShapeTransfertTable(clutData, param_3->m8_clut.vx, param_3->m8_clut.vy, width, 1);
			}
		}
		if ((READ_LE_U16(local_50) & 0x8000) == 0) {
			assert(0);
		}
		else
		{
			uploadCharacterSpriteSub1(param_1, param_2, param_3);
		}
	}
}

void uploadCharacterSprites()
{
	sFieldEntitySub4* psVar1;

	psVar1 = spriteTransfertListHead;
	if (spriteTransfertListHead != (sFieldEntitySub4*)0x0) {
		do {
			if ((ushort)psVar1->m34 == 0) {
				psVar1->m40 = psVar1->m40 & 0xffffff03;
			}
			else {
				uploadCharacterSprite(psVar1, (uint)(ushort)psVar1->m34, psVar1->m24);
			}
			psVar1 = psVar1->m20->m38;
		} while (psVar1 != (sFieldEntitySub4*)0x0);
	}
	spriteTransfertListHead = (sFieldEntitySub4*)0x0;
}

int frameOddOrEven = 0;

void clearShapeTransfertTableEntry(int param_1)
{
	void* ppvVar1 = shapeTransfertTemporaryBuffersLinkedLists[param_1];
	shapeTransfertTableStart = shapeTransfertBuffer[param_1];
	shapeTransfertTableEnd = shapeTransfertTableStart + shapeTransfertTableSize;
	shapeTransfertTableCurrentEntry = shapeTransfertTableStart;
	shapeTransfertDoubleBufferIndex = param_1;
	while (ppvVar1 != nullptr) {
		assert(0);
		/*free(*ppvVar1);
		ppvVar1 = (void**)ppvVar1[1];*/
	}
	shapeTransfertTemporaryBuffersLinkedLists[param_1] = nullptr;
	return;
}

void renderChars()
{
	if (renderCharsDisabled != 1) {
		clearShapeTransfertTableEntry(frameOddOrEven);
		MissingCode();
		uploadCharacterSprites();
		MissingCode();
	}
}

void renderParticles()
{
	MissingCode();
}

void ClearImage(RECT* pRect, u8 r, u8 g, u8 b)
{
	MissingCode();
}

void shapeTransfert()
{
	sShapeTransfert* rect;

	rect = shapeTransfertTable[shapeTransfertDoubleBufferIndex];
	while (rect != (sShapeTransfert*)0x0) {
		if (rect->m8_pData.getPointer() == nullptr) {
			ClearImage(&rect->m0_rect, '\0', '\0', '\0');
		}
		else {
			LoadImage(&rect->m0_rect, rect->m8_pData);
		}
		rect = rect->mC_pNext;
	}
	shapeTransfertTable[shapeTransfertDoubleBufferIndex] = (sShapeTransfert*)0x0;
}

void AddPrims(sTag* ot, sTag* p0, sTag* p1)
{
	p1->m0_pNext = ot->m0_pNext;
	ot->m0_pNext = p0;
}

void linkOT(sTag* a, std::array<sTag, 4096>& b, int index)
{
	AddPrims(a, &b[index], &b[0]);
}

void updateAndRenderField()
{
	MissingCode();
	updateAllEntities();
	updateCamera();
	if (fieldDebugDisable == 0) {
		assert(0);
	}
	MissingCode(); //updateScreenEffectSlots(_DAT_800c426c[0x166].dr_env.code + 3, frameOddOrEven);
	if (fieldDebugDisable == 0) {
		assert(0);
	}
	renderCompass();
	MissingCode();
	renderObjects();
	renderChars();
	renderParticles();
	MissingCode();
	DrawSync(0);
	stepDialogWindows();
	addDialogWindowsToOT(&pCurrentFieldRenderingContext->m80D4_uiOT[0], g_frameOddOrEven);
	VSync(0);
	MissingCode();
	shapeTransfert();
	MissingCode();

	if (updateAllEntitiesSub2Var0 == 0) {
		if (fieldUseGroundOT != 0) {
			linkOT(&pCurrentFieldRenderingContext->mCC_OT[linkOTIndex], pCurrentFieldRenderingContext->m40D0_secondaryOT, linkOTIndex);
		}
		linkOT(&pCurrentFieldRenderingContext->m80D4_uiOT[7], pCurrentFieldRenderingContext->mCC_OT, linkOTIndex);
	}
	DrawOTag(&pCurrentFieldRenderingContext->m80D4_uiOT[7]);

	MissingCode();
}

int runningOnDTL = -1;
int startOfUpdateFieldTime = 0;

void logFieldRenderingEvent(char* param_1)
{
	MissingCode();
}

int loadInputFromVSyncBuffer()
{
	int iVar1;
	uint uVar2;

	if (inputReplayPosition == 0) {
		iVar1 = 0;
	}
	else {
		uVar2 = inputReplayOffset & 0xf;
		newPadButtonForScripts[0].vx = inputReplayBuffer.m0_newPadButtonForScripts0[uVar2];
		newPadButtonForScripts[1].vx = inputReplayBuffer.m20_newPadButtonForScripts1[uVar2];
		newPadButtonForDialogs = inputReplayBuffer.m40_newPadButtonForDialogs[uVar2];
		newPadButtonForDialogs2 = inputReplayBuffer.m60_newPadButtonForDialogs2[uVar2];
		newPadButtonForField = inputReplayBuffer.m80_newPadButtonForField[uVar2];
		newPadButtonForField2 = inputReplayBuffer.mA0_newPadButtonForField2[uVar2];
		inputReplayOffset = inputReplayOffset + 1;
		iVar1 = inputReplayPosition;
		inputReplayPosition = inputReplayPosition + -1;
	}
	return iVar1;
}

/*

// Pad terminal type masks. First four bits of padData[1]
#define PADTT_DIG 0x40 // 16 button controller (no analog)
#define PADTT_ANA 0x70 // Analog controller

// All 16 16bit button bit masks. Button state bit is 0 if pressed.
#define PADLeft 0x8000
#define PADDown 0x4000
#define PADRight 0x2000
#define PADUp 0x1000
#define PADStart 0x800
#define PADR3 0x400
#define PADL3 0x200
#define PADSelect 0x100
#define PADSquare 0x80
#define PADCross 0x40
#define PADCircle 0x20
#define PADTriangle 0x10
#define PADRB1 0x8
#define PADLB1 0x4
#define PADRB2 0x2
#define PADLB2 0x1

*/

void getInputDuringVsync(void)
{
	MissingCode();

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		if (keyState[i])
		{
			u16 buttonMask = 0;
			switch (i)
			{
			case SDL_SCANCODE_Z:
				buttonMask = 0x40; // CROSS
				break;
			case SDL_SCANCODE_X:
				buttonMask = 0x20; // CIRCLE
				break;
			case SDL_SCANCODE_A:
				buttonMask = 0x80; // SQUARE
				break;
			case SDL_SCANCODE_S:
				buttonMask = 0x10; // TRIANGLE
				break;
			case SDL_SCANCODE_UP:
				buttonMask = 0x1000;
				break;
			case SDL_SCANCODE_RIGHT:
				buttonMask = 0x2000;
				break;
			case SDL_SCANCODE_DOWN:
				buttonMask = 0x4000;
				break;
			case SDL_SCANCODE_LEFT:
				buttonMask = 0x8000;
				break;
			default:
				break;
			}

			newPadButtonForScripts[0].vx |= buttonMask;
		}
	}
}

void saveInputs()
{
	ushort uVar1;
	ushort uVar2;
	ushort uVar3;
	ushort uVar4;
	short sVar5;
	uint uVar6;

	sVar5 = newPadButtonForScripts[1].vx;
	uVar4 = newPadButtonForField2;
	uVar3 = newPadButtonForField;
	uVar2 = newPadButtonForDialogs2;
	uVar1 = newPadButtonForDialogs;
	if (inputReplayPosition < 0x10) {
		uVar6 = inputReplayPosition2 & 0xf;
		inputReplayPosition2 = inputReplayPosition2 + 1;
		inputReplayPosition = inputReplayPosition + 1;
		inputReplayBuffer.m0_newPadButtonForScripts0[uVar6] = newPadButtonForScripts[0].vx;
		inputReplayBuffer.m20_newPadButtonForScripts1[uVar6] = sVar5;
		inputReplayBuffer.m40_newPadButtonForDialogs[uVar6] = uVar1;
		inputReplayBuffer.m60_newPadButtonForDialogs2[uVar6] = uVar2;
		inputReplayBuffer.m80_newPadButtonForField[uVar6] = uVar3;
		inputReplayBuffer.mA0_newPadButtonForField2[uVar6] = uVar4;
	}
	else {
		inputOverflowed = 1;
	}
}

void incrementTime()
{
	MissingCode();
}

u32 numVsync = 0;
void vsyncCallback(void)
{
	numVsync++;
	getInputDuringVsync();
	saveInputs();
	incrementTime();
	MissingCode();
}

void updateFieldInputs()
{
	padButtonForScripts[0].vx = 0;
	padButtonForScripts[1].vx = 0;
	padButtonForDialogs = 0;
	padButtonForDialogs2 = 0;
	padButtonForField = 0;
	padButtonForField2 = 0;
	while (loadInputFromVSyncBuffer()) {
		padButtonForScripts[0].vx |= newPadButtonForScripts[0].vx & inputAllowedMask2;
		padButtonForScripts[1].vx |= newPadButtonForScripts[1].vx;
		padButtonForDialogs |= newPadButtonForDialogs & inputAllowedMask2;
		padButtonForDialogs2 |= newPadButtonForDialogs2;
		padButtonForField |= newPadButtonForField & inputAllowedMask2;
		padButtonForField2 |= newPadButtonForField2;
	}
	padButtonForScripts[0].vx &= inputAllowedMask;
	padButtonForField &= inputAllowedMask;
	padButtonForDialogs &= inputAllowedMask;
	resetInputs();
	//FUN_Field__8007ae78(1, &DAT_80065848);
	MissingCode();
	if (updateAllEntitiesSub2Var0 != 0) {
		padButtonForScripts[0].vx = 0;
		padButtonForScripts[1].vx = 0;
		padButtonForDialogs = 0;
		padButtonForDialogs2 = 0;
		padButtonForField = 0;
		padButtonForField2 = 0;
	}
	if (playMusicAuthorized == 0) {
		padButtonForDialogs = padButtonForDialogs & 0xff7f;
	}
}

void syncKernelAndFieldStates()
{
	MissingCode();
}

void fieldPerFrameReset()
{
	startOfUpdateFieldTime = VSync(1);
	clearFieldOrderingTable();
	updateFieldInputs();
	if (fieldDebugDisable == 0) {
		logFieldRenderingEvent("Clear OTAG");
	}
	syncKernelAndFieldStates();
}

int updateMusicState2(int param_1)
{
	MissingCode();
	return 0;
}

void updateMusicState()
{
	rand();
	if (fieldMusicLoadPending == -1) {
		fieldMusicLoadPending = updateMusicState2(currentlyPlayingMusic);
	}
	if (updateAllEntitiesSub2Var0 != 0) {
		updateAllEntitiesSub2Var0 = updateAllEntitiesSub2Var0 + -1;
	}
}

void fieldEntryPoint()
{
	fieldDebugDisable = (runningOnDTL == -1);
	ClearCacheAfterOverlayLoad();
	if (fieldDebugDisable == 0) {
		assert(0);
		//DrawSyncCallback(fieldDebuggerDrawSyncCallback);
	}

	MissingCode();

	setCurrentDirectory(4, 0); // TODO: this is not explicitly called at this level

	allocatePartyCharacterBuffers();

	MissingCode();

	fieldExecuteVar3 = -1;
	fieldExecuteVar2 = -1;
	playMusicAuthorized = -1;

	MissingCode();

	fieldMapNumber = gameState.m231A_fieldID;
	pKernelGameState = &gameState;

	MissingCode();

	if (fieldDebugDisable == 1) {
		gameState.m1930_fieldVarsBackup[41] = 1;
		setVar(0x50, 1);
	}
	initCompassData();
	fieldRequestedGears = 0;
	startLoadingPlayableCharacters();
	finalizeLoadPlayableCharacters();

	MissingCode();

	bootField();

	MissingCode();

	while (!noahFrame_end())
	{
		noahFrame_start();

		MissingCode();
		////
		fieldPerFrameReset();
		updateAndRenderField();

		MissingCode();
		////
		if ((fieldChangePrevented == 0) /*&& (fieldChangePrevented2 == 0)*/)
		{
			//if ((asyncLoadingVar1 == 0xff) && (DAT_Field__800adb90 == 0))
			{
				setCurrentDirectory(4, 0); //TODO: shouldn't be necessary!
				if (loadNewField((fieldMapNumber & 0xfff) * 2, 0) == 0)
				{
					if (isCDBusy() == 0)
					{
						//if (screenEffects[0].m3C_duration == 0)
						{
							//fieldTransitionCompleted = 0;
							//saveStateToKernel();
							waitReadCompletion(0);
							transitionFields();
							//resetInputs();
							//fieldTransitionCompleted = 1;
						}
					}
				}
			}
		}
		////
		MissingCode();

		updateMusicState();
	}
}
