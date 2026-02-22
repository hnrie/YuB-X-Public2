#pragma once

#include <cstdint>
#include <Windows.h>

struct lua_State;
struct WeakThreadRef;
struct DebuggerResult;

#define REBASE(Address) (Address + reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr)))

namespace Offsets
{
    const uintptr_t Print = REBASE(0x1B09F20);
    const uintptr_t TaskDefer = REBASE(0x1AF0340);
    const uintptr_t RawScheduler = REBASE(0x7EF3C48);
    const uintptr_t OpcodeLookupTable = REBASE(0x5E36590);
    const uintptr_t ScriptContextResume = REBASE(0x1A3E620);
    const uintptr_t GetLuaStateForInstance = REBASE(0x1955D60);

    namespace Luau
    {
        const uintptr_t Luau_Execute = REBASE(0x3A40720);
        const uintptr_t LuaO_NilObject = REBASE(0x594BCE8);
        const uintptr_t LuaH_DummyNode = REBASE(0x594B3B8);
    }

    namespace DataModel
    {
        const uintptr_t Children = 0x70;
        const uintptr_t GameLoaded = 0x5F8;
        const uintptr_t ScriptContext = 0x3F0;
        const uintptr_t FakeDataModelToDataModel = 0x1C0;

        const uintptr_t FakeDataModelPointer = REBASE(0x7E35858);
    }
    namespace Scripts {
        const uintptr_t LocalScriptByteCode              = 0x1A8;
        const uintptr_t ModuleScriptByteCode             = 0x150;
                                                         
        const uintptr_t weak_thread_node                 = 0x180;
        const uintptr_t weak_thread_ref                  = 0x8;
        const uintptr_t weak_thread_ref_live             = 0x20;
        const uintptr_t weak_thread_ref_live_thread      = 0x8;
}
    namespace ExtraSpace
    {
        const uintptr_t RequireBypass = 0x948;
        const uintptr_t ScriptContextToResume = 0x850;
    }
}

namespace Roblox
{
    inline auto Print = (uintptr_t(*)(int, const char*, ...))Offsets::Print;
    inline auto TaskDefer = (uint64_t(__fastcall*)(lua_State*))Offsets::TaskDefer;
    inline auto Luau_Execute = (void(__fastcall*)(lua_State*))Offsets::Luau::Luau_Execute;
    inline auto GetLuaStateForInstance = (lua_State*(__fastcall*)(uint64_t, uint64_t*, uint64_t*))Offsets::GetLuaStateForInstance;
    inline auto ScriptContextResume = (uint64_t(__fastcall*)(uint64_t, DebuggerResult*, WeakThreadRef**, uint32_t, uint8_t, uint64_t))Offsets::ScriptContextResume;
}

// Dont forget to update Encryptions and Structs
