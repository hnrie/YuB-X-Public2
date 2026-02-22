#include <Exploit/Utils.hpp>
#include <Exploit/Globals.hpp>
#include <Communication/Communication.hpp>
#include <Exploit/TaskScheduler/TaskScheduler.hpp>

void main_thread()
{
    communication::initialize();

    while (true)
    {
        uintptr_t data_model = task_scheduler::get_data_model();
        if (!data_model)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        if (shared_variables::last_data_model != data_model)
        {
            if (!utils::is_in_game(data_model))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }
            shared_variables::last_data_model = data_model;
            {
                std::lock_guard<std::mutex> lock(shared_variables::execution_mutex);
                shared_variables::execution_requests.clear();
            }

            task_scheduler::setup_exploit();
            task_scheduler::request_execution("print(\"YuB-X-Public successfully loaded\")");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(h_module);
        std::thread(main_thread).detach();
    }

    return TRUE;
}
