#pragma once

#include <cstdint>
#include <assert.h>
#include <string>
#include <vector>
#include <array>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef u8 byte;
typedef u16 ushort;
typedef u32 uint;

struct sPS1Pointer
{
public:
    void setPointer(u8* pointer)
    {
        m_pointer = pointer;
    }
    u8* getPointer() const
    {
        return m_pointer;
    }

    bool operator == (const sPS1Pointer& other) {
        return (other.m_pointer == m_pointer);
    }

    sPS1Pointer operator + (unsigned int i) const
    {
        sPS1Pointer newPtr = *this;
        newPtr.m_pointer += i;
        return newPtr;
    }
    void makeNull()
    {
        m_pointer = nullptr;
    }
private:
    u8* m_pointer = nullptr;
};

u8 READ_LE_U8(const sPS1Pointer&);
s8 READ_LE_S8(const sPS1Pointer&);
u16 READ_LE_U16(const sPS1Pointer&);
s16 READ_LE_S16(const sPS1Pointer&);
u32 READ_LE_U32(const sPS1Pointer&);
s32 READ_LE_S32(const sPS1Pointer&);

u16 READ_LE_U16(const void* ptr);
s16 READ_LE_S16(const void* ptr);
u32 READ_LE_U32(const void* ptr);
s32 READ_LE_S32(const void* ptr);

u8 READ_LE_U8(std::vector<u8>::const_iterator& inputStream);
s8 READ_LE_S8(std::vector<u8>::const_iterator& inputStream);
u16 READ_LE_U16(std::vector<u8>::const_iterator& inputStream);
s16 READ_LE_S16(std::vector<u8>::const_iterator& inputStream);
u32 READ_LE_U32(std::vector<u8>::const_iterator& inputStream);
s32 READ_LE_S32(std::vector<u8>::const_iterator& inputStream);

void noahFrame_start();
bool noahFrame_end();

struct VECTOR
{
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
};

struct SVECTOR
{
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
};

#define MissingCode()
