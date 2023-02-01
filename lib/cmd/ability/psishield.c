#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("utility");
    set_ability_requirements(([
        "psionist": ([
            "level": 5,
            "skills": ([
                "psionic defense": 25,
            ]),
        ]),
    ]));
    set_help_text("Use psionic powers to shield a target with bonus armor.");
}

void handle_utility (object source, object target, string limb) {
    int n, t;

    n = 1 + source->query_skill("psionic defense") / 25;
    t = (n * 5) + 30 + source->query_stat("intelligence");

    target->add_protection(n, t);
}