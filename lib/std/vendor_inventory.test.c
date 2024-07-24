#include <verb.h>

inherit M_TEST;
inherit M_CONTAINER;

void test_is_vendor_inventory () {
    expect("is_item returns true", (: ({
        assert_equal(testOb->is_vendor_inventory(), 1),
        assert_equal(testOb->is_item(), 0),
    }) :));
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

void test_clean_up () {
    expect("clean up behaves", (: ({
        assert_equal($(testOb)->handle_move(this_object()), 1),
        assert_equal($(testOb)->clean_up(), 0),
        assert_equal($(testOb)->handle_remove(), 1),
    }) :));
}