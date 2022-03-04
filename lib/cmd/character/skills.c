void command (string input) {
    object tc = this_character(), target = tc;
    string *skills = sort_array(tc->query_all_skills(), 1), *list = ({});
    int width, columns;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    write(format_header_bar("SKILLS", (target != tc ? target->query_name() : 0)) + "\n\n");

    if (sizeof(skills)) {
        width = this_user()->query_account()->query_setting("width") || 80;
        columns = width / 40;

        foreach (string skill in skills) {
            list += ({ sprintf("  %-18s %4s %4s", skill, "" + target->query_skill(skill), target->query_skill_progress(skill)) });
        }
        write(format_page(list, columns));
    }

    write("\n" + format_footer_bar() + "\n");
}