#include "vtables_lua.hpp"

#include "entity.hpp"                              // for Entity
#include "hookable_vtable.hpp"                     // for HookableVTable
#include "movable.hpp"                             // for Movable
#include "render_api.hpp"                          // for RenderInfo
#include "script/usertypes/theme_vtable_lua.hpp"   // for NThemeVTables
#include "script/usertypes/vanilla_render_lua.hpp" // for VanillaRenderContext
#include "state.hpp"                               // for State

namespace NVTables
{
void register_usertypes(sol::state& lua)
{
    NThemeVTables::register_usertypes(lua);

    // Define all vtables
    using EntityVTable = HookableVTable<
        Entity,
        CallbackType::Entity,
        VTableEntry<"dtor", 0x0, void()>,
        VTableEntry<"create_rendering_info", 0x1, void()>,
        VTableEntry<"update_state_machine", 0x2, void()>,
        VTableEntry<"kill", 0x3, void(bool, Entity*)>,
        VTableEntry<"on_collision1", 0x4, void(Entity*)>,
        VTableEntry<"destroy", 0x5, void()>,
        VTableEntry<"can_be_pushed", 0xa, bool()>,
        VTableEntry<"is_in_liquid", 0xc, bool()>,
        VTableEntry<"set_invisible", 0xf, void(bool)>,
        VTableEntry<"friction", 0x11, float()>,
        VTableEntry<"get_held_entity", 0x16, Entity*()>,
        VTableEntry<"trigger_action", 0x18, void(Entity*)>,
        VTableEntry<"activate", 0x19, void(Entity*)>,
        VTableEntry<"on_collision2", 0x1a, void(Entity*)>,
        VTableEntry<"walked_on", 0x1d, void(Entity*)>,
        VTableEntry<"walked_off", 0x1e, void(Entity*)>,
        VTableEntry<"ledge_grab", 0x1f, void(Entity*)>,
        VTableEntry<"stood_on", 0x20, void(Entity*)>,
        VTableEntry<"init", 0x24, void()>>;
    static EntityVTable entity_vtable(lua, lua["Entity"], "ENTITY_OVERRIDE");

    using MovableVTable = HookableVTable<
        Entity,
        CallbackType::Entity,
        EntityVTable,
        VTableEntry<"can_jump", 37, bool()>,
        VTableEntry<"get_collision_info", 38, void(CollisionInfo*)>,
        VTableEntry<"sprint_factor", 39, float()>,
        VTableEntry<"calculate_jump_height", 40, void()>,
        VTableEntry<"apply_velocity", 42, void(Vec2*)>,
        VTableEntry<"stomp_damage", 43, int8_t()>,
        VTableEntry<"is_on_fire", 45, bool()>,
        VTableEntry<"damage", 48, bool(Entity*, int8_t, uint16_t, Vec2*, uint8_t, uint16_t, uint8_t, bool)>,
        VTableEntry<"on_hit", 49, void(Entity*)>,
        VTableEntry<"stun", 51, void(uint16_t)>,
        VTableEntry<"freeze", 52, void(uint8_t)>,
        VTableEntry<"light_on_fire", 53, void(uint8_t)>,
        VTableEntry<"set_cursed", 54, void(bool)>,
        VTableEntry<"web_collision", 55, void()>,
        VTableEntry<"set_owner", 56, void(Entity*)>,
        VTableEntry<"get_owner", 57, uint32_t()>,
        VTableEntry<"check_out_of_bounds", 58, void()>,
        VTableEntry<"standing_on", 60, Entity*()>,
        VTableEntry<"stomped_by", 61, void(Entity*)>,
        VTableEntry<"thrown_by", 62, void(Entity*)>,
        VTableEntry<"cloned_to", 63, void(Entity*)>,
        VTableEntry<"get_type", 64, uint32_t()>,
        VTableEntry<"pick_up", 67, void(Entity*)>,
        VTableEntry<"picked_up_by", 68, void(Entity*)>,
        VTableEntry<"drop", 69, void(Entity*)>,
        VTableEntry<"add_money", 70, void(uint32_t)>,
        VTableEntry<"apply_movement", 71, void()>,
        VTableEntry<"damage_entity", 72, void(Entity*)>,
        VTableEntry<"initialize", 75, void()>,
        VTableEntry<"check_is_falling", 76, void()>,
        VTableEntry<"stun_animation", 77, void()>,
        VTableEntry<"process_input", 78, void()>,
        VTableEntry<"collision_damage", 79, void()>,
        VTableEntry<"picked_up", 80, void()>,
        VTableEntry<"poof_particles", 82, void()>,
        VTableEntry<"fall_logic", 83, void()>,
        VTableEntry<"apply_friction", 84, void()>,
        VTableEntry<"crush", 90, void()>,
        VTableEntry<"fall_on", 91, void(uint32_t, Entity*)>,
        VTableEntry<"instakill", 92, void()>>;
    static MovableVTable movable_vtable(lua, lua["Movable"], "ENTITY_OVERRIDE");

    using FloorVTable = HookableVTable<
        Entity,
        CallbackType::Entity,
        EntityVTable,
        VTableEntry<"floor_update", 0x26, void()>>;
    static FloorVTable floor_vtable(lua, lua["Floor"], "ENTITY_OVERRIDE");

    using RenderInfoVTable = HookableVTable<
        RenderInfo,
        CallbackType::Entity,
        VTableEntry<"dtor", 0x0, void()>,
        VTableEntry<"render", 0x3, void(float*), BackBinder<VanillaRenderContext>>>;
    static RenderInfoVTable render_info_vtable(lua, lua["RenderInfo"], "RENDER_INFO_OVERRIDE");

    // Define the implementations for the LuaBackend handlers
    HookHandler<Entity, CallbackType::Entity>::set_hook_dtor_impl(
        [](std::uint32_t uid, std::function<void(std::uint32_t)> fun)
        {
            Entity* ent = get_entity_ptr(uid);
            std::uint32_t callback_id = entity_vtable.reserve_callback_id(ent);
            entity_vtable.set_pre<void(Entity*), 0x0>(
                ent,
                callback_id,
                [fun = std::move(fun)](Entity* ent_inner)
                { fun(ent_inner->uid); return false; });
            return callback_id;
        });
    HookHandler<Entity, CallbackType::Entity>::set_unhook_impl(
        [](std::uint32_t callback_id, std::uint32_t uid)
        {
            Entity* ent = get_entity_ptr(uid);
            entity_vtable.unhook(ent, callback_id);
        });

    HookHandler<RenderInfo, CallbackType::Entity>::set_hook_dtor_impl(
        [](std::uint32_t uid, std::function<void(std::uint32_t)> fun)
        {
            Entity* ent = get_entity_ptr(uid);
            std::uint32_t callback_id = render_info_vtable.reserve_callback_id(ent->rendering_info);
            render_info_vtable.set_pre<void(RenderInfo*), 0x0>(
                ent->rendering_info,
                callback_id,
                [fun = std::move(fun)](RenderInfo* render_info)
                { fun(render_info->get_aux_id()); return false; });
            return callback_id;
        });
    HookHandler<RenderInfo, CallbackType::Entity>::set_unhook_impl(
        [](std::uint32_t callback_id, std::uint32_t uid)
        {
            Entity* ent = get_entity_ptr(uid);
            render_info_vtable.unhook(ent->rendering_info, callback_id);
        });

    // Add support for hooking entity dtors
    Entity::set_hook_dtor_impl(
        [](Entity* ent, std::function<void(Entity*)> fun)
        {
            std::uint32_t callback_id = entity_vtable.reserve_callback_id(ent);
            entity_vtable.set_pre<void(Entity*), 0x0>(
                ent,
                callback_id,
                [fun = std::move(fun)](Entity* ent_inner)
                { fun(ent_inner); return false; });
            return callback_id;
        },
        [](Entity* ent, std::uint32_t callback_id)
        {
            entity_vtable.unhook(ent, callback_id);
        });
}
}; // namespace NVTables
