meta.name = "Door Randomizer"
meta.version = "WIP"
meta.description = "Random exit doors and boss order. Beat all the bosses and become one with the cosmos to win!"
meta.author = "Dregu"

register_option_int("min_levels", "Min normal levels before boss", 3, 1, 100)
register_option_int("max_levels", "Max normal levels before boss", 8, 1, 100)
register_option_int("bosses", "Bosses to defeat to trigger ending", 5, 1, 5)
register_option_bool("xiit", "Spawn exit on entrance (for debugging)", false)

theme = {1,2,3,5,6,7,8,9,10,11}
bosses = {THEME.OLMEC, THEME.ABZU, THEME.DUAT, THEME.TIAMAT, THEME.HUNDUN}
bosses_left = {}
world = {1,2,2,3,4,4,5,6,7,8,4,4,4,6,7,7,1}
normal_levels = 0
nextworld = 1
nextlevel = 1
nexttheme = 1
reallevel = 1
realtheme = 1
boss_level = false

function set_doors()
  if #players > 0 and (state.theme == THEME.TIAMAT or state.theme == THEME.HUNDUN) then
    px, py, pl = get_position(players[1].uid)
    doors = get_entities_at(ENT_TYPE.FLOOR_DOOR_EXIT, 0, px, py, pl, 10)
    for i,v in ipairs(doors) do
      set_door_target(v, nextworld, nextlevel, nexttheme)
      x, y, layer = get_position(v)
      unlock_door_at(x, y);
    end
  end
end

function random_level()
  -- rare eggplant world
  if math.random(120) == 1 then
    nexttheme = THEME.EGGPLANT_WORLD
    nextlevel = 2

  -- pick a boss level from the remaining
  elseif #bosses_left > #bosses-options.bosses and normal_levels >= options.min_levels and math.random(options.max_levels) <= normal_levels then
    boss_level = true
    normal_levels = 0
    nexttheme = bosses_left[math.random(#bosses_left)]
    nextlevel = 4
    if nexttheme == THEME.OLMEC then
      nextlevel = 1
    end

  -- all bosses killed, go to 7-98
  elseif #bosses_left <= #bosses-options.bosses and normal_levels >= options.min_levels and math.random(options.max_levels) <= normal_levels then
    boss_level = false
    nexttheme = THEME.COSMIC_OCEAN
    nextlevel = 98
    toast("Lets get out of here!")

  -- pick a regular level
  else
    boss_level = false
    normal_levels = normal_levels+1
    nexttheme = theme[math.random(#theme)]
    nextlevel = math.random(4)
    if nexttheme == THEME.COSMIC_OCEAN then
      nextlevel = math.random(5, 97)
    elseif nexttheme == THEME.ICE_CAVES then
      nextlevel = 1
    end
  end
  nextworld = state.world
  reallevel = nextlevel
  nextlevel = 1
  if nexttheme == THEME.COSMIC_OCEAN then
    nextlevel = 5
  end
  realtheme = nexttheme
  if state.theme ~= THEME.OLMEC then
    nexttheme = state.theme
  else
    nexttheme = 1
  end
end

function init_run()
  message("Started new run")
  bosses_left = {table.unpack(bosses)}
  normal_levels = 0
  boss_level = false
  random_level()
end
init_run()

function remove_boss(boss)
  for k,v in ipairs(bosses_left) do
    if v == boss then
      table.remove(bosses_left, k)
      toast("Boss defeated!\nBosses remaining: "..tostring(#bosses_left-(#bosses-options.bosses)))
      return
    end
  end
end

function dead_olmec()
  if state.theme == THEME.OLMEC then
    olmecs = get_entities_by_type(ENT_TYPE.ACTIVEFLOOR_OLMEC)
    if #olmecs > 0 then
      x, y, l = get_position(olmecs[1])
      if y < 71 then -- this olmec is low enough
        remove_boss(THEME.OLMEC)
      end
    end
  end
end

function dead_tiamat()
  if state.theme == THEME.TIAMAT then
    tiamats = get_entities_by_type(ENT_TYPE.MONS_TIAMAT)
    if #tiamats > 0 then
      tiamat = get_entity(tiamats[1])
      if testflag(tiamat.flags, 29) then -- this tiamat is dead
        remove_boss(THEME.TIAMAT)
      end
    end
  end
end

function dead_hundun()
  if state.theme == THEME.HUNDUN then
    hunduns = get_entities_by_type(ENT_TYPE.MONS_HUNDUN)
    if #hunduns > 0 then
      if get_entity_ai_state(hunduns[1]) == 4 then -- this hundun is just chillin on the floor
        remove_boss(THEME.HUNDUN)
      end
    end
  end
end

function dead_kingu()
  if testflag(state.journal_flags, 16) then
    remove_boss(THEME.ABZU)
  end
end

function dead_osiris()
  if testflag(state.journal_flags, 17) then
    remove_boss(THEME.DUAT)
  end
end

function duat_door()
  -- spawn duat skip door
  spawn_door(17, 106, 0, nextworld, nextlevel, nexttheme)
  spawn_entity(ENT_TYPE.BG_DOOR_BACK_LAYER, 17, 106, 0, 0, 0)
end

function on_camp()
  init_run()
end

function on_level()
  if state.level_count == 0 then
    init_run()
  end

  if state.theme == THEME.DUAT then
    duat_door()
  end

  random_level()

  if options.xiit and #players > 0 then 
    x, y, l = get_position(players[1].uid)
    door(x, y, l, nextworld, nextlevel, nexttheme)
  end

  set_interval(function()
    set_doors()
    dead_olmec()
    dead_tiamat()
    dead_hundun()
    dead_kingu()
    dead_osiris()
  end, 15)
end

function on_transition()
  if state.level < 98 then
    toast("Level "..tostring(state.level_count).." completed!\nBosses remaining: "..tostring(#bosses_left-(#bosses-options.bosses)))
    state.theme_next = realtheme
    state.world_next = world[state.theme_next]
    state.level_next = reallevel
  end
end

function on_guiframe()
  -- force level even when engine isn't running
  if state.level < 98 and (state.pause == 3 or (state.pause == 2 and state.level_count == 0)) then
    --random_level()
    state.theme_next = realtheme
    state.world_next = world[state.theme_next]
    state.level_next = reallevel
  end
end
