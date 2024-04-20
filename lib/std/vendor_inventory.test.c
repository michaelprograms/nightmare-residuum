#include <verb.h>

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

void test_receive () {
    object living, item;

    // create test items
    living = new(STD_LIVING);
    item = new(STD_ITEM);

    expect("inventory receives only items", (: ({
        assert_equal(testOb->handle_receive($(living)), 0),
        assert_equal(testOb->handle_receive($(item)), 1),
    }) :));

    if (living) destruct(living);
    if (item) destruct(item);
}

void test_max_items () {
    object item1, item2;

    // create test items
    item1 = new(STD_ITEM);
    item2 = new(STD_ITEM);

    expect("max items prevents receiving items", (: ({
        // set max items to 1
        testOb->set_max_items(1),
        assert_equal(testOb->query_max_items(), 1),
        // 0 of 1 items, receive passes
        assert_equal(testOb->handle_receive($(item1)), 1),
        // move item to inventory to count
        assert_equal($(item1)->handle_move(testOb), 1),
        // 1 of 1 items, receive fails
        assert_equal(testOb->handle_receive($(item2)), 0),
        // set max items to 2
        testOb->set_max_items(2),
        assert_equal(testOb->query_max_items(), 2),
        // 1 of 2 items, receive passes
        assert_equal(testOb->handle_receive($(item2)), 1),
    }) :));

    if (item1) destruct(item1);
    if (item2) destruct(item2);
}
