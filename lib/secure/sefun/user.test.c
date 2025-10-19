inherit M_TEST;

/**
 * @var {"/secure/sefun/user"} testOb
 */

int is_user () {
    return 1;
}
string query_setting (string name) {
    if (name == "test") {
        return "123";
    } else {
        return UNDEFINED;
    }
}

void test_user () {
    object user = new(STD_USER);
    user->set_name("test");

    expect("find_user behaves", (: ({
        assert_equal(testOb->find_user("unknown user"), 0),
        assert_equal(testOb->find_user("test"), $(user)),
    }) :));
    expect("this_user behaves", (: ({
        assert_equal(objectp(testOb->this_user()), 1),
        assert_equal(testOb->this_user()->is_user(), 1),
    }) :));
    expect("query_account_setting behaves", (: ({
        assert_equal(testOb->query_account_setting("test"), "123"),
        assert_equal(testOb->query_account_setting("invalid"), UNDEFINED),
    }) :));

    destruct(user);
}

void test_character () {
    object char = new(STD_CHARACTER);
    char->set_name("test");
    char->set_user(this_object());

    expect("find_character behaves", (: ({
        assert_equal(testOb->find_character("unknown character"), 0),
        assert_equal(testOb->find_character("test"), $(char)),
    }) :));
    // @TODO: re-assess this
    // expect("this_character behaves", (: ({
    //     assert_equal(objectp(testOb->this_character()), 1),
    //     // TODO: assert_equal(testOb->this_character()->is_character(), 1),
    // }) :));

    destruct(char);
}

void test_characters () {
    expect("characters returns an array", (: ({
        assert_equal(arrayp(testOb->characters()), 1),
    }) :));
}