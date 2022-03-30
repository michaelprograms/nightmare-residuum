#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// std
#define STD_ACCOUNT     "/std/account.c"
#define STD_ARMOR       "/std/armor.c"
#define STD_CHARACTER   "/std/character.c"
#define STD_CONTAINER   "/std/container.c"
#define STD_FOOD        "/std/food.c"
#define STD_ITEM        "/std/item.c"
#define STD_LIVING      "/std/living.c"
#define STD_MONSTER     "/std/monster.c"
#define STD_NPC         "/std/npc.c"
#define STD_OBJECT      "/std/object.c"
#define STD_ROOM        "/std/room.c"
#define STD_VERB        "/std/verb.c"
#define STD_WEAPON      "/std/weapon.c"

// std modules
#define M_CURRENCY      "/std/module/currency.c"
#define M_CLEAN         "/std/module/clean.c"
#define M_MOVE          "/std/module/move.c"
#define M_PARENT        "/std/module/parent.c"
#define M_PICKABLE      "/std/module/pickable.c"
#define M_PROPERTY      "/std/module/property.c"
#define M_RESET         "/std/module/reset.c"
#define M_STORY         "/std/module/story.c"
#define M_TEST          "/std/module/test.c"
#define M_TRAIN         "/std/module/train.c"

// secure modules
#define M_SAVE          "/secure/module/save.c"

// daemons
#define D_ANSI          "/daemon/ansi.c"
#define D_CHANNEL       "/daemon/channel.c"
#define D_EXPERIENCE    "/daemon/experience.c"
#define D_LOG           "/daemon/log.c"
#define D_SOUL          "/daemon/soul.c"
#define D_SPECIES       "/daemon/species.c"
#define D_WELCOME       "/daemon/welcome.c"

// secure daemons
#define D_ACCESS        "/secure/daemon/access.c"
#define D_ACCOUNT       "/secure/daemon/account.c"
#define D_CHARACTER     "/secure/daemon/character.c"
#define D_COMMAND       "/secure/daemon/command.c"
#define D_IPC           "/secure/daemon/ipc.c"
#define D_TEST          "/secure/daemon/test.c" // needs .c

#define OBJ_USER        "/secure/user/user"

#define MASTER          "/secure/daemon/master"
#define SEFUN           "/secure/sefun/sefun"

#endif /* __GLOBAL_H__ */