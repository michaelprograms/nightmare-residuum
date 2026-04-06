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
        // validate rolls are in range [1, 120]
        assert_equal(rollData[1][0] >= 1, 1),
        assert_equal(rollData[1][0] <= 2, 1),

        rollData = testOb->roll_die(10, 10),
        assert_equal(sizeof(rollData), 2), // sum and rolls
        assert_equal(sizeof(rollData[1]), 10), // 10 die rolls
        assert_equal(rollData[0] >= 1, 1), // sum
        assert_equal(rollData[0] <= 100, 1), // sum
        // validate rolls are in range [1, 10]
        assert_equal(rollData[1][0] >= 1, 1),
        assert_equal(rollData[1][0] <= 10, 1),
        assert_equal(rollData[1][1] >= 1, 1),
        assert_equal(rollData[1][1] <= 10, 1),
        assert_equal(rollData[1][2] >= 1, 1),
        assert_equal(rollData[1][2] <= 10, 1),
        assert_equal(rollData[1][3] >= 1, 1),
        assert_equal(rollData[1][3] <= 10, 1),
        assert_equal(rollData[1][4] >= 1, 1),
        assert_equal(rollData[1][4] <= 10, 1),
        assert_equal(rollData[1][5] >= 1, 1),
        assert_equal(rollData[1][5] <= 10, 1),
        assert_equal(rollData[1][6] >= 1, 1),
        assert_equal(rollData[1][6] <= 10, 1),
        assert_equal(rollData[1][7] >= 1, 1),
        assert_equal(rollData[1][7] <= 10, 1),
        assert_equal(rollData[1][8] >= 1, 1),
        assert_equal(rollData[1][8] <= 10, 1),
        assert_equal(rollData[1][9] >= 1, 1),
        assert_equal(rollData[1][9] <= 10, 1),
    }) :));

    expect("roll_die handles bad inputs", (: ({
        assert_catch((: testOb->roll_die(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to roll->roll_die\n"),
        assert_catch((: testOb->roll_die(0, 2) :), "*Bad argument 1 to roll->roll_die\n"),

        assert_catch((: testOb->roll_die(1, UNDEFINED) :), "*Bad argument 2 to roll->roll_die\n"),
        assert_catch((: testOb->roll_die(1, 1) :), "*Bad argument 2 to roll->roll_die\n"),

    }) :));
}