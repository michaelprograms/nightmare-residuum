inherit STD_LIVING;

int is_npc () { return 1; }

void create () {
    ::create();
    if (!clonep()) return;
    set_heart_beat(1);
}
// @TODO what does NPC need?
