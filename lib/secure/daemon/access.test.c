#include <access.h>

inherit M_TEST;

/**
 * @var {"/secure/daemon/access"} testOb
 */

void test_query_file_privs () {
    expect("query_file_privs returns correct priv for each path class", (: ({
        assert_equal(testOb->query_file_privs("/etc/welcome"), ACCESS_ALL),
        assert_equal(testOb->query_file_privs("/cmd/character/look.c"), ACCESS_CMD),
        assert_equal(testOb->query_file_privs("/daemon/ansi.c"), ACCESS_MUDLIB),
        assert_equal(testOb->query_file_privs("/daemon/something.c"), ACCESS_MUDLIB),
        assert_equal(testOb->query_file_privs("/std/object.c"), ACCESS_ASSIST),
        assert_equal(testOb->query_file_privs("/std/module/save.c"), ACCESS_ASSIST),
        assert_equal(testOb->query_file_privs("/save/account/test"), ACCESS_SECURE),
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

void test_query_allowed_reads () {
    object basicOb = new(STD_OBJECT);

    expect("query_allowed allows reads from ALL paths for any caller", (: ({
        // read.cfg marks /tmp/ as ALL — any caller is permitted
        assert_equal(testOb->query_allowed(testOb, "file_size", "/tmp/path", "read"), 1),
        assert_equal(testOb->query_allowed($(basicOb), "file_size", "/tmp/path", "read"), 1),
        // read.cfg marks /secure/ as ALL — any caller is permitted to read
        assert_equal(testOb->query_allowed($(basicOb), "read_file", "/secure/daemon/access.c", "read"), 1),
    }) :));

    expect("query_allowed denies reads from NONE paths for all callers", (: ({
        // read.cfg marks /tmp/void/ as NONE — no caller is permitted
        assert_equal(testOb->query_allowed($(basicOb), "read_file", "/tmp/void/path", "read"), 0),
        assert_equal(testOb->query_allowed(testOb, "read_file", "/tmp/void/path", "read"), 0),
    }) :));

    destruct(basicOb);
}

void test_query_allowed_writes () {
    object basicOb = new(STD_OBJECT);

    expect("query_allowed allows writes to ALL paths for any caller", (: ({
        // write.cfg marks /tmp/ as ALL — any caller is permitted
        assert_equal(testOb->query_allowed(testOb, "write_file", "/tmp/path", "write"), 1),
        assert_equal(testOb->query_allowed($(basicOb), "write_file", "/tmp/path", "write"), 1),
        // write.cfg marks /save/test/ as ALL — any caller is permitted
        assert_equal(testOb->query_allowed($(basicOb), "write_file", "/save/test/x", "write"), 1),
    }) :));

    expect("query_allowed denies writes to NONE paths for all callers", (: ({
        // write.cfg marks /tmp/void/ as NONE — no caller is permitted
        assert_equal(testOb->query_allowed($(basicOb), "write_file", "/tmp/void/path", "write"), 0),
        assert_equal(testOb->query_allowed(testOb, "write_file", "/tmp/void/path", "write"), 0),
    }) :));

    expect("query_allowed enforces write.cfg privilege restrictions", (: ({
        // write.cfg restricts /save/ to SECURE:MUDLIB
        // ASSIST caller (basicOb = STD_OBJECT from /std/) is denied
        assert_equal(testOb->query_allowed($(basicOb), "write_file", "/save/account/x", "write"), 0),
        // SECURE caller (testOb = D_ACCESS from /secure/) is permitted
        // Note: previous_object(-1) during tests includes the test file (/secure/ = SECURE)
        // and D_TEST (/secure/ = SECURE), so the full stack clears for SECURE paths
        assert_equal(testOb->query_allowed(testOb, "write_file", "/save/account/x", "write"), 1),
    }) :));

    destruct(basicOb);
}

void test_query_allowed_socket () {
    expect("query_allowed denies socket access to non-IPC callers", (: ({
        // testOb is D_ACCESS, not D_IPC, and does not inherit HTTP module
        assert_equal(testOb->query_allowed(testOb, "socket_create", 0, "socket"), 0),
    }) :));
}

void test_query_allowed_invalid_args () {
    expect("query_allowed rejects invalid arguments", (: ({
        assert_catch((: testOb->query_allowed(0, "file_size", "/tmp/path", "read") :),
            "*Bad argument 1 to access->query_allowed\n"),
        assert_catch((: testOb->query_allowed(testOb, 0, "/tmp/path", "read") :),
            "*Bad argument 2 to access->query_allowed\n"),
        assert_catch((: testOb->query_allowed(testOb, "file_size", 0, "read") :),
            "*Bad argument 3 to access->query_allowed\n"),
        assert_catch((: testOb->query_allowed(testOb, "file_size", "/tmp/path", 0) :),
            "*Bad argument 4 to access->query_allowed\n"),
    }) :));
}