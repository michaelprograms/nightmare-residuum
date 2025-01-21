#include <verb.h>
/**
 * @var {"/std/vendor_inventory"} testOb
 */

inherit M_TEST;
inherit M_CONTAINER;

void test_is_vendor_inventory () {
    expect("is_item returns true", (: ({
        assert_equal(testOb->is_vendor_inventory(), 1),
        assert_equal(itemp(testOb), 0),
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

void test_clean_up () {
    expect("clean up behaves", (: ({
        assert_equal($(testOb)->handle_move(this_object()), 1),
        assert_equal($(testOb)->clean_up(), 0),
        assert_equal($(testOb)->handle_remove(), 1),
    }) :));
}

void test_clear_inventory () {
    object item = new(STD_ITEM);

    expect("clear inventory removes all items", (: ({
        assert_equal(all_inventory(testOb), ({ })),

        assert_equal($(item)->handle_move(testOb), 1),

        assert_equal(all_inventory(testOb), ({ $(item) })),

        testOb->clear_inventory(),
        assert_equal(all_inventory(testOb), ({ })),
    }) :));

    if (item) destruct(item);
}