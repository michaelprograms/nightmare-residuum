#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_skill_powers(([
        "psionic": 10,
    ]));
    set_help_text("Zap your opponent with your mind.");
}