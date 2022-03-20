#include <access.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/daemon/access.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_file_privs () {
    expect_function("query_file_privs", testOb);

    expect("query_file_privs handled files", (: ({
        assert((: testOb->query_file_privs("/etc/welcome") :), "==", ACCESS_ALL),
        assert((: testOb->query_file_privs("/daemon/ansi.c") :), "==", ACCESS_MUDLIB),
        assert((: testOb->query_file_privs("/daemon/something.c") :), "==", ACCESS_MUDLIB),
        assert((: testOb->query_file_privs("/std/object.c") :), "==", ACCESS_ASSIST),
        assert((: testOb->query_file_privs("/std/module/save.c") :), "==", ACCESS_ASSIST),
        assert((: testOb->query_file_privs("/secure/daemon/access.c") :), "==", ACCESS_SECURE),
        assert((: testOb->query_file_privs("/secure/daemon/master.c") :), "==", ACCESS_SECURE),
        assert((: testOb->query_file_privs("/realm/user/workroom.c") :), "==", "user"),
        assert((: testOb->query_file_privs("/realm/user/header.h") :), "==", "user"),
        assert((: testOb->query_file_privs("/domain/Area/room/room1.c") :), "==", "Area"),
        assert((: testOb->query_file_privs("/domain/Area/include/area.h") :), "==", "Area"),
        assert((: testOb->query_file_privs("/domain/CAPITALIZED/file.c") :), "==", "Capitalized"),
        assert((: testOb->query_file_privs("") :), "==", 0),
        assert((: testOb->query_file_privs("/") :), "==", 0),
        assert((: testOb->query_file_privs("/nonexistantpath/file.c") :), "==", 0),
    }) :));
}

void test_unguarded () {
    expect_function("unguarded", testOb);

    expect("unguarded handled bad arguments", (: ({
        assert((: testOb->unguarded((: MAX_INT :)) :), "catch", "*Illegal unguarded.\n"),
        assert((: testOb->unguarded(function () { return MAX_INT; }) :), "catch", "*Illegal unguarded.\n"),
        assert((: testOb->unguarded() :), "catch", "*Illegal unguarded.\n"),
        assert((: testOb->unguarded("bad argument") :), "catch", "*Illegal unguarded.\n"),
    }) :));
}