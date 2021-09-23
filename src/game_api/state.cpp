#include "state.hpp"
#include "game_manager.hpp"
#include "level_api.hpp"
#include "logger.h"
#include "render_api.hpp"
#include "spawn_api.hpp"
#include "virtual_table.hpp"

uint16_t StateMemory::get_correct_ushabti() // returns animation_frame of ushabti
{
    return (correct_ushabti + (correct_ushabti / 10) * 2);
}
void StateMemory::set_correct_ushabti(uint16_t animation_frame)
{
    correct_ushabti = static_cast<uint8_t>(animation_frame - (animation_frame / 12) * 2);
}

size_t get_dark()
{
    ONCE(size_t)
    {
        auto memory = Memory::get();
        auto addr_dark = memory.after_bundle;
        addr_dark = find_inst(memory.exe(), "\x44\xC5\x80\xA0\x12\x0A\x00\x00\xFD"s, memory.after_bundle);
        return res = memory.at_exe(addr_dark) + 9;
    }
}

size_t get_zoom()
{
    ONCE(size_t)
    {
        auto memory = Memory::get();
        auto addr_zoom = memory.after_bundle;
        for (int _ = 0; _ < 3; _++)
        {
            addr_zoom = find_inst(memory.exe(), "\x48\x8B\x48\x10\xC7\x81"s, addr_zoom + 1);
        }
        return res = memory.at_exe(addr_zoom) + 10;
    }
}

size_t get_zoom_shop()
{
    ONCE(size_t)
    {
        auto memory = Memory::get();
        auto addr_zoom = memory.after_bundle;
        for (int _ = 0; _ < 2; _++)
        {
            addr_zoom = find_inst(memory.exe(), "\x48\x8B\x48\x10\xC7\x81"s, addr_zoom + 1);
        }
        return res = memory.at_exe(addr_zoom) + 10;
    }
}

size_t get_insta()
{
    // TODO
    return 0ul;

    ONCE(size_t)
    {
        auto memory = Memory::get();
        auto off = memory.after_bundle;
        off = find_inst(memory.exe(), "\x40\x53\x56\x41\x54\x41\x55\x48\x83\xEC\x68"s,
                        off + 1); // Spel2.exe+21E37920
        return res = function_start(memory.at_exe(off));
    }
}

size_t get_camera()
{
    ONCE(size_t)
    {
        auto memory = Memory::get();
        auto off = find_inst(memory.exe(), "\xC7\x87\xCC\x00\x00\x00\x00\x00"s, memory.after_bundle);
        off = find_inst(memory.exe(), "\xF3\x0F\x11\x05"s, off) + 1;
        return res = memory.at_exe(decode_pc(memory.exe(), off));
    }
}

inline bool& get_is_init()
{
    static bool is_init{false};
    return is_init;
}

void do_write_load_opt()
{
    write_mem_prot(get_address("write_load_opt"), "\x90\x90"s, true);
}
bool& get_write_load_opt()
{
    static bool allowed{true};
    return allowed;
}
void State::set_write_load_opt(bool write_load_opt)
{
    if (get_is_init())
    {
        if (write_load_opt && !get_write_load_opt())
        {
            do_write_load_opt();
        }
        else if (!write_load_opt && get_write_load_opt())
        {
            DEBUG("Can not unwrite the load optimization...");
        }
    }
    else
    {
        get_write_load_opt() = write_load_opt;
    }
}

State& State::get()
{
    static State STATE;
    if (!get_is_init())
    {
        if (get_write_load_opt())
        {
            do_write_load_opt();
        }
        auto addr_location = get_address("state_location");
        auto addr_insta = get_insta();
        auto addr_zoom = 0ul;      // get_zoom();
        auto addr_zoom_shop = 0ul; //get_zoom_shop();
        auto addr_dark = 0ul;      //get_dark();
        STATE = State{addr_location, addr_insta, addr_zoom, addr_zoom_shop, addr_dark};
        DEBUG("TODO: patterns for level_gen and spawn_hooks");
        //STATE.ptr()->level_gen->init();
        //init_spawn_hooks();
        init_render_api_hooks();
        get_is_init() = true;
    }
    return STATE;
}

StateMemory* State::ptr() const
{
    OnHeapPointer<StateMemory> p(read_u64(location));
    return p.decode();
}

StateMemory* State::ptr_local() const
{
    OnHeapPointer<StateMemory> p(read_u64(location));
    return p.decode_local();
}

std::pair<float, float> State::click_position(float x, float y)
{
    float cz = get_zoom_level();
    float cx = ptr()->camera->calculated_focus_x;
    float cy = ptr()->camera->calculated_focus_y;
    float rx = cx + ZF * cz * x;
    float ry = cy + (ZF / 16.0f * 9.0f) * cz * y;
    return {rx, ry};
}

std::pair<float, float> State::screen_position(float x, float y)
{
    float cz = get_zoom_level();
    float cx = ptr()->camera->calculated_focus_x;
    float cy = ptr()->camera->calculated_focus_y;
    float rx = (x - cx) / cz / ZF;
    float ry = (y - cy) / cz / (ZF / 16.0f * 9.0f);
    return {rx, ry};
}

float State::get_zoom_level()
{
    float default_zoom = 13.5;
    static size_t offset = 0;
    if (offset == 0)
    {
        size_t obj1 = get_address("zoom_level");

        size_t obj2 = read_u64(obj1);
        if (obj2 == 0)
        {
            return default_zoom;
        }

        size_t obj3 = read_u64(obj2 + 0x10);
        if (obj3 == 0)
        {
            return default_zoom;
        }
        offset = obj3 + get_address("zoom_level_offset");
    }
    return read_f32(offset);
}

void State::godmode(bool g)
{
    DEBUG("TODO godmode: fix get_insta()");
    auto memory = Memory::get();
    auto addr_damage = memory.at_exe(get_virtual_function_address(VTABLE_OFFSET::CHAR_ANA_SPELUNKY, 48));

    // log::debug!("God {:?}" mode; g);
    if (g)
    {
        write_mem_prot(addr_damage, ("\xC3"s), true);
        //write_mem_prot(addr_insta, ("\xC3"s), true);
    }
    else
    {
        write_mem_prot(addr_damage, ("\x41"s), true);
        //write_mem_prot(addr_insta, ("\x40"s), true);
    }
}

std::pair<float, float> State::get_camera_position()
{
    float cx = ptr()->camera->calculated_focus_x;
    float cy = ptr()->camera->calculated_focus_y;
    return {cx, cy};
}

void State::set_camera_position(float cx, float cy)
{
    write_mem_prot(get_camera(), to_le_bytes(cx), true);
    write_mem_prot(get_camera() + 4, to_le_bytes(cy), true);
}

void State::warp(uint8_t w, uint8_t l, uint8_t t)
{
    //if (ptr()->screen < 11 || ptr()->screen > 20)
    //    return;
    if (ptr()->items->player_count < 1)
    {
        ptr()->items->player_select_slots[0].activated = true;
        ptr()->items->player_select_slots[0].character = savedata()->players[0] + to_id("ENT_TYPE_CHAR_ANA_SPELUNKY");
        ptr()->items->player_select_slots[0].texture_id = savedata()->players[0] + 285; //TODO: magic numbers
        ptr()->items->player_count = 1;
    }
    ptr()->world_next = w;
    ptr()->level_next = l;
    ptr()->theme_next = t;
    if (ptr()->world_start < 1 || ptr()->level_start < 1 || ptr()->theme_start < 1 || ptr()->theme == 17)
    {
        ptr()->world_start = w;
        ptr()->level_start = l;
        ptr()->theme_start = t;
        ptr()->quest_flags = 1;
    }
    if (t != 17)
    {
        ptr()->screen_next = 12;
    }
    else
    {
        ptr()->screen_next = 11;
    }
    ptr()->fadeout = 5;
    ptr()->fadein = 5;
    ptr()->win_state = 0;
    ptr()->loading = 1;
}

void State::set_seed(uint32_t seed)
{
    if (ptr()->screen < 11 || ptr()->screen > 20)
        return;
    ptr()->seed = seed;
    ptr()->world_start = 1;
    ptr()->level_start = 1;
    ptr()->theme_start = 1;
    ptr()->world_next = 1;
    ptr()->level_next = 1;
    ptr()->theme_next = 1;
    ptr()->quest_flags = 0x1e | 0x41;
    ptr()->screen_next = 12;
    ptr()->fadeout = 5;
    ptr()->fadein = 5;
    ptr()->loading = 1;
}

SaveData* State::savedata()
{
    auto gm = get_game_manager();
    return gm->tmp->savedata.decode();
}

Entity* State::find(uint32_t uid)
{
    // Ported from MauveAlert's python code in the CAT tracker
    auto mask = ptr()->uid_to_entity_mask;
    uint32_t target_uid_plus_one = uid + 1;
    uint32_t cur_index = target_uid_plus_one & mask;
    while (true)
    {
        auto entry = ptr()->uid_to_entity_data[cur_index];
        if (entry.uid_plus_one == target_uid_plus_one)
        {
            return entry.entity;
        }

        if (entry.uid_plus_one == 0)
        {
            return nullptr;
        }

        if ((target_uid_plus_one & mask) > (entry.uid_plus_one & mask))
        {
            return nullptr;
        }

        cur_index = (cur_index + 1) & mask;
    }
}
