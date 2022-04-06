inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/format.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_ignore () { return ::test_ignore() + ({ "query_account" }); }

nosave private object __MockAccount;
object query_account () {
    return __MockAccount;
}

void test_format_header_bar () {
    expect_function("format_header_bar", testOb);

    __MockAccount = new(STD_ACCOUNT);
    __MockAccount->set_setting("ansi", "off"); // @TODO test for ansi on

    expect("format_header_bar handled screenreader=off ansi=off width=80", (: ({
        assert(testOb->format_header_bar("Title"), "regex", "Title"),
        assert(testOb->format_header_bar("Other Title"), "regex", "Other Title"),
        assert(testOb->format_header_bar("Title2", "optional"), "regex", "Title2: optional"),
        assert(testOb->format_header_bar("Something", "else"), "regex", "Something: else"),
        assert(testOb->format_header_bar("Title"), "==", "==/ Title \\====================================================================="),
        assert(strlen(testOb->format_header_bar("Title")), "==", 80),
    }) :));

    __MockAccount->set_setting("width", 60);
    expect("format_header_bar handled screenreader=off ansi=off width=60", (: ({
        assert(testOb->format_header_bar("Title"), "==", "==/ Title \\================================================="),
        assert(strlen(testOb->format_header_bar("Title")), "==", 60),
    }) :));

    __MockAccount->set_setting("screenreader", "on");
    expect("format_header_bar handled screenreader=on", (: ({
        assert(testOb->format_header_bar("Title"), "==", "Title"),
        assert(testOb->format_header_bar("Title", "optional"), "==", "Title: optional"),
        assert(testOb->format_header_bar("Something", "else"), "==", "Something: else"),
    }) :));

    expect("format_header_bar handled invalid argument 1", (: ({
        assert((: testOb->format_header_bar(0) :), "catch", "*Bad argument 1 to format->format_header_bar\n"),
        assert((: testOb->format_header_bar(0.0) :), "catch", "*Bad argument 1 to format->format_header_bar\n"),
        assert((: testOb->format_header_bar(({})) :), "catch", "*Bad argument 1 to format->format_header_bar\n"),
        assert((: testOb->format_header_bar(([])) :), "catch", "*Bad argument 1 to format->format_header_bar\n"),
        assert((: testOb->format_header_bar((: 1 :)) :), "catch", "*Bad argument 1 to format->format_header_bar\n"),
    }) :));

    destruct(__MockAccount);
}
void test_format_divider_bar () {
    expect_function("format_divider_bar", testOb);

    __MockAccount = new(STD_ACCOUNT);
    __MockAccount->set_setting("ansi", "off"); // @TODO test for ansi on

    expect("format_divider_bar handled screenreader=off ansi=off width=80", (: ({
        assert(testOb->format_divider_bar(), "regex", "----"),
        assert(sizeof(testOb->format_divider_bar()), "==", __MockAccount->query_setting("width")),
    }) :));

    __MockAccount->set_setting("screenreader", "on");
    expect("format_divider_bar handled screenreader=on", (: ({
        assert(testOb->format_divider_bar(), "==", ""),
        assert(sizeof(testOb->format_divider_bar()), "==", 0),
    }) :));

    destruct(__MockAccount);
}
void test_format_footer_bar () {
    expect_function("format_footer_bar", testOb);

    __MockAccount = new(STD_ACCOUNT);
    __MockAccount->set_setting("ansi", "off"); // @TODO test for ansi on

    expect("format_footer_bar handled screenreader=off ansi=off width=80", (: ({
        assert(testOb->format_footer_bar(), "regex", "====="),
        assert(strlen(testOb->format_footer_bar()), "==", 80),
    }) :));

    __MockAccount->set_setting("screenreader", "on");
    expect("format_footer_bar handled screenreader=on", (: ({
        assert(testOb->format_footer_bar(), "==", ""),
        assert(strlen(testOb->format_footer_bar()), "==", 0),
    }) :));

    destruct(__MockAccount);
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
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2), "\n")[0]), "==", 60),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, 1), "\n")[0]), "==", 58),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, -1), "\n")[0]), "==", 62),
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