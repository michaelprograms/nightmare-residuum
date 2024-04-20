#include <access.h>

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

void test_query_file_privs () {
    expect("query_file_privs handles files", (: ({
        assert_equal(testOb->query_file_privs("/etc/welcome"), ACCESS_ALL),
        assert_equal(testOb->query_file_privs("/daemon/ansi.c"), ACCESS_MUDLIB),
        assert_equal(testOb->query_file_privs("/daemon/something.c"), ACCESS_MUDLIB),
        assert_equal(testOb->query_file_privs("/std/object.c"), ACCESS_ASSIST),
        assert_equal(testOb->query_file_privs("/std/module/save.c"), ACCESS_ASSIST),
        assert_equal(testOb->query_file_privs("/secure/daemon/access.c"), ACCESS_SECURE),
        assert_equal(testOb->query_file_privs("/secure/daemon/master.c"), ACCESS_SECURE),
        assert_equal(testOb->query_file_privs("/realm/user/workroom.c"), "user"),
        assert_equal(testOb->query_file_privs("/realm/user/header.h"), "user"),
        assert_equal(testOb->query_file_privs("/domain/Area/room/room1.c"), "Area"),
        assert_equal(testOb->query_file_privs("/domain/Area/include/area.h"), "Area"),
        assert_equal(testOb->query_file_privs("/domain/CAPITALIZED/file.c"), "Capitalized"),
        assert_equal(testOb->query_file_privs(""), 0),
        assert_equal(testOb->query_file_privs("/"), 0),
        assert_equal(testOb->query_file_privs("/nonexistantpath/file.c"), 0),
    }) :));
}

void test_unguarded () {
    expect("unguarded handles bad arguments", (: ({
        assert_catch((: testOb->unguarded((: MAX_INT :)) :), "*Bad previous_object to access->unguarded\n"),
        assert_catch((: testOb->unguarded(function () { return MAX_INT; }) :), "*Bad previous_object to access->unguarded\n"),
    }) :));
}

void test_query_allowed () {
    object basicOb;

    basicOb = new(STD_OBJECT);
    expect("query_allowed handles valid requests", (: ({
        // query read/write on path with permissions
        assert_equal(testOb->query_allowed(testOb, "file_size", "/tmp/path", "read"), 1),
        assert_equal(testOb->query_allowed(testOb, "write_file", "/tmp/path", "write"), 1),

        // query read/write on path without permissions
        assert_equal(testOb->query_allowed($(basicOb), "read_file", "/tmp/void/path", "read"), 0),
        assert_equal(testOb->query_allowed($(basicOb), "write_file", "/tmp/void/path", "write"), 0),
    }) :));
    destruct(basicOb);
}