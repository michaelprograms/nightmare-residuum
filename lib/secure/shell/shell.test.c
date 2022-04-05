inherit M_TEST;

private int action = 0;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/shell/shell.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_parse_command_flags () {
    expect_function("parse_command_flags", testOb);

    expect("parse_command_flags parses valid flags", (: ({
        // check no flags
        assert((: testOb->parse_command_flags("something") :), "==", ({ "something", ([ ]) })),
        assert((: testOb->parse_command_flags() :), "==", ({ UNDEFINED, ([ ]) })),
        assert((: testOb->parse_command_flags(0) :), "==", ({ 0, ([ ]) })),

        // check flags at front
        assert((: testOb->parse_command_flags("-a something") :), "==", ({ "something", ([ "a": 1, ]) })),
        assert((: testOb->parse_command_flags("-abc something") :), "==", ({ "something", ([ "abc": 1, ]) })),
        assert((: testOb->parse_command_flags("-a -b -c something") :), "==", ({ "something", ([ "a": 1, "b": 1, "c": 1, ]) })),
        assert((: testOb->parse_command_flags("-a=1 -b=2 -c=3 something") :), "==", ({ "something", ([ "a": "1", "b": "2", "c": "3", ]) })),
        assert((: testOb->parse_command_flags("-a=x -b=y -c=z something") :), "==", ({ "something", ([ "a": "x", "b": "y", "c": "z", ]) })),
        assert((: testOb->parse_command_flags("-path=/path/with/special_characters-in.it something else") :), "==", ({ "something else", ([ "path": "/path/with/special_characters-in.it", ]) })),
    }) :));

    expect("parse_command_flags parses invalid flags", (: ({
        assert((: testOb->parse_command_flags("- something") :), "==", ({ "something", ([ ]) })),
        assert((: testOb->parse_command_flags("something -") :), "==", ({ "something -", ([ ]) })),
        assert((: testOb->parse_command_flags("something -invalidflag") :), "==", ({ "something -invalidflag", ([ ]) })),
        assert((: testOb->parse_command_flags("x y z -a") :), "==", ({ "x y z -a", ([ ]) })),
        assert((: testOb->parse_command_flags("x y z -abc") :), "==", ({ "x y z -abc", ([ ]) })),
    }) :));
}