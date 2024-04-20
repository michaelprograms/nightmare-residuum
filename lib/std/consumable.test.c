inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

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
    return ({ "test_is_consumable", "test_strength", "test_handle_consume", });
}

void test_is_consumable () {
    expect("is_consumable returns true", (: ({
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_consumable(), 1),
        assert_equal(testOb->is_drink(), 0),
        assert_equal(testOb->is_food(), 0),
    }) :));
}

void test_strength () {
    expect("consumable handles strength", (: ({
        assert_equal(testOb->query_strength(), 0),
        testOb->set_strength(5),
        assert_equal(testOb->query_strength(), 5),
        testOb->set_strength(100),
        assert_equal(testOb->query_strength(), 100),
        testOb->set_strength(0),
        assert_equal(testOb->query_strength(), 0),
        testOb->set_strength(-5),
        assert_equal(testOb->query_strength(), -5),
    }) :));
}

void test_handle_consume () {
    object r = new(STD_ROOM);

    expect("consumable handle_consume behaves", (: ({
        // setup consumable
        testOb->set_strength(5),
        assert_equal(testOb->query_strength(), 5),

        // setup test object
        assert_equal(this_object()->handle_move($(r)), 1),
        assert_equal(this_object()->update_vitals(), 0), // initialize vitals
        assert_equal(this_object()->add_hp(5), 0),
        assert_equal(this_object()->query_hp(), 5),
        assert_equal(this_object()->add_sp(5), 0),
        assert_equal(this_object()->query_sp(), 5),
        assert_equal(this_object()->add_mp(5), 0),
        assert_equal(this_object()->query_mp(), 5),

        // test consuming
        assert_equal(testOb->handle_consume(this_object()), 0),
        assert_equal(this_object()->query_hp(), 10),
        assert_equal(this_object()->query_sp(), 10),
        assert_equal(this_object()->query_mp(), 10),

        assert_equal(objectp(testOb), 0), // consumable was removed
    }) :));

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}