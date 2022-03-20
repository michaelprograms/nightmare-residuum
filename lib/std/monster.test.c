inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/monster.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_npc () {
    int *values = ({}), *results = ({});

    expect_function("is_monster", testOb);
    expect_function("is_npc", testOb);

    expect("is_monster behaves", (: ({
        assert((: testOb->is_living() :), "==", 1),
        assert((: testOb->is_monster() :), "==", 1),
        assert((: testOb->is_npc() :), "==", 0),
        assert((: testOb->is_character() :), "==", 0),
    }) :));
}