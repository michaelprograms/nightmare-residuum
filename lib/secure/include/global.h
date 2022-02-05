#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// std
#define STD_OBJECT      "/std/object/object.c"
#define STD_ACCOUNT     "/std/account.c"
#define STD_CHARACTER   "/std/character.c"
#define STD_CONTAINER   "/std/container.c"
#define STD_LIVING      "/std/living.c"
#define STD_ROOM        "/std/room.c"

// std modules
#define M_CLEAN         "/std/module/clean.c"
#define M_EXIT          "/std/module/exit.c"
#define M_PARENT        "/std/module/parent.c"
#define M_PROPERTY      "/std/module/property.c"
#define M_STORY         "/std/module/story.c"

// secure modules
#define M_SAVE          "/secure/module/save.c"
#define M_TEST          "/secure/module/test.c"

// daemons
#define D_ANSI          "/daemon/ansi.c"
#define D_CHANNEL       "/daemon/channel.c"
#define D_LOG           "/daemon/log.c"
#define D_SOUL          "/daemon/soul.c"
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