inherit M_TEST;

/**
 * @var {"/secure/sefun/user"} testOb
 */


void test_user () {
    object user = new(STD_USER);
    user->set_name("test");

    expect("find_user behaves", (: ({
        assert_equal(testOb->find_user("unknown user"), 0),
        assert_equal(testOb->find_user("test"), $(user)),
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

    destruct(char);
}

void test_characters () {
    expect("characters returns an array", (: ({
        assert_equal(arrayp(testOb->characters()), 1),
    }) :));
}