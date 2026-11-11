inherit M_TEST;

/**
 * @var {"/std/user/shell"} testOb
 */

private int action = 0;

void test_variables() {
    expect("variables are queryable and settable", (: ({
        assert_equal(testOb->query_variable("testkey1"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey1", 123), 123),
        assert_equal(testOb->query_variable("testkey1"), 123),

        assert_equal(testOb->unset_variable("testkey1"), 1),
        assert_equal(testOb->query_variable("testkey1"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey1", "xyz"), "xyz"),
        assert_equal(testOb->query_variable("testkey1"), "xyz"),
    }) :));

    expect("variable_if_undefined handled", (: ({
        assert_equal(testOb->query_variable("testkey2"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey2", 123, 1), 123),
        assert_equal(testOb->query_variable("testkey2"), 123),

        assert_equal(testOb->set_variable("testkey2", 987, 1), 123),  // didn't set

        assert_equal(testOb->unset_variable("testkey2"), 1),
        assert_equal(testOb->query_variable("testkey2"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey2", 987, 1), 987),  // did set
    }) :));
}

void test_variable_hooks() {
    testOb->set_variable_hook("testkey", function(mixed value) {
        action++;
        return value;
    });

    expect("set_variable_hook handles hook", (: ({
        assert_equal(action, 0),

        assert_equal(testOb->set_variable("testkey", 123), 123),
        assert_equal(action, 1),

        assert_equal(testOb->set_variable("testkey", "xyz"), "xyz"),
        assert_equal(action, 2),
    }) :));
}

void test_aliases() {
    expect("aliases are queryable, settable, and removable", (: ({
        assert_equal(
            testOb->query_alias_names(),
            ({
                "'",
                "cl",
                "d",
                "e",
                "ent",
                "l",
                "n",
                "ne",
                "nw",
                "s",
                "se",
                "sw",
                "u",
                "w"
            })
        ),
        assert_equal(testOb->query_alias("testalias1"), UNDEFINED),

        testOb->set_alias("testalias1", "test alias 1 expansion"),
        assert_equal(
            testOb->query_alias("testalias1"),
            ([
                "d": ({ "" }),
                "n": UNDEFINED,
                "t": "test alias 1 expansion $*"
            ])
        ),

        testOb->unset_alias("testalias1"),
        assert_equal(testOb->query_alias("testalias1"), UNDEFINED),

        testOb->set_alias("testalias1", "xyz"),
        assert_equal(
            testOb->query_alias("testalias1"),
            ([ "d": ({ "" }), "n": UNDEFINED, "t": "xyz $*" ])
        ),
    }) :));
}

/* ----- prompt ----- */

// query_prompt is protected, so a copy of the instrumented shell exposes it
#define PROMPT_SHELL "/tmp/shell_prompt.c"

// stands in for the character, see /std/character.c
int query_hp() {
    return 1;
}
int query_max_hp() {
    return 2;
}
int query_sp() {
    return 3;
}
int query_max_sp() {
    return 4;
}
int query_mp() {
    return 5;
}
int query_max_mp() {
    return 6;
}

void test_query_prompt() {
    object shell;

    write_file(PROMPT_SHELL, "inherit \"/std/user/shell.coverage.c\";\n"
        "object __Character;\n"
        "void set_character(object ob) { __Character = ob; }\n"
        "object query_character() { return __Character; }\n"
        "mixed test_query_prompt() { return query_prompt(); }\n", 1);
    // generated at runtime in /tmp, unknown to the static checker
    // @lpc-ignore
    shell = new(PROMPT_SHELL);
    shell->set_character(this_object());

    expect("query_prompt expands tokens", (: ({
        $(shell)->set_variable("prompt", "$hp/$HP hp $sp/$SP sp $mp/$MP mp >"),
        assert_equal($(shell)->test_query_prompt(), "1/2 hp 3/4 sp 5/6 mp > "),

        $(shell)->set_variable("cwd", "/domain"),
        $(shell)->set_variable("prompt", "$cd >"),
        assert_equal($(shell)->test_query_prompt(), "/domain > "),
    }) :));

    expect("query_prompt handles unknown tokens", (: ({
        // an unknown token only drops its own $
        $(shell)->set_variable("prompt", "$x $hp >"),
        assert_equal($(shell)->test_query_prompt(), "x 1 > "),
        $(shell)->set_variable("prompt", "$hp $x $HP"),
        assert_equal($(shell)->test_query_prompt(), "1 x 2 "),

        // too short to be a token
        $(shell)->set_variable("prompt", ">$"),
        assert_equal($(shell)->test_query_prompt(), ">$ "),

        $(shell)->set_variable("prompt", 0),
        assert_equal($(shell)->test_query_prompt(), " "),
    }) :));

    if (shell) {
        destruct(shell);
    }
    if (find_object(PROMPT_SHELL)) {
        destruct(find_object(PROMPT_SHELL));
    }
    rm(PROMPT_SHELL);
}
