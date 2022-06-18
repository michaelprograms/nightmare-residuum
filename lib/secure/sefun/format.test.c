inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/format.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

nosave private object __MockAccount;
object query_account () {
    return __MockAccount;
}

void test_format_page () {
    expect_function("format_page", testOb);

    __MockAccount = new(STD_ACCOUNT);

    expect("format_page handled width=80", (: ({
        assert(testOb->format_page(({ "a", "b" })), "regex", "^a +b +$"),
        assert(testOb->format_page(({ "a", "b", "c" })), "regex", "^a +b +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 1), "regex", "^a +\nb +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 2), "regex", "^a +b +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 3), "regex", "^a +b +c +$"),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2), "\n")[0]), "==", 80),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, 1), "\n")[0]), "==", 78),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, -1), "\n")[0]), "==", 82),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2), "\n")[0]), "==", 80),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, 1), "\n")[0]), "==", 78),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, -1), "\n")[0]), "==", 82),
    }) :));

    __MockAccount->set_setting("width", 60);
    expect("format_page handled width=60", (: ({
        assert(testOb->format_page(({ "a", "b" })), "regex", "^a +b +$"),
        assert(testOb->format_page(({ "a", "b", "c" })), "regex", "^a +b +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 1), "regex", "^a +\nb +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 3), "regex", "^a +b +c +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 2), "regex", "^a +b +\nc +$"),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2), "\n")[0]), "==", 60),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, 1), "\n")[0]), "==", 58),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, -1), "\n")[0]), "==", 62),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2), "\n")[0]), "==", 60),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, 1), "\n")[0]), "==", 58),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, -1), "\n")[0]), "==", 62),
    }) :));

    __MockAccount->set_setting("width", 80);
    expect("format_page handled remainder of width/columns", (: ({
        assert(strlen(testOb->format_page(({ "a", "b", "c", "d", "e", }), 5, 4)), "==", 72), // remainder = 2
        assert(strlen(testOb->format_page(({ "a", "b", "c" }), 3, 0)), "==", 80), // remainder = 2
        assert(strlen(testOb->format_page(({ "1", "2", "3", "4", "5", "6", "7" }), 7, 0)), "==", 80), // remainder = 3
    }) :));

    expect("format_page handled trimming longer strings", (: ({
        assert(strlen(testOb->format_page(({ "111111111111111111111111111111", "222222222222222222222222222222", "333333333333333333333333333333", "444444444444444444444444444444", }), 4, 0)), "==", 80), // 30 * 4 = 120
        assert(strlen(testOb->format_page(({ "111111111111111111111111111111", "2", "3", "4", }), 4, 0)), "==", 80), // 30 + 25 * 3 = 85
    }) :));

    __MockAccount->set_setting("width", 20);
    expect("format_page left aligns strings", (: ({
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0), "==", "1    2    3    4    "),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0, 0), "==", "1    2    3    4    "),
        assert(testOb->format_page(({ "1" }), 1, 0, 0), "==", "1                   "),
        assert(testOb->format_page(({ "1", "2" }), 1, 0, 0), "==", "1                   \n2                   "),
    }) :));
    expect("format_page center aligns strings", (: ({
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0, 1), "==", "  1    2    3    4  "),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0, 1), "==", "  1    2    3    4  "),
        assert(testOb->format_page(({ "1" }), 1, 0, 1), "==", "          1         "),
        assert(testOb->format_page(({ "1", "2" }), 1, 0, 1), "==", "          1         \n          2         "),
    }) :));

    expect("format_page handled invalid argument 1", (: ({
        assert((: testOb->format_page() :), "catch", "*Bad argument 1 to format->format_page\n"),
        assert((: testOb->format_page(0) :), "catch", "*Bad argument 1 to format->format_page\n"),
        assert((: testOb->format_page(0.0) :), "catch", "*Bad argument 1 to format->format_page\n"),
        assert((: testOb->format_page(({})) :), "catch", "*Bad argument 1 to format->format_page\n"),
        assert((: testOb->format_page(([])) :), "catch", "*Bad argument 1 to format->format_page\n"),
        assert((: testOb->format_page((: 1 :)) :), "catch", "*Bad argument 1 to format->format_page\n"),
    }) :));

    destruct(__MockAccount);
}

void test_format_exit_brief () {
    expect_function("format_exit_brief", testOb);

    expect("format_exit_brief handled exits", (: ({
        assert(testOb->format_exit_brief("north"), "==", "n"),
        assert(testOb->format_exit_brief("northeast"), "==", "ne"),
        assert(testOb->format_exit_brief("east"), "==", "e"),
        assert(testOb->format_exit_brief("southeast"), "==", "se"),
        assert(testOb->format_exit_brief("south"), "==", "s"),
        assert(testOb->format_exit_brief("southwest"), "==", "sw"),
        assert(testOb->format_exit_brief("west"), "==", "w"),
        assert(testOb->format_exit_brief("northwest"), "==", "nw"),
        assert(testOb->format_exit_brief("enter"), "==", "ent"),
        assert(testOb->format_exit_brief("out"), "==", "out"),
        assert(testOb->format_exit_brief("enter north"), "==", "ent n"),
        assert(testOb->format_exit_brief("up"), "==", "u"),
        assert(testOb->format_exit_brief("down"), "==", "d"),
        assert(testOb->format_exit_brief("n"), "==", "n"),
        assert(testOb->format_exit_brief("exit"), "==", "exit"),
    }) :));

    expect("format_exit_brief handled invalid argument 1", (: ({
        assert((: testOb->format_exit_brief(0) :), "catch", "*Bad argument 1 to format->format_exit_brief\n"),
        assert((: testOb->format_exit_brief(0.0) :), "catch", "*Bad argument 1 to format->format_exit_brief\n"),
        assert((: testOb->format_exit_brief(({})) :), "catch", "*Bad argument 1 to format->format_exit_brief\n"),
        assert((: testOb->format_exit_brief(([])) :), "catch", "*Bad argument 1 to format->format_exit_brief\n"),
        assert((: testOb->format_exit_brief((: 1 :)) :), "catch", "*Bad argument 1 to format->format_exit_brief\n"),
    }) :));
}
void test_format_exit_verbose () {
    expect_function("format_exit_verbose", testOb);

    expect("format_exit_verbose handled exits", (: ({
        assert(testOb->format_exit_verbose("n"), "==", "north"),
        assert(testOb->format_exit_verbose("ne"), "==", "northeast"),
        assert(testOb->format_exit_verbose("e"), "==", "east"),
        assert(testOb->format_exit_verbose("se"), "==", "southeast"),
        assert(testOb->format_exit_verbose("s"), "==", "south"),
        assert(testOb->format_exit_verbose("sw"), "==", "southwest"),
        assert(testOb->format_exit_verbose("w"), "==", "west"),
        assert(testOb->format_exit_verbose("nw"), "==", "northwest"),
        assert(testOb->format_exit_verbose("ent"), "==", "enter"),
        assert(testOb->format_exit_verbose("out"), "==", "out"),
        assert(testOb->format_exit_verbose("ent n"), "==", "enter north"),
        assert(testOb->format_exit_verbose("u"), "==", "up"),
        assert(testOb->format_exit_verbose("d"), "==", "down"),
        assert(testOb->format_exit_verbose("north"), "==", "north"),
        assert(testOb->format_exit_verbose("exit"), "==", "exit"),
    }) :));

    expect("format_exit_verbose handled invalid argument 1", (: ({
        assert((: testOb->format_exit_verbose(0) :), "catch", "*Bad argument 1 to format->format_exit_verbose\n"),
        assert((: testOb->format_exit_verbose(0.0) :), "catch", "*Bad argument 1 to format->format_exit_verbose\n"),
        assert((: testOb->format_exit_verbose(({})) :), "catch", "*Bad argument 1 to format->format_exit_verbose\n"),
        assert((: testOb->format_exit_verbose(([])) :), "catch", "*Bad argument 1 to format->format_exit_verbose\n"),
        assert((: testOb->format_exit_verbose((: 1 :)) :), "catch", "*Bad argument 1 to format->format_exit_verbose\n"),
    }) :));
}
void test_format_exit_reverse () {
    expect_function("format_exit_reverse", testOb);

    expect("format_exit_reverse handled exits", (: ({
        assert(testOb->format_exit_reverse("south"), "==", "north"),
        assert(testOb->format_exit_reverse("southwest"), "==", "northeast"),
        assert(testOb->format_exit_reverse("west"), "==", "east"),
        assert(testOb->format_exit_reverse("northwest"), "==", "southeast"),
        assert(testOb->format_exit_reverse("north"), "==", "south"),
        assert(testOb->format_exit_reverse("northeast"), "==", "southwest"),
        assert(testOb->format_exit_reverse("east"), "==", "west"),
        assert(testOb->format_exit_reverse("southeast"), "==", "northwest"),
        assert(testOb->format_exit_reverse("out"), "==", "enter"),
        assert(testOb->format_exit_reverse("enter"), "==", "out"),
        assert(testOb->format_exit_reverse("enter north"), "==", "out south"),
        assert(testOb->format_exit_reverse("out north"), "==", "enter south"),
        assert(testOb->format_exit_reverse("down"), "==", "up"),
        assert(testOb->format_exit_reverse("up"), "==", "down"),
        assert(testOb->format_exit_reverse("exit"), "==", "exit"),
    }) :));

    expect("format_exit_reverse handled invalid argument 1", (: ({
        assert((: testOb->format_exit_reverse(0) :), "catch", "*Bad argument 1 to format->format_exit_reverse\n"),
        assert((: testOb->format_exit_reverse(0.0) :), "catch", "*Bad argument 1 to format->format_exit_reverse\n"),
        assert((: testOb->format_exit_reverse(({})) :), "catch", "*Bad argument 1 to format->format_exit_reverse\n"),
        assert((: testOb->format_exit_reverse(([])) :), "catch", "*Bad argument 1 to format->format_exit_reverse\n"),
        assert((: testOb->format_exit_reverse((: 1 :)) :), "catch", "*Bad argument 1 to format->format_exit_reverse\n"),
    }) :));
}

void test_format_stat_brief () {
    expect_function("format_stat_brief", testOb);

    expect("format_stat_brief handled exits", (: ({
        assert(testOb->format_stat_brief("strength"), "==", "str"),
        assert(testOb->format_stat_brief("perception"), "==", "per"),
        assert(testOb->format_stat_brief("endurance"), "==", "end"),
        assert(testOb->format_stat_brief("charisma"), "==", "cha"),
        assert(testOb->format_stat_brief("intelligence"), "==", "int"),
        assert(testOb->format_stat_brief("agility"), "==", "agi"),
        assert(testOb->format_stat_brief("luck"), "==", "lck"),
        assert(testOb->format_stat_brief("unknown"), "==", ""),
        assert(testOb->format_stat_brief("str"), "==", "str"),
        assert(testOb->format_stat_brief("per"), "==", "per"),
        assert(testOb->format_stat_brief("end"), "==", "end"),
        assert(testOb->format_stat_brief("cha"), "==", "cha"),
        assert(testOb->format_stat_brief("int"), "==", "int"),
        assert(testOb->format_stat_brief("agi"), "==", "agi"),
        assert(testOb->format_stat_brief("lck"), "==", "lck"),
    }) :));

    expect("format_stat_brief handled invalid argument 1", (: ({
        assert((: testOb->format_stat_brief(0) :), "catch", "*Bad argument 1 to format->format_stat_brief\n"),
        assert((: testOb->format_stat_brief(0.0) :), "catch", "*Bad argument 1 to format->format_stat_brief\n"),
        assert((: testOb->format_stat_brief(({})) :), "catch", "*Bad argument 1 to format->format_stat_brief\n"),
        assert((: testOb->format_stat_brief(([])) :), "catch", "*Bad argument 1 to format->format_stat_brief\n"),
        assert((: testOb->format_stat_brief((: 1 :)) :), "catch", "*Bad argument 1 to format->format_stat_brief\n"),
    }) :));
}
void test_format_stat_verbose () {
    expect_function("format_stat_verbose", testOb);

    expect("format_stat_brief handled exits", (: ({
        assert(testOb->format_stat_verbose("str"), "==", "strength"),
        assert(testOb->format_stat_verbose("per"), "==", "perception"),
        assert(testOb->format_stat_verbose("end"), "==", "endurance"),
        assert(testOb->format_stat_verbose("cha"), "==", "charisma"),
        assert(testOb->format_stat_verbose("int"), "==", "intelligence"),
        assert(testOb->format_stat_verbose("agi"), "==", "agility"),
        assert(testOb->format_stat_verbose("lck"), "==", "luck"),
        assert(testOb->format_stat_verbose("unknown"), "==", ""),
        assert(testOb->format_stat_verbose("strength"), "==", "strength"),
        assert(testOb->format_stat_verbose("perception"), "==", "perception"),
        assert(testOb->format_stat_verbose("endurance"), "==", "endurance"),
        assert(testOb->format_stat_verbose("charisma"), "==", "charisma"),
        assert(testOb->format_stat_verbose("intelligence"), "==", "intelligence"),
        assert(testOb->format_stat_verbose("agility"), "==", "agility"),
        assert(testOb->format_stat_verbose("luck"), "==", "luck"),
    }) :));

    expect("format_stat_verbose handled invalid argument 1", (: ({
        assert((: testOb->format_stat_verbose(0) :), "catch", "*Bad argument 1 to format->format_stat_verbose\n"),
        assert((: testOb->format_stat_verbose(0.0) :), "catch", "*Bad argument 1 to format->format_stat_verbose\n"),
        assert((: testOb->format_stat_verbose(({})) :), "catch", "*Bad argument 1 to format->format_stat_verbose\n"),
        assert((: testOb->format_stat_verbose(([])) :), "catch", "*Bad argument 1 to format->format_stat_verbose\n"),
        assert((: testOb->format_stat_verbose((: 1 :)) :), "catch", "*Bad argument 1 to format->format_stat_verbose\n"),
    }) :));
}

void test_format_integer () {
    expect_function("format_integer", testOb);

    expect("format_integer handled integers", (: ({
        // positives
        assert(testOb->format_integer(0), "==", "0"),
        assert(testOb->format_integer(1), "==", "1"),
        assert(testOb->format_integer(123), "==", "123"),
        assert(testOb->format_integer(1234), "==", "1,234"),
        assert(testOb->format_integer(123456), "==", "123,456"),
        assert(testOb->format_integer(1234567), "==", "1,234,567"),
        assert(testOb->format_integer(1234567890), "==", "1,234,567,890"),

        // negatives
        assert(testOb->format_integer(-0), "==", "0"),
        assert(testOb->format_integer(-1), "==", "-1"),
        assert(testOb->format_integer(-1234), "==", "-1,234"),
        assert(testOb->format_integer(-123456789), "==", "-123,456,789"),
        assert(testOb->format_integer(-1234567890), "==", "-1,234,567,890"),
    }) :));

    expect("format_integer handled invalid argument 1", (: ({
        assert((: testOb->format_integer("") :), "catch", "*Bad argument 1 to format->format_integer\n"),
        assert((: testOb->format_integer(0.0) :), "catch", "*Bad argument 1 to format->format_integer\n"),
        assert((: testOb->format_integer(({})) :), "catch", "*Bad argument 1 to format->format_integer\n"),
        assert((: testOb->format_integer(([])) :), "catch", "*Bad argument 1 to format->format_integer\n"),
        assert((: testOb->format_integer((: 1 :)) :), "catch", "*Bad argument 1 to format->format_integer\n"),
    }) :));
}