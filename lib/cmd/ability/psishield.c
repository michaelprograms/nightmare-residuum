#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("utility");
    set_ability_requirements(([
        "psionist": ([
            "level": 5,
        ]),
    ]));
    set_skill_powers(([ // only used for cost in utility
        "psionic": 10,
    ]));
    set_help_text("Use psionic powers to shield a target with bonus armor.");
}

void handle_utility (object source, object target, string limb) {
    int n, t;

    n = 1 + source->query_stat("intelligence") / 25;
    t = (n * 5) + 30;

    target->add_protection(n, t);
}