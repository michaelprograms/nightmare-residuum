#define PATH_TEST_DIR "/save/test/testdir"

inherit M_TEST;

// -----------------------------------------------------------------------------

object __MockCharacter;
object query_character () {
    return __MockCharacter;
}

object __MockShell;
object query_shell () {
    return __MockShell;
}

// -----------------------------------------------------------------------------

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
    }) :));
}

void test_base_path() {
    expect("get_include_path handles paths", (: ({
        assert_equal(testOb->base_path("/domain/area"), "/domain/"),
        assert_equal(testOb->base_path("/domain/area/"), "/domain/"),
    }) :));
}

void test_sanitize_path () {
    expect("sanitize_path handles //", (: ({
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

    __MockCharacter = new("/std/module/id.c");
    __MockCharacter->set_key_name("test"); // must be named test
    __MockShell = new("/secure/shell/shell.c");
    __MockShell->start_shell();
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
        __MockShell->set_variable("cwd", "/realm/test/testdir/"),
        assert_equal(testOb->sanitize_path("test"), "/realm/test/testdir/"),

        __MockShell->set_variable("cwd", "/realm/test/otherdir/"),
        assert_equal(testOb->sanitize_path("test"), "/realm/test/otherdir/"),
    }) :));

    destruct(__MockCharacter);
    destruct(__MockShell);
}

void test_absolute_path () {
    expect("absolute_path handles relative_to", (: ({
        assert_equal(testOb->absolute_path("file.c", "/realm/username"), "/realm/username/file.c"),
        assert_equal(testOb->absolute_path("dir/file.c", "/realm/username"), "/realm/username/dir/file.c"),
        assert_equal(testOb->absolute_path("dir/file.c", this_object()), "/secure/sefun/dir/file.c"),
    }) :));

    expect("absolute_path handles ^ alias for /domain", (: ({
        assert_equal(testOb->absolute_path("^", "/"), "/domain"),
        assert_equal(testOb->absolute_path("^/", "/"), "/domain/"),
        assert_equal(testOb->absolute_path("^file.c", "/"), "/domain/file.c"),
        assert_equal(testOb->absolute_path("^/file.c", "/"), "/domain/file.c"),
        assert_equal(testOb->absolute_path("^dir/file.c", "/"), "/domain/dir/file.c"),
    }) :));

    __MockCharacter = new("/std/module/id.c");
    __MockCharacter->set_key_name("test"); // must be named test
    __MockShell = new("/secure/shell/shell.c");
    __MockShell->start_shell();

    expect("absolute_path handles ~ alias for /realm", (: ({
        assert_equal(testOb->absolute_path("~", "/"), "/realm/test"),
        assert_equal(testOb->absolute_path("~/", "/"), "/realm/test/"),
        assert_equal(testOb->absolute_path("~file.c", "/"), "/realm/test/file.c"),
        assert_equal(testOb->absolute_path("~/file.c", "/"), "/realm/test/file.c"),
    }) :));

    destruct(__MockCharacter);
    destruct(__MockShell);
}

void test_mkdirs () {
    expect("mkdirs creates dirs if missing", (: ({
        assert_equal(testOb->mkDirs(""), 0), // no errors for empty string

        assert_equal(testOb->mkdirs("/save/test"), 1), // test should exist already

        rmdir(PATH_TEST_DIR),
        assert_equal(file_size(PATH_TEST_DIR), -1), // verify testdir doesn't exist

        assert_equal(testOb->mkdirs(PATH_TEST_DIR), 1), // testdir has been created
        assert_equal(file_size(PATH_TEST_DIR), -2), // verify testdir exists

        rmdir(PATH_TEST_DIR),
        assert_equal(file_size(PATH_TEST_DIR), -1), // verify testdir doesn't exist

    }) :));
}

void test_wild_card () {
    expect("wild_card matches paths", (: ({
        assert_equal(wild_card(0, 0), "({ })"),
        assert_equal(wild_card("", ""), "({ })"),

        assert_equal(wild_card("/", "/"), "({ \"/\" })"),

        assert_equal(wild_card("/secure/sefun/path*.c", ""), "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.coverage.c\", \"/secure/sefun/path.test.c\" })"),
        assert_equal(wild_card("/secure/sefun/path*.c", "/"), "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.coverage.c\", \"/secure/sefun/path.test.c\" })"),
        assert_equal(wild_card("/secure/sefun/path*.c", "/domain"), "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.coverage.c\", \"/secure/sefun/path.test.c\" })"),
        assert_equal(wild_card("../secure/sefun/path*.c", "/realm"), "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.coverage.c\", \"/secure/sefun/path.test.c\" })"),
    }) :));
}