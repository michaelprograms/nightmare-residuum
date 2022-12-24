inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/array.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_distinct_array () {
    expect_function("distinct_array", testOb);

    expect("distinct_array handles inputs", (: ({
        assert(testOb->distinct_array(({ 1, 2, 2, 3, 2, 1, 3, 2, 1 })), "==", ({ 3, 2, 1 })),
        assert(testOb->distinct_array(({ 1, 4, 4, 3, 4, 1, 3, 2, 1 })), "==", ({ 4, 3, 2, 1 })),
    }) :));
    expect("distinct_array handles bad argument 1", (: ({
        assert((: testOb->distinct_array(({})) :), "catch", "*Bad argument 1 to array->distinct_array\n"),
        assert((: testOb->distinct_array(0) :), "catch", "*Bad argument 1 to array->distinct_array\n"),
        assert((: testOb->distinct_array() :), "catch", "*Bad argument 1 to array->distinct_array\n"),
    }) :));
}

void test_reduce () {
    expect_function("reduce", testOb);

    expect("reduce handles array", (: ({
        assert(testOb->reduce(({ 1, 2, 3, 4, 5 }), (: $1 + $2 :), 0), "==", 15),
        assert(testOb->reduce(({ 1, 3, 5, 7, 9, }), (: $1 + $2 :), 0), "==", 25),
    }) :));
    expect("reduce handles bad argument 1", (: ({
        assert((: testOb->reduce(this_object()) :), "catch", "*Bad argument 1 to array->reduce\n"),
        assert((: testOb->reduce("") :), "catch", "*Bad argument 1 to array->reduce\n"),
        assert((: testOb->reduce(1) :), "catch", "*Bad argument 1 to array->reduce\n"),
        assert((: testOb->reduce(1.0) :), "catch", "*Bad argument 1 to array->reduce\n"),
        assert((: testOb->reduce(({})) :), "catch", "*Bad argument 1 to array->reduce\n"),
        assert((: testOb->reduce(([])) :), "catch", "*Bad argument 1 to array->reduce\n"),
        assert((: testOb->reduce((: 1 :)) :), "catch", "*Bad argument 1 to array->reduce\n"),
    }) :));
    expect("reduce handles bad argument 2", (: ({
        assert((: testOb->reduce(({ 1 }), this_object()) :), "catch", "*Bad argument 2 to array->reduce\n"),
        assert((: testOb->reduce(({ 1 }), "") :), "catch", "*Bad argument 2 to array->reduce\n"),
        assert((: testOb->reduce(({ 1 }), 1) :), "catch", "*Bad argument 2 to array->reduce\n"),
        assert((: testOb->reduce(({ 1 }), 1.0) :), "catch", "*Bad argument 2 to array->reduce\n"),
        assert((: testOb->reduce(({ 1 }), ({})) :), "catch", "*Bad argument 2 to array->reduce\n"),
        assert((: testOb->reduce(({ 1 }), ([])) :), "catch", "*Bad argument 2 to array->reduce\n"),
    }) :));
    expect("reduce handles bad argument 3", (: ({
        assert((: testOb->reduce(({ 1 }), (: 1 :)) :), "catch", "*Bad argument 3 to array->reduce\n"),
        assert((: testOb->reduce(({ 1 }), (: 1 :), UNDEFINED) :), "catch", "*Bad argument 3 to array->reduce\n"),
    }) :));
}