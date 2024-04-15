inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}
string *test_order () {
    return ({ "test_experience", "test_handle_victory", "test_handle_defeat", });
}

nosave private int __MockLevel, __MockLiving;
int query_level () { return __MockLevel; }
int is_living () { return __MockLiving; }
string query_cap_name () { return "Biography Test"; }

void test_experience () {
    expect_function("add_experience", testOb);
    expect_function("spend_experience", testOb);
    expect_function("query_experience", testOb);
    expect_function("query_total_experience", testOb);

    expect("handles adding and spending experience", (: ({
        assert_equal(testOb->query_experience(), 0),
        assert_equal(testOb->query_total_experience(), 0),

        assert_equal(testOb->add_experience(123), 0),
        assert_equal(testOb->query_experience(), 123),
        assert_equal(testOb->query_total_experience(), 0),

        assert_equal(testOb->add_experience(9000), 0),
        assert_equal(testOb->query_experience(), 9123),
        assert_equal(testOb->query_total_experience(), 0),

        assert_equal(testOb->spend_experience(1000), 0),
        assert_equal(testOb->query_experience(), 8123),
        assert_equal(testOb->query_total_experience(), 1000),

        assert_equal(testOb->spend_experience(2000), 0),
        assert_equal(testOb->query_experience(), 6123),
        assert_equal(testOb->query_total_experience(), 3000),
    }) :));
}

void test_handle_victory () {
    expect_function("handle_victory", testOb);
    expect_function("query_victory", testOb);
    expect_function("query_victory_average", testOb);

    // setup test object
    __MockLiving = 1;
    __MockLevel = 1;

    expect("handle_victory behaves", (: ({
        assert_equal(testOb->query_experience(), 0),
        assert_equal(testOb->query_victory(), 0),
        assert_equal(testOb->query_victory_average(), 0),
        assert_equal(testOb->handle_victory(this_object()), 0),
        assert_equal(testOb->query_experience() > 0, 1),
        assert_equal(testOb->query_victory(), 1),
        assert_equal(testOb->query_victory_average(), 1),

        __MockLevel = 3,
        assert_equal(testOb->handle_victory(this_object()), 0),
        assert_equal(testOb->query_victory(), 2),
        assert_equal(testOb->query_victory_average(), 2),
    }) :));

    // cleanup
    __MockLiving = 0;
    __MockLevel = 0;
}

void test_handle_defeat () {
    object r = new(STD_ROOM);
    object mockCharacter = new("/std/mock/character.c");

    expect_function("handle_defeat", testOb);
    expect_function("query_defeat", testOb);
    expect_function("set_defeated", testOb);
    expect_function("query_defeated", testOb);

    // setup test object
    if (testOb) destruct(testOb);
    testOb = new(STD_LIVING); // need biography's parent inherit living for handle_move

    expect("handle_defeat behaved", (: ({
        assert_equal(testOb->query_experience(), 0),
        assert_equal(testOb->query_defeat(), ({ })),
        assert_equal(testOb->handle_move($(r)), 1),
        assert_equal(sizeof($(r)->query_living_contents()), 1),
        assert_equal(sizeof($(r)->query_item_contents()), 0),

        // test defeat with keep
        assert_equal($(mockCharacter)->start_shadow(testOb), 1),
        assert_equal(testOb->is_character(), 1),
        assert_equal(testOb->query_defeated(), 0),
        assert_equal(testOb->handle_defeat(this_object()), 0),
        assert_equal(objectp(testOb), 1),
        assert_equal(testOb->query_defeated(), 1),
        assert_equal(testOb->query_defeat(), ({ ({ "Biography Test", time() }) })),
        assert_equal(sizeof($(r)->query_living_contents()), 0),
        assert_equal(sizeof($(r)->query_item_contents()), 1),
        assert_equal(!!present("corpse", $(r)), 1),
        assert_equal(present("corpse", $(r))->handle_remove(), 1),
        testOb->set_defeated(0),
        assert_equal(testOb->query_defeated(), 0),

        // move test object back
        assert_equal(testOb->handle_move($(r)), 1),
        assert_equal(sizeof($(r)->query_living_contents()), 1),
        assert_equal(sizeof($(r)->query_item_contents()), 0),

        // test defeat with keep
        assert_equal($(mockCharacter)->stop_shadow(), 1),
        assert_equal(testOb->handle_defeat(this_object()), 0),
        assert_equal(objectp(testOb), 0),
        assert_equal(sizeof($(r)->query_living_contents()), 0),
        assert_equal(sizeof($(r)->query_item_contents()), 1),
        assert_equal(!!present("corpse", $(r)), 1),
        assert_equal(present("corpse", $(r))->handle_remove(), 1),
    }) :));

    // cleanup
    if (r) destruct(r);
    if (mockCharacter) destruct(mockCharacter);
}