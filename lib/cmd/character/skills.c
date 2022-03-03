void command (string input) {
    object tc = this_character();
    string *skills = tc->query_all_skills();

    write(format_header_bar("SKILLS") + "\n\n");

    write("    %^BOLD%^Combat%^RESET%^\n");
    if (sizeof(skills)) {
        skills = sort_array(skills, 1);
        foreach (string skill in skills) {
            write(sprintf("%20s %10s %3s", skill, "level " + tc->query_skill(skill), tc->query_skill_progress(skill)) + "\n");
        }
    }

    write("\n" + format_footer_bar() + "\n");
}