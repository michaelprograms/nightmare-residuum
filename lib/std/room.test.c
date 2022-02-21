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
    expect_true(testOb->is_room(), "room->is_room returns true");
}
