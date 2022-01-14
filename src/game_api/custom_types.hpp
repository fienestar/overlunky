#pragma once

#include "aliases.hpp"
#include <span>
#include <string_view>
#include <vector>

enum class CUSTOM_TYPE : uint32_t
{
    ACIDBUBBLE = 1000, // to be sure that's it's bigger than ENT_TYPE_LIQUID_COARSE_LAVA
    ALIEN,
    ALTAR,
    AMMIT,
    ANKHPOWERUP,
    ANUBIS,
    APEPHEAD,
    APEPPART,
    ARROW,
    ARROWTRAP,
    AXOLOTL,
    AXOLOTLSHOT,
    BACKPACK,
    BAT,
    BEE,
    BEG,
    BGBACKLAYERDOOR,
    BGEGGSHIPROOM,
    BGFLOATINGDEBRIS,
    BGMOVINGSTAR,
    BGRELATIVEELEMENT,
    BGSHOOTINGSTAR,
    BGSHOPENTRENCE,
    BGSHOPKEEPERPRIME,
    BGSURFACELAYER,
    BGSURFACESTAR,
    BGTUTORIALSIGN,
    BIGSPEARTRAP,
    BIRDIES,
    BODYGUARD,
    BOMB,
    BONEBLOCK,
    BOOMBOX,
    BOOMERANG,
    BOULDER,
    BOULDERSPAWNER,
    BULLET,
    BURNINGROPEEFFECT,
    BUTTON,
    CAMERAFLASH,
    CAPE,
    CATMUMMY,
    CAVEMAN,
    CAVEMANSHOPKEEPER,
    CHAIN,
    CHAINEDPUSHBLOCK,
    CHEST,
    CINEMATICANCHOR,
    CITYOFGOLDDOOR,
    CLAMBASE,
    CLAW,
    CLIMBABLEROPE,
    CLONEGUNSHOT,
    COBRA,
    COFFIN,
    COIN,
    CONTAINER,
    CONVEYORBELT,
    COOKFIRE,
    CRABMAN,
    CRITTER,
    CRITTERBEETLE,
    CRITTERBUTTERFLY,
    CRITTERCRAB,
    CRITTERDRONE,
    CRITTERFIREFLY,
    CRITTERFISH,
    CRITTERLOCUST,
    CRITTERPENGUIN,
    CRITTERSLIME,
    CRITTERSNAIL,
    CROCMAN,
    CROSSBEAM,
    CRUSHTRAP,
    CURSEDEFFECT,
    CURSEDPOT,
    DECORATEDDOOR,
    DECOREGENERATINGBLOCK,
    DESTRUCTIBLEBG,
    DMALIENBLAST,
    DMSPAWNING,
    DOOR,
    DRILL,
    DUSTWALLAPEP,
    EGGPLANTMINISTER,
    EGGPLANTTHROWER,
    EGGSAC,
    EGGSHIPCENTERJETFLAME,
    EGGSHIPDOOR,
    EGGSHIPDOORS,
    ELEVATOR,
    EMPRESSGRAVE,
    ENTITY,
    EXCALIBUR,
    EXITDOOR,
    EXPLOSION,
    FALLINGPLATFORM,
    FIREBALL,
    FIREBUG,
    FIREBUGUNCHAINED,
    FIREFROG,
    FISH,
    FLAME,
    FLAMESIZE,
    FLOOR,
    FLY,
    FLYHEAD,
    FORCEFIELD,
    FORESTSISTER,
    FROG,
    FROSTBREATHEFFECT,
    FROZENLIQUID,
    FXALIENBLAST,
    FXANKHBROKENPIECE,
    FXANKHROTATINGSPARK,
    FXCOMPASS,
    FXEMPRESS,
    FXFIREFLYLIGHT,
    FXHUNDUNNECKPIECE,
    FXJELLYFISHSTAR,
    FXJETPACKFLAME,
    FXKINGUSLIDING,
    FXLAMASSUATTACK,
    FXMAINEXITDOOR,
    FXNECROMANCERANKH,
    FXOUROBORODRAGONPART,
    FXOUROBOROOCCLUDER,
    FXPICKUPEFFECT,
    FXPLAYERINDICATOR,
    FXQUICKSAND,
    FXSALECONTAINER,
    FXSHOTGUNBLAST,
    FXSORCERESSATTACK,
    FXSPARKSMALL,
    FXSPRINGTRAPRING,
    FXTIAMATHEAD,
    FXTIAMATTAIL,
    FXTIAMATTORSO,
    FXTORNJOURNALPAGE,
    FXUNDERWATERBUBBLE,
    FXVATBUBBLE,
    FXWATERDROP,
    FXWEBBEDEFFECT,
    FXWITCHDOCTORHINT,
    GENERATOR,
    GHIST,
    GHOST,
    GHOSTBREATH,
    GIANTCLAMTOP,
    GIANTFISH,
    GIANTFLY,
    GIANTFROG,
    GOLDBAR,
    GOLDMONKEY,
    GRUB,
    GUN,
    HANGANCHOR,
    HANGSPIDER,
    HANGSTRAND,
    HERMITCRAB,
    HONEY,
    HORIZONTALFORCEFIELD,
    HORNEDLIZARD,
    HOVERPACK,
    HUNDUN,
    HUNDUNCHEST,
    HUNDUNHEAD,
    ICESLIDINGSOUND,
    IDOL,
    IMP,
    JETPACK,
    JIANGSHI,
    JUMPDOG,
    JUNGLESPEARCOSMETIC,
    JUNGLETRAPTRIGGER,
    KAPALAPOWERUP,
    KINGU,
    LAHAMU,
    LAMASSU,
    LAMPFLAME,
    LANDMINE,
    LASERBEAM,
    LASERTRAP,
    LAVA,
    LAVAMANDER,
    LEAF,
    LEPRECHAUN,
    LIGHTARROW,
    LIGHTARROWPLATFORM,
    LIGHTEMITTER,
    LIGHTSHOT,
    LIMBANCHOR,
    LIQUID,
    LIQUIDSURFACE,
    LOCKEDDOOR,
    LOGICALANCHOVYFLOCK,
    LOGICALCONVEYORBELTSOUND,
    LOGICALDOOR,
    LOGICALDRAIN,
    LOGICALLIQUIDSTREAMSOUND,
    LOGICALMINIGAME,
    LOGICALREGENERATINGBLOCK,
    LOGICALSOUND,
    LOGICALSTATICSOUND,
    LOGICALTRAPTRIGGER,
    MAGMAMAN,
    MAINEXIT,
    MANTRAP,
    MATTOCK,
    MECH,
    MEGAJELLYFISH,
    MINIGAMEASTEROID,
    MINIGAMESHIP,
    MINIGAMESHIPOFFSET,
    MOLE,
    MONKEY,
    MONSTER,
    MOSQUITO,
    MOTHERSTATUE,
    MOUNT,
    MOVABLE,
    MOVINGICON,
    MUMMY,
    MUMMYFLIESSOUND,
    NECROMANCER,
    NPC,
    OCTOPUS,
    OLMEC,
    OLMECCANNON,
    OLMECFLOATER,
    OLMITE,
    ONFIREEFFECT,
    ORB,
    OSIRISHAND,
    OSIRISHEAD,
    OUROBOROCAMERAANCHOR,
    OUROBOROCAMERAZOOMIN,
    PALACESIGN,
    PARACHUTEPOWERUP,
    PET,
    PIPE,
    PIPETRAVELERSOUND,
    PLAYER,
    PLAYERBAG,
    PLAYERGHOST,
    POISONEDEFFECT,
    POLEDECO,
    PORTAL,
    POT,
    POWERUP,
    POWERUPCAPABLE,
    PROTOSHOPKEEPER,
    PUNISHBALL,
    PUSHBLOCK,
    QILIN,
    QUICKSAND,
    QUICKSANDSOUND,
    QUILLBACK,
    REGENBLOCK,
    ROBOT,
    ROCKDOG,
    ROLLINGITEM,
    ROOMLIGHT,
    ROOMOWNER,
    RUBBLE,
    SCARAB,
    SCEPTERSHOT,
    SCORPION,
    SHIELD,
    SHOOTINGSTARSPAWNER,
    SHOPKEEPER,
    SKELETON,
    SKULLDROPTRAP,
    SLEEPBUBBLE,
    SLIDINGWALLCEILING,
    SNAPTRAP,
    SORCERESS,
    SOUNDSHOT,
    SPARK,
    SPARKTRAP,
    SPEAR,
    SPECIALSHOT,
    SPIDER,
    SPIKEBALLTRAP,
    SPLASHBUBBLEGENERATOR,
    STICKYTRAP,
    STRETCHCHAIN,
    SWITCH,
    TADPOLE,
    TELEPORTER,
    TELEPORTERBACKPACK,
    TELEPORTINGBORDER,
    TELESCOPE,
    TENTACLE,
    TENTACLEBOTTOM,
    TERRA,
    THINICE,
    TIAMAT,
    TIAMATSHOT,
    TIMEDFORCEFIELD,
    TIMEDPOWDERKEG,
    TIMEDSHOT,
    TORCH,
    TORCHFLAME,
    TOTEMTRAP,
    TRANSFERFLOOR,
    TRAPPART,
    TREASURE,
    TREASUREHOOK,
    TRIGGER,
    TRUECROWNPOWERUP,
    TUN,
    TV,
    UDJATSOCKET,
    UFO,
    UNCHAINEDSPIKEBALL,
    USHABTI,
    VAMPIRE,
    VANHORSING,
    VLAD,
    VLADSCAPE,
    WADDLER,
    WALKINGMONSTER,
    WALLTORCH,
    WEBSHOT,
    WETEFFECT,
    WITCHDOCTOR,
    WITCHDOCTORSKULL,
    WOODENLOGTRAP,
    YAMA,
    YANG,
    YELLOWCAPE,
    YETIKING,
    YETIQUEEN,
};

std::span<const ENT_TYPE> get_custom_entity_types(CUSTOM_TYPE type);
bool is_type_movable(ENT_TYPE type);
const std::map<CUSTOM_TYPE, std::string_view>& get_custom_types_map();
