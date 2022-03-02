void command (string input) {
    object tc = this_character();
    string *skills = tc->query_all_skills();

    write(format_header_bar("SKILLS") + "\n\n");

    if (sizeof(skills)) {
        foreach (string skill in skills) {
            write(pad_left(skill, 15)+"    "+tc->query_skill(skill)+"    "+tc->query_skill_progress(skill)+"%\n");
        }
    }

    write("\n" + format_footer_bar() + "\n");
}