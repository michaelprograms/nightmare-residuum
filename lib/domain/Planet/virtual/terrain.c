#include <planet.h>

inherit PLANET_VIRTUAL + "terrain.virtual.c";

void create() {
    ::create();
    set_layout_file("/domain/Planet/virtual/planet.txt");
    set_inheritable("/domain/Planet/virtual/room/base/terrain.c");
}