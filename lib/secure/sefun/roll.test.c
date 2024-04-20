inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

mixed *rollData;
void test_roll_die () {
    expect("roll_die returns valid values", (: ({
        rollData = testOb->roll_die(1, 2),
        assert_equal(sizeof(rollData), 2), // sum and rolls
        assert_equal(sizeof(rollData[1]), 1), // 1 die roll
        assert_equal(rollData[0] >= 1, 1), // sum
        assert_equal(rollData[0] <= 2, 1), // sum

        rollData = testOb->roll_die(10, 10),
        assert_equal(sizeof(rollData), 2), // sum and rolls
        assert_equal(sizeof(rollData[1]), 10), // 1 die roll
        assert_equal(rollData[0] >= 1, 1), // sum
        assert_equal(rollData[0] <= 100, 1), // sum
    }) :));
}