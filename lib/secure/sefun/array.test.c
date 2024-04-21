inherit M_TEST;

void test_distinct_array () {
    expect("distinct_array handles inputs", (: ({
        assert_equal(testOb->distinct_array(({ 1, 2, 2, 3, 2, 1, 3, 2, 1 })), ({ 3, 2, 1 })),
        assert_equal(testOb->distinct_array(({ 1, 4, 4, 3, 4, 1, 3, 2, 1 })), ({ 4, 3, 2, 1 })),
    }) :));
}

void test_reduce () {
    expect("reduce handles array", (: ({
        assert_equal(testOb->reduce(({ 1, 2, 3, 4, 5 }), (: $1 + $2 :), 0), 15),
        assert_equal(testOb->reduce(({ 1, 3, 5, 7, 9, }), (: $1 + $2 :), 0), 25),
    }) :));
}