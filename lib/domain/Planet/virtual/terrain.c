#include <planet.h>

inherit PLANET_VIRTUAL + "terrain.virtual.c";

void create() {
    ::create();
    set_inheritable("/domain/Planet/virtual/room/base/terrain.c");
}