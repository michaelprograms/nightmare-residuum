nosave private string __Name;
nosave private string __Syntax;
nosave private string __HelpText;
nosave private string *__HelpSimilar;

string query_name () {
    return __Name;
}

void set_syntax (string str) {
    if (!sizeof(str)) error("Bad argument 1 to command->set_syntax");
    __Syntax = format_syntax(str);
}
string query_syntax () {
    return __Syntax;
}

/* ----- help ----- */

string query_help_text () {
    return __HelpText;
}
void set_help_text (string str) {
    if (!sizeof(str)) error("Bad argument 1 to command->set_help_text");

    __HelpText = str;
}
string *query_help_similar () {
    return __HelpSimilar;
}
void set_help_similar (string *similar) {
    if (!arrayp(similar) || !sizeof(similar)) error("Bad argument 1 to command->set_help_similar");

    __HelpSimilar = similar;
}

string handle_help (object char) {
    string result;

    result = "\n%^CYAN%^BOLD%^" + sprintf("%-12s", "Syntax") + "%^RESET%^\n" + query_syntax() + "\n";

    if (sizeof(__HelpText) > 0) {
        result += "\n%^CYAN%^BOLD%^Description%^RESET%^\n" + __HelpText + "\n";
    }
    if (sizeof(__HelpSimilar) > 0) {
        result += "\n%^CYAN%^BOLD%^Similar Actions%^RESET%^\n" + implode(__HelpSimilar, ", ") + "\n";
    }
    return result;
}

/* ----- applies ----- */

void create () {
    __Name = split_path(base_name())[1];
}