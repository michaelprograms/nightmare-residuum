inherit M_TEST;

/**
 * @var {"/secure/sefun/roll"} testOb
 */

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

    expect("roll_die handles bad inputs", (: ({
        assert_catch((: testOb->roll_die(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to roll_die\n"),

        assert_catch((: testOb->roll_die(1, UNDEFINED) :), "*Bad argument 2 to roll_die\n"),
        assert_catch((: testOb->roll_die(1, 1) :), "*Bad argument 2 to roll_die\n"),
    }) :));
}