#include "noahLib.h"
#include "graphics.h"
#include "bgfx/bgfx.h"
#include <bx/math.h>
#include "imguiBGFX.h"
#include "kernel/gte.h"

bgfx::TextureHandle m_vramTextureHandle = BGFX_INVALID_HANDLE;

const bgfx::TextureHandle& getPSXVramTextureHandle()
{
	return m_vramTextureHandle;
}

void updatePSXVram()
{
	int textureHeight = 512;
	int textureWidth = 2048;

	extern std::array<u8, 2048 * 512> gVram;
	//if (m_isVramDirty)
	{
		if (!bgfx::isValid(m_vramTextureHandle))
		{
			m_vramTextureHandle = bgfx::createTexture2D(textureWidth, textureHeight, false, 1, bgfx::TextureFormat::R8U, 0, nullptr);
		}
		bgfx::updateTexture2D(m_vramTextureHandle, 0, 0, 0, 0, textureWidth, textureHeight, bgfx::copy(&gVram[0], textureWidth * textureHeight));
	}
}

bgfx::ProgramHandle loadBgfxProgram(const std::string& VSFile, const std::string& PSFile);

bgfx::ProgramHandle getSPRTShader()
{
	static bgfx::ProgramHandle programHandle = BGFX_INVALID_HANDLE;
	if (!bgfx::isValid(programHandle))
	{
		programHandle = loadBgfxProgram("SPRT_vs", "SPRT_ps");
	}

	return programHandle;
}

bgfx::ProgramHandle getTILEShader()
{
	static bgfx::ProgramHandle programHandle = BGFX_INVALID_HANDLE;
	if (!bgfx::isValid(programHandle))
	{
		programHandle = loadBgfxProgram("TILE_vs", "TILE_ps");
	}

	return programHandle;
}

ImVec2 PSXResolution = { 320,240 };

void DrawSync(int)
{
    MissingCode();
}

int VSync(int)
{
    MissingCode();
	return 0;
}

void InitGeom()
{
    MissingCode();

	setCopControlWord(2, 0xe800, 0x155);
	setCopControlWord(2, 0xf000, 0x100);
	setCopControlWord(2, 0xd000, 1000);
	setCopControlWord(2, 0xd800, 0xffffef9e);
	setCopControlWord(2, 0xe000, 0x1400000);
	setCopControlWord(2, 0xc000, 0);
	setCopControlWord(2, 0xc800, 0);
}

void SetPolyFT4(POLY_FT4* p)
{
	p->m3_size = 9;
	p->code = 0x2C;
}

u16 GetTPage(int tp, int abr, int x, int y)
{
	return (u16)((ushort)((tp & 3U) << 7) | (ushort)((abr & 3U) << 5) | (ushort)((int)(y & 0x100U) >> 4) | (ushort)((int)(x & 0x3ffU) >> 6) | (ushort)((y & 0x200U) << 2));
}

void SetTile(TILE* p)
{
	p->m3_size = 3;
	p->code = 0x60;
}

void SetSemiTrans(TILE* p, int abe)
{
	if (abe == 0)
	{
		p->code &= ~2;
	}
	else
	{
		p->code |= 2;
	}
}

void SetSprt(SPRT* p)
{
	p->m3_size = 4;
	p->code = 0x64;
}

void SetSemiTrans(SPRT* p, int abe)
{
	if (abe == 0)
	{
		p->code &= ~2;
	}
	else
	{
		p->code |= 2;
	}
}

void SetSemiTrans(POLY_FT4* p, int abe)
{
	if (abe == 0)
	{
		p->code &= ~2;
	}
	else
	{
		p->code |= 2;
	}
}

void SetSemiTrans(POLY_F4* p, int abe)
{
    if (abe == 0)
    {
        p->code &= ~2;
    }
    else
    {
        p->code |= 2;
    }
}

void SetSemiTrans(POLY_G4* p, int abe)
{
    if (abe == 0)
    {
        p->code &= ~2;
    }
    else
    {
        p->code |= 2;
    }
}

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h)
{
	(env->disp).x = (short)x;
	(env->disp).y = (short)y;
	(env->disp).w = (short)w;
	(env->screen).x = 0;
	(env->screen).y = 0;
	(env->screen).w = 0;
	(env->screen).h = 0;
	env->isrgb24 = '\0';
	env->isinter = '\0';
	env->pad1 = '\0';
	env->pad0 = '\0';
	(env->disp).h = (short)h;
	return env;
}

u16 GetClut(int x, int y)
{
	return (u16)((u16)(y << 6) | (u16)(x >> 4) & 0x3f);
}

s32 currentVideoMode = 0;

long GetVideoMode(void)

{
	return currentVideoMode;
}

DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h)
{
	bool bVar1;
	long lVar2;

	lVar2 = GetVideoMode();
	(env->clip).x = (short)x;
	(env->clip).y = (short)y;
	(env->clip).w = (short)w;
	(env->tw).x = 0;
	(env->tw).y = 0;
	(env->tw).w = 0;
	(env->tw).h = 0;
	env->r0 = 0;
	env->g0 = 0;
	env->b0 = 0;
	env->dtd = 1;
	(env->clip).h = (short)h;
	if (lVar2 == 0) {
		bVar1 = h < 0x101;
	}
	else {
		bVar1 = h < 0x121;
	}
	env->dfe = bVar1;
	env->ofs[0] = (short)x;
	env->ofs[1] = (short)y;
	env->tpage = 10;
	env->isbg = '\0';
	return env;
}

DISPENV currentDispEnv;
DRAWENV currentDrawEnv;

DISPENV* PutDispEnv(DISPENV* env)
{
	MissingCode();

	currentDispEnv = *env;
	return env;

}
DRAWENV* PutDrawEnv(DRAWENV* env)
{
	MissingCode();

	currentDrawEnv = *env;
	return env;
}

void SetDispMask(int mask)
{
    MissingCode();
}

sTag gEndTag;

sTag* ClearOTagR(sTag* ot, int n)
{
	sTag* pCurrent = ot;
	for (int i=0; i<n-1; i++)
	{
		pCurrent++;
		pCurrent->m0_pNext = pCurrent - 1;
		pCurrent->m3_size = 0;
	}

	ot->m0_pNext = &gEndTag;
	ot->m3_size = 0;

	return ot;
}

void DrawOTag(sTag* ot)
{
	updatePSXVram();
	while (ot)
	{
		ot->execute();
		ot = ot->m0_pNext;
	}
}

u8 graphType = 0;

u32 get_mode(int dfe, int dtd, int tpage)
{
	uint uVar1;

	if (graphType - 1 < 2) {
		uVar1 = 0xe1000000;
		if (dtd != 0) {
			uVar1 = 0xe1000800;
		}
		tpage = tpage & 0x27ff;
		if (dfe != 0) {
			tpage = tpage | 0x1000;
		}
	}
	else {
		uVar1 = 0xe1000000;
		if (dtd != 0) {
			uVar1 = 0xe1000200;
		}
		tpage = tpage & 0x9ff;
		if (dfe != 0) {
			tpage = tpage | 0x400;
		}
	}
	return uVar1 | tpage;
}

u32 get_tw(RECT* param_1)
{
	uint uVar1;

	if (param_1 == (RECT*)0x0) {
		uVar1 = 0;
	}
	else {
		uVar1 = (uint)(*(byte*)&param_1->y >> 3) << 0xf | (uint)(*(byte*)&param_1->x >> 3) << 10 | 0xe2000000 | ((int)(-(int)param_1->h & 0xffU) >> 3) << 5 | (int)(-(int)param_1->w & 0xffU) >> 3;
	}
	return uVar1;
}

void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw)
{
	p->m3_size = 2;
	p->code[0] = get_mode(dfe, dtd, tpage & 0xffff);
	p->code[1] = get_tw(tw);
}

bgfx::FrameBufferHandle PSXOutput_FB = BGFX_INVALID_HANDLE;
bgfx::TextureHandle PSXOutput_Texture = BGFX_INVALID_HANDLE;
int PSXOutput_bgfxView = 3;

void initPSXFB()
{
	if (!bgfx::isValid(PSXOutput_FB))
	{
		const uint64_t tsFlags = 0
			//| BGFX_SAMPLER_MIN_POINT
			//| BGFX_SAMPLER_MAG_POINT
			//| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

		PSXOutput_Texture = bgfx::createTexture2D(PSXResolution[0], PSXResolution[1], false, 0, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | tsFlags);
		std::array<bgfx::Attachment, 1> attachements;
		attachements[0].init(PSXOutput_Texture);
		PSXOutput_FB = bgfx::createFrameBuffer(1, &attachements[0], true);
	}

	bgfx::setViewRect(PSXOutput_bgfxView, 0, 0, PSXResolution[0], PSXResolution[1]);

	float mtx_view[16];
	float mtx_projection[16];

	bx::mtxIdentity(mtx_view);
	bx::mtxOrtho(mtx_projection, 0, PSXResolution[0], PSXResolution[1], 0, -1, 1, 0, 0);

	bgfx::setViewTransform(PSXOutput_bgfxView, mtx_view, mtx_projection);
	//bgfx::setTransform(matrix);

	bgfx::setViewFrameBuffer(PSXOutput_bgfxView, PSXOutput_FB);
	bgfx::setViewRect(PSXOutput_bgfxView, 0, 0, PSXResolution[0], PSXResolution[1]);

	bgfx::setViewClear(PSXOutput_bgfxView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 255);

	bgfx::setViewName(PSXOutput_bgfxView, "PSX Output");
	bgfx::setViewMode(PSXOutput_bgfxView, bgfx::ViewMode::Sequential);

	bgfx::touch(PSXOutput_bgfxView);
}

bool isGameInFocus() {
    ImGui::Begin("PSX Output");
    bool gameHasFocus = ImGui::IsWindowFocused();
    ImGui::End();

    return gameHasFocus;
}

void drawPSXFB()
{
	if (ImGui::Begin("PSX Output"))
	{
		ImGui::Image(PSXOutput_Texture, ImVec2(PSXResolution[0]*2, PSXResolution[1]*2));
	}
	ImGui::End();
}

DR_MODE* pCurrentDrMode = nullptr;

void DR_MODE::execute()
{
	pCurrentDrMode = this;
}

void DR_ENV::execute()
{
}

u32 getBlending(u32 code, u32 tpage, u64& State)
{
/*
* 0.5 x B + 0.5 x F
* 1.0 x B + 1.0 x F
* 1.0 x B - 1.0 x F
* 1.0 x B +0.25 x F
*/

	if (code & 2)
	{
		int abr = (tpage >> 5) & 3;

		switch (abr)
		{
		case 0: // 0.5 x B + 0.5 x F
			State |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_FACTOR, BGFX_STATE_BLEND_FACTOR) | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD);
			return 0x7F7F7F7F;
			break;
		case 1: // 1.0 x B + 1.0 x F
			State |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE) | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD);
			return 0xFFFFFFFF;
			break;
		case 2: // 1.0 x B - 1.0 x F
			State |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE) | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_REVSUB);
			return 0xFFFFFFFF;
			break;
		case 3: // 1.0 x B +0.25 x F
			State |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_FACTOR, BGFX_STATE_BLEND_INV_FACTOR) | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD);
			return 0x3F3F3F3F;
		}
	}
	return 0;
}

void SPRT::execute()
{
	float matrix[16];
	bx::mtxIdentity(matrix);

	bgfx::setTransform(matrix);
    if(1)
	{
		bgfx::VertexLayout layout;
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
			.add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
			.add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
			.end();

		bgfx::TransientVertexBuffer vertexBuffer;
		bgfx::TransientIndexBuffer indexBuffer;
		bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 4);

		struct sVertice
		{
			float v[3];
			float color[3];
			float texcoord[2];
			u16 CLUT[2];
			u8 Texpage[4];
			u8 TextureWindow[4];
		};

		sVertice* pVertices = (sVertice*)vertexBuffer.data;
		u16* pIndices = (u16*)indexBuffer.data;

		for (int i=0; i<4; i++)
		{
			pVertices[i].CLUT[0] = clut & 0x3F;
			pVertices[i].CLUT[1] = (clut >> 6) & 0x1FF;

			assert(((pCurrentDrMode->code[0] >> 24) & 0xFF) == 0xE1);
			pVertices[i].Texpage[0] = pCurrentDrMode->code[0] & 0xFF;
			pVertices[i].Texpage[1] = (pCurrentDrMode->code[0] >> 8) & 0xFF;
			pVertices[i].Texpage[2] = (pCurrentDrMode->code[0] >> 16) & 0xFF;
			pVertices[i].Texpage[3] = 0xE1;

			pVertices[i].TextureWindow[0] = pCurrentDrMode->code[1] & 0xFF;
			pVertices[i].TextureWindow[1] = (pCurrentDrMode->code[1] >> 8) & 0xFF;
			pVertices[i].TextureWindow[2] = (pCurrentDrMode->code[1] >> 16) & 0xFF;
			pVertices[i].TextureWindow[3] = 0;
		}

		float localU0 = u0;
		float localV0 = v0;
		float localU1 = u0 + w;
		float localV1 = v0 + h;

		pVertices[0].v[0] = x0;
		pVertices[0].v[1] = y0;
		pVertices[0].v[2] = 0;
		pVertices[0].texcoord[0] = localU0;
		pVertices[0].texcoord[1] = localV0;

		pVertices[1].v[0] = x0 + w;
		pVertices[1].v[1] = y0;
		pVertices[1].v[2] = 0;
		pVertices[1].texcoord[0] = localU1;
		pVertices[1].texcoord[1] = localV0;

		pVertices[2].v[0] = x0 + w;
		pVertices[2].v[1] = y0 + h;
		pVertices[2].v[2] = 0;
		pVertices[2].texcoord[0] = localU1;
		pVertices[2].texcoord[1] = localV1;

		pVertices[3].v[0] = x0;
		pVertices[3].v[1] = y0 + h;
		pVertices[3].v[2] = 0;
		pVertices[3].texcoord[0] = localU0;
		pVertices[3].texcoord[1] = localV1;

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 3;
		pIndices[3] = 2;

		u64 State = BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_ALWAYS
			| BGFX_STATE_MSAA
			| BGFX_STATE_PT_TRISTRIP;

		u32 blendRGBA = getBlending(code, pCurrentDrMode->code[0], State);

		bgfx::setState(State, blendRGBA);

		bgfx::setVertexBuffer(0, &vertexBuffer);
		bgfx::setIndexBuffer(&indexBuffer);

		static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
		if (!bgfx::isValid(s_PSXVramUniformHandle))
		{
			s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
		}
		bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
		bgfx::submit(PSXOutput_bgfxView, getSPRTShader());
	}

}

void TILE::execute()
{
	float matrix[16];
	bx::mtxIdentity(matrix);

	bgfx::setTransform(matrix);
    if(1)
	{
		bgfx::VertexLayout layout;
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
			.add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
			.add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
			.end();

		bgfx::TransientVertexBuffer vertexBuffer;
		bgfx::TransientIndexBuffer indexBuffer;
		bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 4);

		struct sVertice
		{
			float v[3];
			float color[4];
			float texcoord[2];
			u16 CLUT[2];
			u8 Texpage[4];
			u8 TextureWindow[4];
		};

		sVertice* pVertices = (sVertice*)vertexBuffer.data;
		u16* pIndices = (u16*)indexBuffer.data;

		for (int i = 0; i < 4; i++)
		{
			pVertices[i].color[0] = r0 / 255.f;
			pVertices[i].color[1] = g0 / 255.f;
			pVertices[i].color[2] = b0 / 255.f;
			pVertices[i].color[3] = 1.f - ((r0 & 0x80) / (float)0x80);
		}

		pVertices[0].v[0] = x0;
		pVertices[0].v[1] = y0;
		pVertices[0].v[2] = 0;

		pVertices[1].v[0] = x0 + w;
		pVertices[1].v[1] = y0;
		pVertices[1].v[2] = 0;

		pVertices[2].v[0] = x0 + w;
		pVertices[2].v[1] = y0 + h;
		pVertices[2].v[2] = 0;

		pVertices[3].v[0] = x0;
		pVertices[3].v[1] = y0 + h;
		pVertices[3].v[2] = 0;

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 3;
		pIndices[3] = 2;

		u64 State = BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_ALWAYS
			| BGFX_STATE_MSAA
			| BGFX_STATE_PT_TRISTRIP;

		u32 blendRGBA = getBlending(code, pCurrentDrMode->code[0], State);

		bgfx::setState(State, blendRGBA);

		bgfx::setVertexBuffer(0, &vertexBuffer);
		bgfx::setIndexBuffer(&indexBuffer);

		static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
		if (!bgfx::isValid(s_PSXVramUniformHandle))
		{
			s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
		}
		bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
		bgfx::submit(PSXOutput_bgfxView, getTILEShader());
	}
}

void POLY_F3::execute()
{
    float matrix[16];
    bx::mtxIdentity(matrix);

    bgfx::setTransform(matrix);
    if (1)
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
            .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
            .add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
            .end();

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::allocTransientBuffers(&vertexBuffer, layout, 3, &indexBuffer, 3);

        struct sVertice
        {
            float v[3];
            float color[3];
            float texcoord[2];
            u16 CLUT[2];
            u8 Texpage[4];
            u8 TextureWindow[4];
        };

        sVertice* pVertices = (sVertice*)vertexBuffer.data;
        u16* pIndices = (u16*)indexBuffer.data;

        for (int i = 0; i < 3; i++)
        {
            pVertices[i].color[0] = r0 / 255.f;
            pVertices[i].color[1] = g0 / 255.f;
            pVertices[i].color[2] = b0 / 255.f;
            pVertices[i].color[3] = 1.f - ((r0 & 0x80) / (float)0x80);
        }

        pVertices[0].v[0] = x0y0.vx;
        pVertices[0].v[1] = x0y0.vy;
        pVertices[0].v[2] = 0;

        pVertices[1].v[0] = x1y1.vx;
        pVertices[1].v[1] = x1y1.vy;
        pVertices[1].v[2] = 0;

        pVertices[2].v[0] = x2y2.vx;
        pVertices[2].v[1] = x2y2.vy;
        pVertices[2].v[2] = 0;

        pIndices[0] = 0;
        pIndices[1] = 1;
        pIndices[2] = 2;

        u64 State = BGFX_STATE_WRITE_RGB
            | BGFX_STATE_DEPTH_TEST_ALWAYS
            | BGFX_STATE_MSAA
            | BGFX_STATE_PT_TRISTRIP;

        u32 blendRGBA = getBlending(code, pCurrentDrMode->code[0], State);

        bgfx::setState(State, blendRGBA);

        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);

        static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
        if (!bgfx::isValid(s_PSXVramUniformHandle))
        {
            s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
        }
        bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
        bgfx::submit(PSXOutput_bgfxView, getTILEShader());
    }
}

void POLY_F4::execute()
{
    float matrix[16];
    bx::mtxIdentity(matrix);

    bgfx::setTransform(matrix);
    if (1)
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
            .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
            .add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
            .end();

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 4);

        struct sVertice
        {
            float v[3];
            float color[3];
            float texcoord[2];
            u16 CLUT[2];
            u8 Texpage[4];
            u8 TextureWindow[4];
        };

        sVertice* pVertices = (sVertice*)vertexBuffer.data;
        u16* pIndices = (u16*)indexBuffer.data;

        for (int i = 0; i < 4; i++)
        {
            pVertices[i].color[0] = r0 / 255.f;
            pVertices[i].color[1] = g0 / 255.f;
            pVertices[i].color[2] = b0 / 255.f;
            pVertices[i].color[3] = 1.f - ((r0 & 0x80) / (float)0x80);
        }

        pVertices[0].v[0] = x0y0.vx;
        pVertices[0].v[1] = x0y0.vy;
        pVertices[0].v[2] = 0;

        pVertices[1].v[0] = x1y1.vx;
        pVertices[1].v[1] = x1y1.vy;
        pVertices[1].v[2] = 0;

        pVertices[2].v[0] = x2y2.vx;
        pVertices[2].v[1] = x2y2.vy;
        pVertices[2].v[2] = 0;

        pVertices[3].v[0] = x3y3.vx;
        pVertices[3].v[1] = x3y3.vy;
        pVertices[3].v[2] = 0;

        pIndices[0] = 0;
        pIndices[1] = 1;
        pIndices[2] = 2;
        pIndices[3] = 3;

        u64 State = BGFX_STATE_WRITE_RGB
            | BGFX_STATE_DEPTH_TEST_ALWAYS
            | BGFX_STATE_MSAA
            | BGFX_STATE_PT_TRISTRIP;

        u32 blendRGBA = getBlending(code, pCurrentDrMode->code[0], State);

        bgfx::setState(State, blendRGBA);

        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);

        static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
        if (!bgfx::isValid(s_PSXVramUniformHandle))
        {
            s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
        }
        bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
        bgfx::submit(PSXOutput_bgfxView, getSPRTShader());
    }
}

void POLY_FT4::execute()
{
	float matrix[16];
	bx::mtxIdentity(matrix);

	bgfx::setTransform(matrix);
    if(1)
	{
		bgfx::VertexLayout layout;
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
			.add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
			.add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
			.end();

		bgfx::TransientVertexBuffer vertexBuffer;
		bgfx::TransientIndexBuffer indexBuffer;
		bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 4);

		struct sVertice
		{
			float v[3];
			float color[3];
			float texcoord[2];
			u16 CLUT[2];
			u8 Texpage[4];
			u8 TextureWindow[4];
		};

		sVertice* pVertices = (sVertice*)vertexBuffer.data;
		u16* pIndices = (u16*)indexBuffer.data;

		for (int i = 0; i < 4; i++)
		{
			pVertices[i].CLUT[0] = clut & 0x3F;
			pVertices[i].CLUT[1] = (clut >> 6) & 0x1FF;

			u32 code = tpage;
			pVertices[i].Texpage[0] = code & 0xFF;
			pVertices[i].Texpage[1] = (code >> 8) & 0xFF;
			pVertices[i].Texpage[2] = (code >> 16) & 0xFF;
			pVertices[i].Texpage[3] = 0xE1;

			RECT fullTextureWindow = { 0,0,255,255 };
			u32 textureWindow = get_tw(&fullTextureWindow);
			pVertices[i].TextureWindow[0] = textureWindow & 0xFF;
			pVertices[i].TextureWindow[1] = (textureWindow >> 8) & 0xFF;
			pVertices[i].TextureWindow[2] = (textureWindow >> 16) & 0xFF;
			pVertices[i].TextureWindow[3] = 0xE2;
		}

		pVertices[0].v[0] = x0y0.vx;
		pVertices[0].v[1] = x0y0.vy;
		pVertices[0].v[2] = 0;
		pVertices[0].texcoord[0] = u0;
		pVertices[0].texcoord[1] = v0;

		pVertices[1].v[0] = x1y1.vx;
		pVertices[1].v[1] = x1y1.vy;
		pVertices[1].v[2] = 0;
		pVertices[1].texcoord[0] = u1;
		pVertices[1].texcoord[1] = v1;

		pVertices[2].v[0] = x2y2.vx;
		pVertices[2].v[1] = x2y2.vy;
		pVertices[2].v[2] = 0;
		pVertices[2].texcoord[0] = u2;
		pVertices[2].texcoord[1] = v2;

		pVertices[3].v[0] = x3y3.vx;
		pVertices[3].v[1] = x3y3.vy;
		pVertices[3].v[2] = 0;
		pVertices[3].texcoord[0] = u3;
		pVertices[3].texcoord[1] = v3;

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;
		pIndices[3] = 3;

		u64 State = BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_ALWAYS
			| BGFX_STATE_MSAA
			| BGFX_STATE_PT_TRISTRIP;

		u32 blendRGBA = getBlending(code, tpage, State);

		bgfx::setState(State, blendRGBA);

		bgfx::setVertexBuffer(0, &vertexBuffer);
		bgfx::setIndexBuffer(&indexBuffer);

		static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
		if (!bgfx::isValid(s_PSXVramUniformHandle))
		{
			s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
		}
		bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
		bgfx::submit(PSXOutput_bgfxView, getSPRTShader());
	}

}

void POLY_FT3::execute()
{
	float matrix[16];
	bx::mtxIdentity(matrix);

	bgfx::setTransform(matrix);
	{
		bgfx::VertexLayout layout;
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
			.add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
			.add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
			.end();

		bgfx::TransientVertexBuffer vertexBuffer;
		bgfx::TransientIndexBuffer indexBuffer;
		bgfx::allocTransientBuffers(&vertexBuffer, layout, 3, &indexBuffer, 3);

		struct sVertice
		{
			float v[3];
			float color[3];
			float texcoord[2];
			u16 CLUT[2];
			u8 Texpage[4];
			u8 TextureWindow[4];
		};

		sVertice* pVertices = (sVertice*)vertexBuffer.data;
		u16* pIndices = (u16*)indexBuffer.data;

		for (int i = 0; i < 4; i++)
		{
			pVertices[i].CLUT[0] = clut & 0x3F;
			pVertices[i].CLUT[1] = (clut >> 6) & 0x1FF;

			u32 code = tpage;
			pVertices[i].Texpage[0] = code & 0xFF;
			pVertices[i].Texpage[1] = (code >> 8) & 0xFF;
			pVertices[i].Texpage[2] = (code >> 16) & 0xFF;
			pVertices[i].Texpage[3] = 0xE1;

			RECT fullTextureWindow = { 0,0,255,255 };
			u32 textureWindow = get_tw(&fullTextureWindow);
			pVertices[i].TextureWindow[0] = textureWindow & 0xFF;
			pVertices[i].TextureWindow[1] = (textureWindow >> 8) & 0xFF;
			pVertices[i].TextureWindow[2] = (textureWindow >> 16) & 0xFF;
			pVertices[i].TextureWindow[3] = 0xE2;
		}

		pVertices[0].v[0] = x0y0.vx;
		pVertices[0].v[1] = x0y0.vy;
		pVertices[0].v[2] = 0;
		pVertices[0].texcoord[0] = u0;
		pVertices[0].texcoord[1] = v0;

		pVertices[1].v[0] = x1y1.vx;
		pVertices[1].v[1] = x1y1.vy;
		pVertices[1].v[2] = 0;
		pVertices[1].texcoord[0] = u1;
		pVertices[1].texcoord[1] = v1;

		pVertices[2].v[0] = x2y2.vx;
		pVertices[2].v[1] = x2y2.vy;
		pVertices[2].v[2] = 0;
		pVertices[2].texcoord[0] = u2;
		pVertices[2].texcoord[1] = v2;

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;

		u64 State = BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_ALWAYS
			| BGFX_STATE_MSAA
			| BGFX_STATE_PT_TRISTRIP;

		u32 blendRGBA = getBlending(code, tpage, State);

		bgfx::setState(State, blendRGBA);

		bgfx::setVertexBuffer(0, &vertexBuffer);
		bgfx::setIndexBuffer(&indexBuffer);

		static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
		if (!bgfx::isValid(s_PSXVramUniformHandle))
		{
			s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
		}
		bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
		bgfx::submit(PSXOutput_bgfxView, getSPRTShader());
	}
}

void POLY_GT3::execute()
{
    float matrix[16];
    bx::mtxIdentity(matrix);

    bgfx::setTransform(matrix);
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Int16) // CLUT
            .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Uint8) // Texpage
            .add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Uint8) // TextureWindow
            .end();

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::allocTransientBuffers(&vertexBuffer, layout, 3, &indexBuffer, 3);

        struct sVertice
        {
            float v[3];
            float color[3];
            float texcoord[2];
            u16 CLUT[2];
            u8 Texpage[4];
            u8 TextureWindow[4];
        };

        sVertice* pVertices = (sVertice*)vertexBuffer.data;
        u16* pIndices = (u16*)indexBuffer.data;

        for (int i = 0; i < 4; i++)
        {
            pVertices[i].CLUT[0] = clut & 0x3F;
            pVertices[i].CLUT[1] = (clut >> 6) & 0x1FF;

            u32 code = tpage;
            pVertices[i].Texpage[0] = code & 0xFF;
            pVertices[i].Texpage[1] = (code >> 8) & 0xFF;
            pVertices[i].Texpage[2] = (code >> 16) & 0xFF;
            pVertices[i].Texpage[3] = 0xE1;

            RECT fullTextureWindow = { 0,0,255,255 };
            u32 textureWindow = get_tw(&fullTextureWindow);
            pVertices[i].TextureWindow[0] = textureWindow & 0xFF;
            pVertices[i].TextureWindow[1] = (textureWindow >> 8) & 0xFF;
            pVertices[i].TextureWindow[2] = (textureWindow >> 16) & 0xFF;
            pVertices[i].TextureWindow[3] = 0xE2;
        }

        {
            pVertices[0].color[0] = r0 / 255.f;
            pVertices[0].color[1] = g0 / 255.f;
            pVertices[0].color[2] = b0 / 255.f;
            pVertices[0].color[3] = 1.f - ((r0 & 0x80) / (float)0x80);
        }

        {
            pVertices[1].color[0] = r1 / 255.f;
            pVertices[1].color[1] = g1 / 255.f;
            pVertices[1].color[2] = b1 / 255.f;
            pVertices[1].color[3] = 1.f - ((r1 & 0x80) / (float)0x80);
        }

        {
            pVertices[2].color[0] = r2 / 255.f;
            pVertices[2].color[1] = g2 / 255.f;
            pVertices[2].color[2] = b2 / 255.f;
            pVertices[2].color[3] = 1.f - ((r2 & 0x80) / (float)0x80);
        }

        pVertices[0].v[0] = x0y0.vx;
        pVertices[0].v[1] = x0y0.vy;
        pVertices[0].v[2] = 0;
        pVertices[0].texcoord[0] = u0;
        pVertices[0].texcoord[1] = v0;

        pVertices[1].v[0] = x1y1.vx;
        pVertices[1].v[1] = x1y1.vy;
        pVertices[1].v[2] = 0;
        pVertices[1].texcoord[0] = u1;
        pVertices[1].texcoord[1] = v1;

        pVertices[2].v[0] = x2y2.vx;
        pVertices[2].v[1] = x2y2.vy;
        pVertices[2].v[2] = 0;
        pVertices[2].texcoord[0] = u2;
        pVertices[2].texcoord[1] = v2;

        pIndices[0] = 0;
        pIndices[1] = 1;
        pIndices[2] = 2;

        u64 State = BGFX_STATE_WRITE_RGB
            | BGFX_STATE_DEPTH_TEST_ALWAYS
            | BGFX_STATE_MSAA
            | BGFX_STATE_PT_TRISTRIP;

        u32 blendRGBA = getBlending(code, tpage, State);

        bgfx::setState(State, blendRGBA);

        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);

        static bgfx::UniformHandle s_PSXVramUniformHandle = BGFX_INVALID_HANDLE;
        if (!bgfx::isValid(s_PSXVramUniformHandle))
        {
            s_PSXVramUniformHandle = bgfx::createUniform("s_PSXVram", bgfx::UniformType::Sampler);
        }
        bgfx::setTexture(0, s_PSXVramUniformHandle, m_vramTextureHandle);
        bgfx::submit(PSXOutput_bgfxView, getSPRTShader());
    }
}

void DR_MOVE::execute()
{
    MissingCode();
}

void SetDrawMove(DR_MOVE* p, RECT* rect, int x, int y)
{
    MissingCode();
}

