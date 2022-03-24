inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/room.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_is_room () {
    expect_function("is_room", testOb);

    expect("is_room returns true", (: ({
        assert(testOb->is_room(), "==", 1),
    }) :));
}
