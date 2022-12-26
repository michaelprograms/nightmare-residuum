#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("heal");
    set_ability_requirements(([
        "mystic": ([
            "level": 1,
        ]),
    ]));
    set_skill_powers(([
        "medicine": 5,
        "theurgy": 10,
    ]));
    set_help_text("Chant soothing words towards a target to restore vitals.");
}

void handle_heal (object source, object target, string limb) {
    int n;

    n = 1 + random(5);
    n += random(source->query_skill("medicine"));
    n += random(to_int(source->query_skill("theurgy") / 2));

    if (target->query_hp() < target->query_max_hp()) {
        message("action", possessive_noun(target->query_cap_name()) + " wounds heal slightly.", environment(target), target);
        message("action", "Your wounds heal slightly.", target);
    }
    target->heal(n);

    if (source->query_immortal() || source->query_property("debug")) {
        message("action", "%^CYAN%^Heal:%^RESET%^ " + n, source);
    }
    if (target && (target->query_immortal() || target->query_property("debug")) && target != source) {
        message("action", "%^CYAN%^Heal:%^RESET%^ " + n, target);
    }
}