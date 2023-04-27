inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/roll.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

mixed *rollData;
void test_roll_die () {
    expect_function("roll_die", testOb);

    expect("roll_die returns valid values", (: ({
        rollData = testOb->roll_die(1, 2),
        assert(sizeof(rollData), "==", 2), // sum and rolls
        assert(sizeof(rollData[1]), "==", 1), // 1 die roll
        assert(rollData[0], ">=", 1), // sum
        assert(rollData[0], "<=", 2), // sum

        rollData = testOb->roll_die(10, 10),
        assert(sizeof(rollData), "==", 2), // sum and rolls
        assert(sizeof(rollData[1]), "==", 10), // 1 die roll
        assert(rollData[0], ">=", 1), // sum
        assert(rollData[0], "<=", 100), // sum
    }) :));
}