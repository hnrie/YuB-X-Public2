#pragma once

#include <cstdint>
#include <Windows.h>

struct lua_State;
struct WeakThreadRef;
struct DebuggerResult;

#define REBASE(Address) (Address + reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr)))

namespace Offsets
{
    const uintptr_t Print = REBASE(0x1723440);
    const uintptr_t TaskDefer = REBASE(0x1709970);
    const uintptr_t RawScheduler = REBASE(0x80AB1C8);
    const uintptr_t OpcodeLookupTable = REBASE(0x5C69D00);
    const uintptr_t ScriptContextResume = REBASE(0x1635BD0);

    namespace Luau
    {
        const uintptr_t Luau_Execute = REBASE(0x35BC760);
        const uintptr_t LuaO_NilObject = REBASE(0x5788A68);
        const uintptr_t LuaH_DummyNode = REBASE(0x5788458);
    }

    namespace DataModel
    {
        const uintptr_t Children = 0x70;
        const uintptr_t GameLoaded = 0x5F8;
        const uintptr_t ScriptContext = 0x3F0;
        const uintptr_t FakeDataModelToDataModel = 0x1C0;

        const uintptr_t FakeDataModelPointer = REBASE(0x8014E88);
    }

    namespace ExtraSpace
    {
        const uintptr_t RequireBypass = 0x8E9;
        const uintptr_t InstanceToLuaState = 0x230;
        const uintptr_t ScriptContextToResume = 0x840;
    }
}

namespace Roblox
{
    inline auto TaskDefer = (int(__fastcall*)(lua_State*))Offsets::TaskDefer;
    inline auto Print = (uintptr_t(__fastcall*)(int, const char*, ...))Offsets::Print;
    inline auto Luau_Execute = (void(__fastcall*)(lua_State*))Offsets::Luau::Luau_Execute;
    inline auto ScriptContextResume = (int(__fastcall*)(int64_t, DebuggerResult*, WeakThreadRef**, int32_t, bool, const char*))Offsets::ScriptContextResume;
}

// Dont forget to update TaskScheduler::GetGlobalStateForInstance, Encryptions and Structs
