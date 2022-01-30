#define MAX_ALIAS_COUNT 100

inherit "/std/class/shell_alias.c";

private mapping __Aliases = ([]);
private string *__XAliases = ({});

nomask string *query_alias_names () {
    return keys(__Aliases);
}
nomask class ShellAlias query_alias (string alias) {
    return __Aliases[alias];
}

varargs void add_alias (string name, string template, string *defaults, int xverb) {
    class ShellAlias new_alias;
    int i;
    string tmp;

    if (sizeof(__Aliases) >= MAX_ALIAS_COUNT) {
        write("You have reached the maxmimum amount of aliases.\n");
        return;
    }

    if (strsrch(template, "$*") == -1) {
        template += " $*";
    }

    new_alias = new(class ShellAlias);
    new_alias->template = template;
    new_alias->defaults = defaults;
    tmp = explode(template[strsrch(template, "$")..], "$");
    new_alias->num_args = max(map(tmp, function (string s) {
        int d;
        sscanf(s, "%d%s", d, s);
        return d;
    }));

    if (!arrayp(new_alias->defaults)) {
        new_alias->defaults = ({ });
    }
    i = new_alias->num_args - (sizeof(defaults) - 1);
    while (i--) {
        new_alias->defaults += ({ "" });
    }

    if (xverb) {
        __XAliases += ({ name });
    }
    __Aliases[name] = new_alias;
    this_object()->save_data();
}

// @TODO remove_alias

mixed expand_alias (string input) {
    string *argv = explode(input, " ");
    class ShellAlias currentAlias;
    string *xverbMatches, expandedInput;
    int numArgs = sizeof(argv) - 1, i, j;

    xverbMatches = filter_array(__XAliases, (: strsrch($2, $1) == 0 :), argv[0]);
    if (sizeof(xverbMatches) != 1) {
        error("Alias conflict: can't distinguish between " + implode(xverbMatches, ", "));
    }

    sscanf(argv[0], xverbMatches[0] + "%s", argv[0]);
    if (argv[0] == "") {
        argv[0] = xverbMatches[0];
    } else {
        numArgs ++;
        argv = xverbMatches + argv;
    }

    if (!(currentAlias = __Aliases[argv[0]])) {
        return trim(implode(argv, " "));
    }

    expandedInput = replace_string(currentAlias->template, "\\\\$", sprintf("%c", 255));

    for (i = 1, j = numArgs; i <= currentAlias->num_args; i ++, j --) {
        if (j < 1) {
            expandedInput = replace_string(expandedInput, sprintf("$%d", i), currentAlias->defaults[i]);
        } else {
            expandedInput = replace_string(expandedInput, sprintf("$%d", i), argv[i]);
        }
    }
    if (j > 0) {
        expandedInput = replace_string(expandedInput, "$*", implode(argv[i..], " "));
    } else {
        expandedInput = replace_string(expandedInput, "$*", currentAlias->defaults[0]);
    }

    return trim(replace_string(expandedInput, sprintf("%c", 255), "$"));
}

void create () {
    if (clonep()) {
        // @TODO defaults shouldn't count against player num
        if (!sizeof(__Aliases)) {
            add_alias("l", "look");
            add_alias("n", "go north");
            add_alias("ne", "go northeast");
            add_alias("e", "go east");
            add_alias("se", "go southeast");
            add_alias("s", "go south");
            add_alias("sw", "go southwest");
            add_alias("w", "go west");
            add_alias("nw", "go northwest");
            add_alias("u", "go up");
            add_alias("d", "go down");
            add_alias("en", "enter");
            // add_alias("cl", "climb");
            add_alias("'", "say", 0, 1);
        }
    }
}