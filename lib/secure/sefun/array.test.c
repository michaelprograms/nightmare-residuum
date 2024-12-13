inherit M_TEST;

/**
 * @var {"/secure/sefun/array"} testOb
 */

void test_distinct_array () {
    expect("distinct_array handles inputs", (: ({
        assert_equal(testOb->distinct_array(({ 1, 2, 2, 3, 2, 1, 3, 2, 1 })), ({ 3, 2, 1 })),
        assert_equal(testOb->distinct_array(({ 1, 4, 4, 3, 4, 1, 3, 2, 1 })), ({ 4, 3, 2, 1 })),

        assert_equal(testOb->distinct_array(({ })), ({ })),
    }) :));
}

void test_reduce () {
    expect("reduce handles array", (: ({
        assert_equal(testOb->reduce(({ 1, 2, 3, 4, 5 }), (: $1 + $2 :), 0), 15),
        assert_equal(testOb->reduce(({ 1, 3, 5, 7, 9, }), (: $1 + $2 :), 0), 25),
    }) :));
    expect("reduce handles bad inputs", (: ({
        assert_catch((: testOb->reduce(0, 0, UNDEFINED) :), "*Bad argument 1 to array->reduce\n"),

        assert_catch((: testOb->reduce(({ 1, 2, 3 }), 0, UNDEFINED) :), "*Bad argument 2 to array->reduce\n"),

        assert_catch((: testOb->reduce(({ 1, 2, 3 }), (: $1 + $2 :), UNDEFINED) :), "*Bad argument 3 to array->reduce\n"),
    }) :));
}