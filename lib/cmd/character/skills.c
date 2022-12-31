inherit STD_COMMAND;

void create () {
    set_syntax("skills");
    set_help_text("The skills command is used to view the skillsets of your character.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *skillsCombat, *skillsTalents;
    mixed *body = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    skillsCombat = sort_array(filter_array(tc->query_all_skills(), (: regexp($1, "[a-z]+ attack|defense") :)), 1);
    if (sizeof(skillsCombat)) {
        body += ({
            ([
                "header": ({ "Combat" }),
                "columns": ({ 4, 1, 1, 4, 1, 1, }),
                "items": reduce(skillsCombat, (: $1 += ({ $2, $(target)->query_skill($2), $(target)->query_skill_progress($2) }) :), ({ })),
            ])
        });
    }

    skillsTalents = sort_array(filter_array(tc->query_all_skills(), (: member_array($1, ({ "medicine", "theurgy" })) > -1 :)), 1);
    if (sizeof(skillsTalents)) {
        body += ({
            ([
                "header": ({ "Talents" }),
                "columns": ({ 4, 1, 1, 4, 1, 1}),
                "items": reduce(skillsTalents, (: $1 += ({ $2, $(target)->query_skill($2), $(target)->query_skill_progress($2) }) :), ({ })),
            ])
        });
    }

    border(([
        "title": "SKILLS",
        "subtitle": target->query_cap_name(),
        "body": body,
    ]));
}