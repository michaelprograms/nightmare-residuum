#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("utility");
    set_ability_requirements(([
        "psionist": ([
            "level": 5,
        ]),
    ]));
    set_help_text("Use psionic powers to restore a target's missing limb.");
}

void handle_utility (object source, object target, string limb) {
    string *severedLimbs = target->query_severed_limbs();

    if (!sizeof(severedLimbs)) {
        message("action", target->query_cap_name() + " has no missing limbs.", source);
        message("action", "Nothing happens.", environment(source), source);
        return;
    }

    limb = element_of(severedLimbs);
    target->handle_limb_restore(limb);

    message("action", possessive_noun(target->query_cap_name()) + " " + limb + " is regrows as it is restored.", environment(target), target);
    message("action", "Your " + limb + " regrows as it is restored.", target);
}