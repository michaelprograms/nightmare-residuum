#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_skill_powers(([
        "psionic": 8,
        "ranged": 2,
    ]));
    set_help_text("Zap your opponent with your mind.");
}