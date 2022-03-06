inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/format.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_format_exit_brief () {
    string *values = ({}), *results = ({});

    expect_function("format_exit_brief", testOb);

    values += ({ testOb->format_exit_brief("north") });
    results += ({ "n" });
    values += ({ testOb->format_exit_brief("northeast") });
    results += ({ "ne" });
    values += ({ testOb->format_exit_brief("east") });
    results += ({ "e" });
    values += ({ testOb->format_exit_brief("southeast") });
    results += ({ "se" });
    values += ({ testOb->format_exit_brief("south") });
    results += ({ "s" });
    values += ({ testOb->format_exit_brief("southwest") });
    results += ({ "sw" });
    values += ({ testOb->format_exit_brief("west") });
    results += ({ "w" });
    values += ({ testOb->format_exit_brief("northwest") });
    results += ({ "nw" });
    values += ({ testOb->format_exit_brief("enter") });
    results += ({ "ent" });
    values += ({ testOb->format_exit_brief("out") });
    results += ({ "out" });
    values += ({ testOb->format_exit_brief("enter north") });
    results += ({ "ent n" });
    values += ({ testOb->format_exit_brief("up") });
    results += ({ "u" });
    values += ({ testOb->format_exit_brief("down") });
    results += ({ "d" });
    values += ({ testOb->format_exit_brief("n") });
    results += ({ "n" });
    values += ({ testOb->format_exit_brief("exit") });
    results += ({ "exit" });

    expect_arrays_equal(values, results, "format_exit_brief handled exits");

    expect_catches (({
        (: testOb->format_exit_brief(0) :),
        (: testOb->format_exit_brief(0.0) :),
        (: testOb->format_exit_brief(({})) :),
        (: testOb->format_exit_brief(([])) :),
        (: testOb->format_exit_brief((: 1 :)) :),
    }), "*Bad argument 1 to format->format_exit_brief\n", "format_exit_brief handled invalid argument 1");
}
void test_format_exit_verbose () {
    string *values = ({}), *results = ({});

    expect_function("format_exit_verbose", testOb);

    values += ({ testOb->format_exit_verbose("n") });
    results += ({ "north" });
    values += ({ testOb->format_exit_verbose("ne") });
    results += ({ "northeast" });
    values += ({ testOb->format_exit_verbose("e") });
    results += ({ "east" });
    values += ({ testOb->format_exit_verbose("se") });
    results += ({ "southeast" });
    values += ({ testOb->format_exit_verbose("s") });
    results += ({ "south" });
    values += ({ testOb->format_exit_verbose("sw") });
    results += ({ "southwest" });
    values += ({ testOb->format_exit_verbose("w") });
    results += ({ "west" });
    values += ({ testOb->format_exit_verbose("nw") });
    results += ({ "northwest" });
    values += ({ testOb->format_exit_verbose("ent") });
    results += ({ "enter" });
    values += ({ testOb->format_exit_verbose("out") });
    results += ({ "out" });
    values += ({ testOb->format_exit_verbose("ent n") });
    results += ({ "enter north" });
    values += ({ testOb->format_exit_verbose("u") });
    results += ({ "up" });
    values += ({ testOb->format_exit_verbose("d") });
    results += ({ "down" });
    values += ({ testOb->format_exit_verbose("north") });
    results += ({ "north" });
    values += ({ testOb->format_exit_verbose("exit") });
    results += ({ "exit" });

    expect_arrays_equal(values, results, "format_exit_verbose handled exits");

    expect_catches (({
        (: testOb->format_exit_verbose(0) :),
        (: testOb->format_exit_verbose(0.0) :),
        (: testOb->format_exit_verbose(({})) :),
        (: testOb->format_exit_verbose(([])) :),
        (: testOb->format_exit_verbose((: 1 :)) :),
    }), "*Bad argument 1 to format->format_exit_verbose\n", "format_exit_verbose handled invalid argument 1");
}

void test_format_exit_reverse () {
    string *values = ({}), *results = ({});

    expect_function("format_exit_reverse", testOb);

    values += ({ testOb->format_exit_reverse("south") });
    results += ({ "north" });
    values += ({ testOb->format_exit_reverse("southwest") });
    results += ({ "northeast" });
    values += ({ testOb->format_exit_reverse("west") });
    results += ({ "east" });
    values += ({ testOb->format_exit_reverse("northwest") });
    results += ({ "southeast" });
    values += ({ testOb->format_exit_reverse("north") });
    results += ({ "south" });
    values += ({ testOb->format_exit_reverse("northeast") });
    results += ({ "southwest" });
    values += ({ testOb->format_exit_reverse("east") });
    results += ({ "west" });
    values += ({ testOb->format_exit_reverse("southeast") });
    results += ({ "northwest" });
    values += ({ testOb->format_exit_reverse("out") });
    results += ({ "enter" });
    values += ({ testOb->format_exit_reverse("enter") });
    results += ({ "out" });
    values += ({ testOb->format_exit_reverse("enter north") });
    results += ({ "out south" });
    values += ({ testOb->format_exit_reverse("out north") });
    results += ({ "enter south" });
    values += ({ testOb->format_exit_reverse("down") });
    results += ({ "up" });
    values += ({ testOb->format_exit_reverse("up") });
    results += ({ "down" });
    values += ({ testOb->format_exit_reverse("exit") });
    results += ({ "exit" });

    expect_arrays_equal(values, results, "format_exit_reverse handled exits");

    expect_catches (({
        (: testOb->format_exit_reverse(0) :),
        (: testOb->format_exit_reverse(0.0) :),
        (: testOb->format_exit_reverse(({})) :),
        (: testOb->format_exit_reverse(([])) :),
        (: testOb->format_exit_reverse((: 1 :)) :),
    }), "*Bad argument 1 to format->format_exit_reverse\n", "format_exit_reverse handled invalid argument 1");
}

void test_format_stat_brief () {
    string *values = ({}), *results = ({});

    expect_function("format_stat_brief", testOb);

    values += ({ testOb->format_stat_brief("strength") });
    results += ({ "str" });
    values += ({ testOb->format_stat_brief("perception") });
    results += ({ "per" });
    values += ({ testOb->format_stat_brief("endurance") });
    results += ({ "end" });
    values += ({ testOb->format_stat_brief("charisma") });
    results += ({ "cha" });
    values += ({ testOb->format_stat_brief("intelligence") });
    results += ({ "int" });
    values += ({ testOb->format_stat_brief("agility") });
    results += ({ "agi" });
    values += ({ testOb->format_stat_brief("luck") });
    results += ({ "lck" });
    values += ({ testOb->format_stat_brief("unknown") });
    results += ({ "" });
    values += ({ testOb->format_stat_brief("str") });
    results += ({ "str" });
    values += ({ testOb->format_stat_brief("per") });
    results += ({ "per" });
    values += ({ testOb->format_stat_brief("end") });
    results += ({ "end" });
    values += ({ testOb->format_stat_brief("cha") });
    results += ({ "cha" });
    values += ({ testOb->format_stat_brief("int") });
    results += ({ "int" });
    values += ({ testOb->format_stat_brief("agi") });
    results += ({ "agi" });
    values += ({ testOb->format_stat_brief("lck") });
    results += ({ "lck" });

    expect_arrays_equal(values, results, "format_stat_brief handled exits");

    expect_catches (({
        (: testOb->format_stat_brief(0) :),
        (: testOb->format_stat_brief(0.0) :),
        (: testOb->format_stat_brief(({})) :),
        (: testOb->format_stat_brief(([])) :),
        (: testOb->format_stat_brief((: 1 :)) :),
    }), "*Bad argument 1 to format->format_stat_brief\n", "format_stat_brief handled invalid argument 1");
}
void test_format_stat_verbose () {
    string *values = ({}), *results = ({});

    expect_function("format_stat_verbose", testOb);

    values += ({ testOb->format_stat_verbose("str") });
    results += ({ "strength" });
    values += ({ testOb->format_stat_verbose("per") });
    results += ({ "perception" });
    values += ({ testOb->format_stat_verbose("end") });
    results += ({ "endurance" });
    values += ({ testOb->format_stat_verbose("cha") });
    results += ({ "charisma" });
    values += ({ testOb->format_stat_verbose("int") });
    results += ({ "intelligence" });
    values += ({ testOb->format_stat_verbose("agi") });
    results += ({ "agility" });
    values += ({ testOb->format_stat_verbose("lck") });
    results += ({ "luck" });
    values += ({ testOb->format_stat_verbose("unknown") });
    results += ({ "" });
    values += ({ testOb->format_stat_verbose("strength") });
    results += ({ "strength" });
    values += ({ testOb->format_stat_verbose("perception") });
    results += ({ "perception" });
    values += ({ testOb->format_stat_verbose("endurance") });
    results += ({ "endurance" });
    values += ({ testOb->format_stat_verbose("charisma") });
    results += ({ "charisma" });
    values += ({ testOb->format_stat_verbose("intelligence") });
    results += ({ "intelligence" });
    values += ({ testOb->format_stat_verbose("agility") });
    results += ({ "agility" });
    values += ({ testOb->format_stat_verbose("luck") });
    results += ({ "luck" });

    expect_arrays_equal(values, results, "format_stat_verbose handled exits");

    expect_catches (({
        (: testOb->format_stat_verbose(0) :),
        (: testOb->format_stat_verbose(0.0) :),
        (: testOb->format_stat_verbose(({})) :),
        (: testOb->format_stat_verbose(([])) :),
        (: testOb->format_stat_verbose((: 1 :)) :),
    }), "*Bad argument 1 to format->format_stat_verbose\n", "format_stat_verbose handled invalid argument 1");
}

void test_format_integer () {
    string *values = ({}), *results = ({});

    expect_function("format_integer", testOb);

    // positives
    values += ({ testOb->format_integer(0) });
    results += ({ "0" });
    values += ({ testOb->format_integer(1) });
    results += ({ "1" });
    values += ({ testOb->format_integer(123) });
    results += ({ "123" });
    values += ({ testOb->format_integer(1234) });
    results += ({ "1,234" });
    values += ({ testOb->format_integer(123456) });
    results += ({ "123,456" });
    values += ({ testOb->format_integer(1234567) });
    results += ({ "1,234,567" });
    values += ({ testOb->format_integer(1234567890) });
    results += ({ "1,234,567,890" });

    // negatives
    values += ({ testOb->format_integer(-0) });
    results += ({ "0" });
    values += ({ testOb->format_integer(-1) });
    results += ({ "-1" });
    values += ({ testOb->format_integer(-1234) });
    results += ({ "-1,234" });
    values += ({ testOb->format_integer(-123456789) });
    results += ({ "-123,456,789" });
    values += ({ testOb->format_integer(-1234567890) });
    results += ({ "-1,234,567,890" });

    expect_arrays_equal(values, results, "format_integer handled integers");

    expect_catches (({
        (: testOb->format_integer("") :),
        (: testOb->format_integer(0.0) :),
        (: testOb->format_integer(({})) :),
        (: testOb->format_integer(([])) :),
        (: testOb->format_integer((: 1 :)) :),
    }), "*Bad argument 1 to format->format_integer\n", "format_integer handled invalid argument 1");
}