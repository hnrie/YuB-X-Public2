#pragma once

#include <cstdint>
#include <Windows.h>

struct lua_State;
struct WeakThreadRef;
struct DebuggerResult;

#define rebase(address) (address + reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr)))

namespace offsets
{
    const uintptr_t print = rebase(0x1B09F20);
    const uintptr_t task_defer = rebase(0x1AF0340);
    const uintptr_t raw_scheduler = rebase(0x7EF3C48);
    const uintptr_t opcode_lookup_table = rebase(0x5E36590);
    const uintptr_t script_context_resume = rebase(0x1A3E620);
    const uintptr_t get_lua_state_for_instance = rebase(0x1955D60);

    namespace luau
    {
        const uintptr_t luau_execute = rebase(0x3A40720);
        const uintptr_t lua_o_nil_object = rebase(0x594BCE8);
        const uintptr_t lua_h_dummy_node = rebase(0x594B3B8);
    }

    namespace data_model
    {
        const uintptr_t children = 0x70;
        const uintptr_t game_loaded = 0x5F8;
        const uintptr_t script_context = 0x3F0;
        const uintptr_t fake_data_model_to_data_model = 0x1C0;
        const uintptr_t fake_data_model_pointer = rebase(0x7E35858);
    }

    namespace extra_space
    {
        const uintptr_t require_bypass = 0x948;
        const uintptr_t script_context_to_resume = 0x850;
    }
}

namespace roblox
{
    inline auto print = (uintptr_t(*)(int, const char*, ...))offsets::print;
    inline auto task_defer = (uint64_t(__fastcall*)(lua_State*))offsets::task_defer;
    inline auto luau_execute = (void(__fastcall*)(lua_State*))offsets::luau::luau_execute;
    inline auto get_lua_state_for_instance = (lua_State*(__fastcall*)(uint64_t, uint64_t*, uint64_t*))offsets::get_lua_state_for_instance;
    inline auto script_context_resume = (uint64_t(__fastcall*)(uint64_t, DebuggerResult*, WeakThreadRef**, uint32_t, uint8_t, uint64_t))offsets::script_context_resume;
}
