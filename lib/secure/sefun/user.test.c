inherit M_TEST;

/**
 * @var {"/secure/sefun/user"} testOb
 */


void test_user () {
    object user = new(STD_USER);
    user->set_name("test");

    expect("find_user behaves", (: ({
        assert_equal(testOb->find_user("test"), $(user)),
    }) :));

    destruct(user);
}

void test_characters () {
    expect("characters returns an array", (: ({
        assert_equal(arrayp(testOb->characters()), 1),
    }) :));
}