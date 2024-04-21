inherit M_TEST;

nosave private int __MockLevel;
int query_level () { return __MockLevel; }

void test_autoload () {
    expect("set_autoload should set requirements", (: ({
        // items don't autoload by default
        assert_equal(arrayp(testOb->query_autoload(this_object())), 0),
        // set autoload to level 1 and our level to 0
        // set our level to 1
        testOb->set_autoload(1),
        __MockLevel = 0,
        // item doesn't autoload still
        assert_equal(arrayp(testOb->query_autoload(this_object())), 0),
        // set our level to 1
        __MockLevel = 1,
        assert_equal(arrayp(testOb->query_autoload(this_object())), 1),
        // set autoload to level 10 and our level to 5
        testOb->set_autoload(10),
        __MockLevel = 5,
        assert_equal(arrayp(testOb->query_autoload(this_object())), 0),
    }) :));

    expect("query_autoload_data should return an array", (: ({
        assert_equal(testOb->query_autoload_data(), ({ })),
    }) :));

    expect("restore_autoload does nothing", (: ({
        assert_equal(testOb->restore_autoload(({ })), UNDEFINED),
    }) :));
}