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
        return 0;
    }
}
object query_character () {
    return this_object();
}
int is_character() {
    return 321;
}

void test_user () {
    object user = new(STD_USER);
    user->set_name("test");

    expect("find_user behaves", (: ({
        assert_equal(testOb->find_user("unknown user"), 0),
        assert_equal(testOb->find_user("test"), $(user)),

        assert_catch((: testOb->find_user(UNDEFINED) :), "*Bad argument 1 to user->find_user\n"),
    }) :));
    expect("this_user behaves", (: ({
        assert_equal(objectp(testOb->this_user()), 1),
        assert_equal(testOb->this_user()->is_user(), 1),
    }) :));
    expect("query_account_setting behaves", (: ({
        assert_equal(testOb->query_account_setting("test"), "123"),
        assert_equal(testOb->query_account_setting("invalid"), 0),
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

        assert_catch((: testOb->find_character(UNDEFINED) :), "*Bad argument 1 to user->find_character\n"),
    }) :));
    expect("this_character behaves", (: ({
        assert_equal(objectp(testOb->this_character()), 1),
        assert_equal(testOb->this_character()->is_character(), is_character()),
    }) :));

    destruct(char);
}

void test_characters () {
    expect("characters returns an array", (: ({
        assert_equal(arrayp(testOb->characters()), 1),
    }) :));
}