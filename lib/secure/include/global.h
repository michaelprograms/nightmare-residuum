#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// std
#define STD_CHARACTER   "/std/character.c"
#define STD_CONSUMABLE  "/std/consumable.c"
#define STD_DATABASE    "/std/database.c"
#define STD_INJECTABLE  "/std/injectable.c"
#define STD_ITEM        "/std/item.c"
#define STD_LIVING      "/std/living.c"
#define STD_NPC         "/std/npc.c"
#define STD_OBJECT      "/std/object.c"
#define STD_ROOM        "/std/room.c"
#define STD_USER        "/std/user.c"

// std actions
#define STD_ABILITY     "/std/ability.c"
#define STD_COMMAND     "/std/command.c"
#define STD_VERB        "/std/verb.c"

// std item inheritables
#define STD_ARMOR       "/std/item/armor.c"
#define STD_COINS       "/std/item/coins.c"
#define STD_CORPSE      "/std/item/corpse.c"
#define STD_DRINK       "/std/item/drink.c"
#define STD_FOOD        "/std/item/food.c"
#define STD_KEY         "/std/item/key.c"
#define STD_STORAGE     "/std/item/storage.c"
#define STD_WEAPON      "/std/item/weapon.c"

// std npc inheritables
#define STD_VENDOR      "/std/npc/vendor.c"

// std modules
#define M_AUTOLOAD      "/std/module/autoload.c"
#define M_BONUS         "/std/module/bonus.c"
#define M_CONTAINER     "/std/module/container.c"
#define M_CURRENCY      "/std/module/currency.c"
#define M_CLEAN         "/std/module/clean.c"
#define M_DUSTABLE      "/std/module/dustable.c"
#define M_EXIT          "/std/module/exit.c"
#define M_LEVELABLE     "/std/module/levelable.c"
#define M_LOOK          "/std/module/look.c"
#define M_MOVE          "/std/module/move.c"
#define M_PARSE         "/std/module/parse.c"
#define M_PICKABLE      "/std/module/pickable.c"
#define M_PROPERTY      "/std/module/property.c"
#define M_RESET         "/std/module/reset.c"
#define M_SENSES        "/std/module/senses.c"
#define M_STORY         "/std/module/story.c"
#define M_TEST          "/std/module/test.c"

// secure modules
#define M_SAVE          "/secure/module/save.c"

// daemons
#define D_ANSI          "/daemon/ansi.c"
#define D_ASTRONOMY     "/daemon/astronomy.c"
#define D_CHANNEL       "/daemon/channel.c"
#define D_CLASS         "/daemon/class.c"
#define D_EXPERIENCE    "/daemon/experience.c"
#define D_LOG           "/daemon/log.c"
#define D_PLANET        "/daemon/planet.c"
#define D_SOUL          "/daemon/soul.c"
#define D_SPECIES       "/daemon/species.c"
#define D_WELCOME       "/daemon/welcome.c"

// secure daemons
#define D_ACCESS        "/secure/daemon/access.c"
#define D_ACCOUNT       "/secure/daemon/account.c"
#define D_BANK          "/secure/daemon/bank.c"
#define D_CHARACTER     "/secure/daemon/character.c"
#define D_COMMAND       "/secure/daemon/command.c"
#define D_IPC           "/secure/daemon/ipc.c"
#define D_TEST          "/secure/daemon/test.c"

#define MASTER          "/secure/daemon/master"
#define SEFUN           "/secure/sefun/sefun"

#endif /* __GLOBAL_H__ */