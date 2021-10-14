#include <access.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/daemon/access.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_file_privs () {
    int *values = ({}), *results = ({});

    values += ({ testOb->query_file_privs("/etc/welcome") });
    results += ({ ACCESS_ALL });

    values += ({ testOb->query_file_privs("/daemon/ansi.c") });
    results += ({ ACCESS_MUDLIB });
    values += ({ testOb->query_file_privs("/daemon/something.c") });
    results += ({ ACCESS_MUDLIB });

    values += ({ testOb->query_file_privs("/std/object/object.c") });
    results += ({ ACCESS_ASSIST });
    values += ({ testOb->query_file_privs("/std/module/save.c") });
    results += ({ ACCESS_ASSIST });

    values += ({ testOb->query_file_privs("/secure/daemon/access.c") });
    results += ({ ACCESS_SECURE });
    values += ({ testOb->query_file_privs("/secure/daemon/master.c") });
    results += ({ ACCESS_SECURE });

    values += ({ testOb->query_file_privs("/realm/user/workroom.c") });
    results += ({ "user" });
    values += ({ testOb->query_file_privs("/realm/user/header.h") });
    results += ({ "user" });

    values += ({ testOb->query_file_privs("/domain/Area/room/room1.c") });
    results += ({ "Area" });
    values += ({ testOb->query_file_privs("/domain/Area/include/area.h") });
    results += ({ "Area" });
    values += ({ testOb->query_file_privs("/domain/CAPITALIZED/file.c") });
    results += ({ "Capitalized" });

    values += ({ testOb->query_file_privs("") });
    results += ({ 0 });
    values += ({ testOb->query_file_privs("/") });
    results += ({ 0 });
    values += ({ testOb->query_file_privs("/nonexistantpath/file.c") });
    results += ({ 0 });

    expect_arrays_equal(values, results, "query_file_privs handled files");
}