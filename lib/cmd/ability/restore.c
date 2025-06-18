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
    set_help_text("Use psionic powers to restore a target's missing limb.");
}

/**
 * @param {STD_LIVING} source
 * @param {STD_LIVING} target
 * @param limb
 */
void handle_utility (object source, object target, string limb) {
    string *severedLimbs = target->query_severed_limbs();
    int result;

    if (!sizeof(severedLimbs)) {
        message("action", target->query_cap_name() + " has no missing limbs.", source);
        message("action", "Nothing happens.", environment(source), source);
        return;
    }

    // TODO: allow this to be targeted
    limb = element_of(severedLimbs);
    result = target->handle_limb_restore(limb);
    if (result == -1) {
        message("action", target->query_cap_name()+" is missing the limb the "+limb+" is attached to.", source);
        message("action", "Nothing happens.", target);
    } else {
        message("action", possessive_noun(target->query_cap_name()) + " " + limb + " is regrows as it is restored.", environment(target), target);
        message("action", "Your " + limb + " regrows as it is restored.", target);
    }
}