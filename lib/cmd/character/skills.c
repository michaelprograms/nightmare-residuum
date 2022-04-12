void command (string input) {
    object tc = this_character(), target = tc;
    string *skills;
    string *border, subtitle, *body = ({});

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    skills = sort_array(tc->query_all_skills(), 1);
    if (sizeof(skills)) {
        mapping b = ([
            "header": "Combat",
            "columns": 2,
            "items": ({ }),
        ]);
        foreach (string skill in skills) {
            b["items"] += ({ sprintf("%-18s %4s %4s", skill, "" + target->query_skill(skill), target->query_skill_progress(skill)) });
        }
        body += ({ b });
    }

    border = format_border(([
        "title": "SKILLS",
        "subtitle": target->query_cap_name(),
        "body": body,
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}