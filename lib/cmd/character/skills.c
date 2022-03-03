void command (string input) {
    object tc = this_character();
    string *skills = tc->query_all_skills(), *list = ({});
    int width, columns;

    write(format_header_bar("SKILLS") + "\n\n");

    if (sizeof(skills)) {
        width = this_user()->query_account()->query_setting("width") || 80;
        columns = width / 40;

        skills = sort_array(skills, 1);
        foreach (string skill in skills) {
            list += ({ sprintf("  %-18s %4s %4s", skill, "" + tc->query_skill(skill), tc->query_skill_progress(skill)) });
        }
        write(format_page(list, columns));
    }

    write("\n" + format_footer_bar() + "\n");
}