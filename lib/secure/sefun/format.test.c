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