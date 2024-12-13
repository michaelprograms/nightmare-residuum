inherit M_TEST;

#define PATH_TEST_DIR "/save/test/testdir"

/**
 * @var {"/secure/sefun/path"} testOb
 */

object __MockCharacter;
object query_character () {
    return __MockCharacter;
}

mapping __MockVariable = ([ "cwd": "/" ]);
mapping query_variable (string key) {
    return __MockVariable[key];
}

void test_user_path () {
    expect("user_path handles names", (: ({
        assert_equal(testOb->user_path("username"), "/realm/username"),
        assert_equal(testOb->user_path("somebody"), "/realm/somebody"),
    }) :));
    expect("user_path handles invalid inputs", (: ({
        assert_equal(testOb->user_path(), "/realm"),
        assert_equal(testOb->user_path(0), "/realm"),
        assert_equal(testOb->user_path(""), "/realm"),
    }) :));
}

void test_split_path () {
    expect("split_path handles paths", (: ({
        assert_equal(testOb->split_path("/domain/area"), ({ "/domain/", "area" })),
        assert_equal(testOb->split_path("/domain/area/"), ({ "/domain/", "area" })),

        assert_equal(testOb->split_path("/domain/area/dir"), ({ "/domain/area/", "dir" })),
        assert_equal(testOb->split_path("/domain/area/dir/"), ({ "/domain/area/", "dir" })),

        assert_equal(testOb->split_path("/domain/area/file.c"), ({ "/domain/area/", "file.c" })),
        assert_equal(testOb->split_path("/domain/area/dir/file.txt"), ({ "/domain/area/dir/", "file.txt" })),
    }) :));
}

void test_sanitize_path () {
    expect("sanitize_path handles / and //", (: ({
        assert_equal(testOb->sanitize_path("dir/"), "/dir/"),
        assert_equal(testOb->sanitize_path("/dir/"), "/dir/"),
        assert_equal(testOb->sanitize_path("//dir/"), "/dir/"),
        assert_equal(testOb->sanitize_path("/dir//"), "/dir/"),
        assert_equal(testOb->sanitize_path("//dir//"), "/dir/"),
    }) :));

    expect("sanitize_path handles . and ..", (: ({
        assert_equal(testOb->sanitize_path("."), "/"),
        assert_equal(testOb->sanitize_path("/"), "/"),
        assert_equal(testOb->sanitize_path("/."), "/"),
        assert_equal(testOb->sanitize_path("/.././../."), "/"),
        assert_equal(testOb->sanitize_path("/dir/.."), "/"),
        assert_equal(testOb->sanitize_path("/dir/../."), "/"),
        assert_equal(testOb->sanitize_path("/dir/dir/../.."), "/"),
        assert_equal(testOb->sanitize_path("/dir/dir/../../."), "/"),
        assert_equal(testOb->sanitize_path("/dir/dir/.././../."), "/"),
        assert_equal(testOb->sanitize_path("/dir/.././dir2/../."), "/"),
    }) :));

    expect("sanitize_path handles ^", (: ({
        assert_equal(testOb->sanitize_path("^"), "/domain"),
        assert_equal(testOb->sanitize_path("^."), "/domain"),
        assert_equal(testOb->sanitize_path("^/"), "/domain/"),
        assert_equal(testOb->sanitize_path("^/."), "/domain/"),
        assert_equal(testOb->sanitize_path("^/Dir/.."), "/domain/"),
        assert_equal(testOb->sanitize_path("^/Dir/../."), "/domain/"),
        assert_equal(testOb->sanitize_path("^/Dir/dir/../.."), "/domain/"),
        assert_equal(testOb->sanitize_path("^/Dir/dir/../../."), "/domain/"),
        assert_equal(testOb->sanitize_path("^/Dir/dir/.././../."), "/domain/"),
        assert_equal(testOb->sanitize_path("^/Dir/.././Dir2/../."), "/domain/"),
        assert_equal(testOb->sanitize_path("^Dir"), "/domain/Dir"),
        assert_equal(testOb->sanitize_path("^Dir/dir/.."), "/domain/Dir/"),
    }) :));

    __MockCharacter = new("/std/object/id.c");
    __MockCharacter->set_key_name("test"); // must be named test
    expect("sanitize_path handles ~", (: ({
        assert_equal(testOb->sanitize_path("~"), "/realm/test"),
        assert_equal(testOb->sanitize_path("~."), "/realm/test"),
        assert_equal(testOb->sanitize_path("~/"), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/."), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/dir/.."), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/dir/../."), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/dir/dir/../.."), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/dir/dir/../../."), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/dir/dir/.././../."), "/realm/test/"),
        assert_equal(testOb->sanitize_path("~/dir/.././dir2/../."), "/realm/test/"),
    }) :));

    expect("sanitize_path handles cwd", (: ({
        __MockVariable["cwd"] = "/realm/test/testdir/",
        assert_equal(testOb->sanitize_path("test"), "/realm/test/testdir/test"),

        __MockVariable["cwd"] = "/realm/test/otherdir/",
        assert_equal(testOb->sanitize_path("test"), "/realm/test/otherdir/test"),
    }) :));

    destruct(__MockCharacter);

    expect("santitize_path handles invalid input", (: ({
        assert_catch((: testOb->sanitize_path("") :), "*Bad argument 1 to path->sanitize_path\n"),
    }) :));
}

void test_absolute_path () {
    expect("absolute_path handles relative_to", (: ({
        assert_equal(testOb->absolute_path("file.c", "/realm/username"), "/realm/username/file.c"),
        assert_equal(testOb->absolute_path("dir/file.c", "/realm/username"), "/realm/username/dir/file.c"),
        assert_equal(testOb->absolute_path("dir/file.c", this_object()), "/secure/sefun/dir/file.c"),
    }) :));
    expect("absolute_path handles invalid relative_to", (: ({
        assert_equal(testOb->absolute_path("file.c", 1.0), "/secure/sefun/file.c"),
        assert_equal(testOb->absolute_path("file.c", 123), "/secure/sefun/file.c"),
    }) :));
    expect("absolute_path defaults relative_to", (: ({
        assert_equal(testOb->absolute_path("file.c"), "/secure/sefun/file.c"),
    }) :));

    expect("absolute_path handles ^ alias for /domain", (: ({
        assert_equal(testOb->absolute_path("^", "/"), "/domain"),
        assert_equal(testOb->absolute_path("^/", "/"), "/domain/"),
        assert_equal(testOb->absolute_path("^file.c", "/"), "/domain/file.c"),
        assert_equal(testOb->absolute_path("^/file.c", "/"), "/domain/file.c"),
        assert_equal(testOb->absolute_path("^dir/file.c", "/"), "/domain/dir/file.c"),
    }) :));

    __MockCharacter = new("/std/object/id.c");
    __MockCharacter->set_key_name("test"); // must be named test

    expect("absolute_path handles ~ alias for /realm", (: ({
        assert_equal(testOb->absolute_path("~", "/"), "/realm/test"),
        assert_equal(testOb->absolute_path("~/", "/"), "/realm/test/"),
        assert_equal(testOb->absolute_path("~file.c", "/"), "/realm/test/file.c"),
        assert_equal(testOb->absolute_path("~/file.c", "/"), "/realm/test/file.c"),
    }) :));

    destruct(__MockCharacter);
}

void test_mkdirs () {
    expect("mkdirs creates dirs if missing", (: ({
        // no errors for empty string
        assert_equal(testOb->mkdirs(""), 0),

        // test should exist or create
        assert_equal(testOb->mkdirs("/save/test"), 1),
        // test should exist
        assert_equal(testOb->mkdirs("/save/test"), 1),

        rmdir(PATH_TEST_DIR),
        // verify testdir doesn't exist
        assert_equal(file_size(PATH_TEST_DIR), -1),

        assert_equal(testOb->mkdirs(PATH_TEST_DIR), 1),
        // verify testdir exists
        assert_equal(file_size(PATH_TEST_DIR), -2),

        rmdir(PATH_TEST_DIR),
        // verify testdir doesn't exist
        assert_equal(file_size(PATH_TEST_DIR), -1),

        assert_equal(write_file(PATH_TEST_DIR + ".txt", "Test file."), 1),
        // verify can't create when file exists in path
        assert_equal(testOb->mkdirs(PATH_TEST_DIR + ".txt/test"), 0),
        assert_equal(rm(PATH_TEST_DIR + ".txt"), 1),
    }) :));
}

void test_wild_card () {
    expect("wild_card matches paths", (: ({
        assert_equal(testOb->wild_card(0), ({ })),
        assert_equal(testOb->wild_card(""), ({ })),

        assert_equal(testOb->wild_card("/"), ({ "/cmd", "/daemon", "/doc", "/domain", "/etc", "/include", "/log", "/realm", "/save", "/secure", "/std", "/tmp"  })),
        assert_equal(testOb->wild_card("/r*"), ({ "/realm" })),
        assert_equal(testOb->wild_card("/d*"), ({ "/daemon", "/doc", "/domain" })),

        assert_equal(testOb->wild_card("/doesntexist"), ({ })),
        assert_equal(testOb->wild_card("/doesntexist/*"), ({ })),
        assert_equal(testOb->wild_card("/doesntexist*"), ({ })),

        assert_equal(testOb->wild_card("/doc"), ({ "/doc" })),
        assert_equal(testOb->wild_card("/doc/"), ({ "/doc/apply", "/doc/build", "/doc/efun", "/doc/help", "/doc/lpc" })),
        assert_equal(testOb->wild_card("/doc/*"), ({ "/doc/apply", "/doc/build", "/doc/efun", "/doc/help", "/doc/lpc" })),

        assert_equal(testOb->wild_card("/secure/sefun/path*.c"), ({ "/secure/sefun/path.c", "/secure/sefun/path.coverage.c", "/secure/sefun/path.test.c" })),
        assert_equal(testOb->wild_card("/secure/sefun/path*.c"), ({ "/secure/sefun/path.c", "/secure/sefun/path.coverage.c", "/secure/sefun/path.test.c" })),
        assert_equal(testOb->wild_card("/secure/sefun/path*.c"), ({ "/secure/sefun/path.c", "/secure/sefun/path.coverage.c", "/secure/sefun/path.test.c" })),
        assert_equal(testOb->wild_card("../secure/sefun/path*.c"), ({ "/secure/sefun/path.c", "/secure/sefun/path.coverage.c", "/secure/sefun/path.test.c" })),
    }) :));
}

void test_query_file_recursive () {
    expect("query_file_recursive finds files recursively", (: ({
        assert_equal(testOb->query_file_recursive("/secure/sefun/path.test.c", "path"), "/secure/sefun/path.c"),

        assert_equal(testOb->query_file_recursive("/secure/sefun/path.test.c", "nonexistant"), 0),
    }) :));
}