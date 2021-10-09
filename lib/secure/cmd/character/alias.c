inherit "/std/class/shell_alias.c";

private void print_all_aliases (object shell) {
    string output;
    class ShellAlias alias;

    write(format_header_bar("ALIAS", "List") + "\n\n");
    // @TODO cleanup columns
    write(sprintf("Alias:%9sExpansion:%29sDefaults:\n", "", "") + "\n");
    foreach (string name in sort_array(shell->query_alias_names(), 1)) {
        alias = shell->query_alias(name);
        output = sprintf("%-14s %-38s ", name, alias->template);
        if (sizeof(alias->defaults) == 1 && alias->defaults[0] == "") {
            output += "\n";
        } else {
            output += "$*: " + alias->defaults[0] + "\n";
            for (int i = 1; i < sizeof(alias->defaults); i ++) {
                output += sprintf("%54s$%d: %s\n", "", i, alias->defaults[i]);
            }
        }
        write(output);
    }
    write("\n" + format_footer_bar() + "\n");
}

void command (string input) {
    if (!input) {
        print_all_aliases(this_user()->query_shell());
    } else {
        // @TODO
    }
}