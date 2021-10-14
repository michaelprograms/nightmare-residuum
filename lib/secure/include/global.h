#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// std
#define STD_OBJECT      "/std/object/object.c"
#define STD_ACCOUNT     "/std/account.c"
#define STD_CHARACTER   "/std/character.c"
#define STD_CONTAINER   "/std/container.c"
#define STD_LIVING      "/std/living/living.c"
#define STD_ROOM        "/std/room.c"

// std modules
#define M_CLEAN         "/std/module/clean.c"
#define M_EXIT          "/std/module/exit.c"
#define M_SAVE          "/std/module/save.c"

// secure modules
#define M_TEST          "/secure/module/test.c" // @TODO /secure/std/module/test

// daemon
#define D_ACCESS        "/secure/daemon/access.c"
#define D_ACCOUNT       "/secure/daemon/account.c"
#define D_CHARACTER     "/secure/daemon/character.c"
#define D_COMMAND       "/secure/daemon/command.c"
#define D_TEST          "/secure/daemon/test.c" // needs .c
#define D_ANSI          "/daemon/ansi.c"
#define D_SOUL          "/daemon/soul.c"
#define D_WELCOME       "/daemon/welcome.c"

#define OBJ_USER        "/secure/user/user"

#define SEFUN           "/secure/sefun/sefun"

#endif /* __GLOBAL_H__ */
