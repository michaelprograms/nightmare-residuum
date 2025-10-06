inherit M_TEST;
inherit STD_OBJECT;

/**
 * @var {"/std/command"} testOb
 */

string *test_order () {
    return ({
        "test_name",
        "test_syntax",
        "test_target",
        "test_help_text",
        "test_help_similar",
        "test_handle_help",
    });
}

void test_name () {
    expect("handles command name", (: ({
        assert_equal(testOb->query_name(), "command.coverage"),
    }) :));
}

void test_syntax () {
    expect("syntax is settable and queryable", (: ({
        assert_equal(testOb->query_syntax(), UNDEFINED),

        testOb->set_syntax(testOb->query_name()),
        assert_equal(testOb->query_syntax(), "<" + testOb->query_name() + ">"),

        testOb->set_syntax(testOb->query_name() + " [target]"),
        assert_equal(testOb->query_syntax(), "<" + testOb->query_name() + " [target]>"),
    }) :));
    expect("syntax handles bad inputs", (: ({
        assert_catch((: testOb->set_syntax("") :), "*Bad argument 1 to command->set_syntax\n"),
    }) :));
}

void test_target () {
    object r = new(STD_ROOM);
    object char = new(STD_CHARACTER);
    object npc = new(STD_NPC);

    char->set_name("test character");
    npc->set_name("test npc");

    expect("handles immortal targets", (: ({
        // check initial lack of environments
        assert_equal(environment(testOb), 0),
        assert_equal(environment(), 0),

        // assert_equal(testOb->determine_immortal_target(this_object(), "test character"), $(char)),

        // check mismatch environments but character
        assert_equal(handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal($(char)->handle_move($(r)), 1),
        assert_equal(testOb->determine_immortal_target(this_object(), "testcharacter"), this_object()),  // failure

        // check matching environments but NPC
        assert_equal(handle_move($(r)), 1),
        assert_equal($(npc)->handle_move($(r)), 1),
        assert_equal(testOb->determine_immortal_target(this_object(), "testnpc"), $(npc)),  // success

        // cleanup
        assert_equal(this_object()->handle_move("/domain/Nowhere/room/void.c"), 1),
    }) :));

    if (npc) destruct(npc);
    if (char) destruct(char);
    if (r) destruct(r);
}

void test_help_text () {
    expect("handles help text", (: ({
        assert_equal(testOb->query_help_text(), UNDEFINED),

        testOb->set_help_text("Help text."),
        assert_equal(testOb->query_help_text(), "Help text."),

        testOb->set_help_text("Different help text."),
        assert_equal(testOb->query_help_text(), "Different help text."),
    }) :));
    expect("help text handles bad inputs", (: ({
        assert_catch((: testOb->set_help_text("") :), "*Bad argument 1 to command->set_help_text\n"),
    }) :));
}

void test_help_similar () {
    expect("handles help similar", (: ({
        assert_equal(testOb->query_help_similar(), UNDEFINED),

        testOb->set_help_similar(({ "a", "b", "c", })),
        assert_equal(testOb->query_help_similar(), ({ "a", "b", "c", })),

        testOb->set_help_similar(({ "1", "2", "3", })),
        assert_equal(testOb->query_help_similar(), ({ "1", "2", "3", })),
    }) :));
    expect("help similar handles bad inputs", (: ({
        assert_catch((: testOb->set_help_similar(UNDEFINED) :), "*Bad argument 1 to command->set_help_similar\n"),
    }) :));
}

void test_handle_help () {
    expect("handles formatting help file", (: ({
        // should contain Syntax section always
        assert_equal(regexp(testOb->handle_help(this_object()), "Syntax"), 1),

        // only contain Description section if set
        assert_equal(regexp(testOb->handle_help(this_object()), "Description"), 0),
        assert_equal(regexp(testOb->handle_help(this_object()), "Help text"), 0),
        testOb->set_help_text("Help text"),
        assert_equal(regexp(testOb->handle_help(this_object()), "Description"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "Help text"), 1),

        // only contain Similar Actions section if set
        assert_equal(regexp(testOb->handle_help(this_object()), "Similar Actions"), 0),
        assert_equal(regexp(testOb->handle_help(this_object()), "A, B, C"), 0),
        testOb->set_help_similar(({ "A", "B", "C", })),
        assert_equal(regexp(testOb->handle_help(this_object()), "Similar Actions"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "A, B, C"), 1),
    }) :));
}