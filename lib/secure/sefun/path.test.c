#define PATH_TEST_DIR "/save/test/testdir"

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/sefun/path.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

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
    expect_function("user_path", testOb);

    expect("user_path handles names", (: ({
        assert(testOb->user_path("username"), "==", "/realm/username"),
        assert(testOb->user_path("somebody"), "==", "/realm/somebody"),
    }) :));
    expect("user_path handles invalid inputs", (: ({
        assert(testOb->user_path(), "==", "/realm"),
        assert(testOb->user_path(0), "==", "/realm"),
        assert(testOb->user_path(""), "==", "/realm"),
    }) :));
}

void test_split_path () {
    expect_function("split_path", testOb);

    expect("split_path handles paths", (: ({
        assert(testOb->split_path("/domain/area"), "==", ({ "/domain/", "area" })),
        assert(testOb->split_path("/domain/area/"), "==", ({ "/domain/", "area" })),

        assert(testOb->split_path("/domain/area/dir"), "==", ({ "/domain/area/", "dir" })),
        assert(testOb->split_path("/domain/area/dir/"), "==", ({ "/domain/area/", "dir" })),
    }) :));
}

void test_base_path() {
    expect_function("base_path", testOb);

    expect("get_include_path handles paths", (: ({
        assert(testOb->base_path("/domain/area"), "==", "/domain/"),
        assert(testOb->base_path("/domain/area/"), "==", "/domain/"),
    }) :));
}

void test_sanitize_path () {
    expect_function("sanitize_path", testOb);

    expect("sanitize_path handles //", (: ({
        assert(testOb->sanitize_path("dir/"), "==", "/dir/"),
        assert(testOb->sanitize_path("/dir/"), "==", "/dir/"),
        assert(testOb->sanitize_path("//dir/"), "==", "/dir/"),
        assert(testOb->sanitize_path("/dir//"), "==", "/dir/"),
        assert(testOb->sanitize_path("//dir//"), "==", "/dir/"),
    }) :));

    expect("sanitize_path handles . and ..", (: ({
        assert((:testOb->sanitize_path(""):), "catch", "*Bad argument 1 to path->sanitize_path\n"),
        assert(testOb->sanitize_path("."), "==", "/"),
        assert(testOb->sanitize_path("/"), "==", "/"),
        assert(testOb->sanitize_path("/."), "==", "/"),
        assert(testOb->sanitize_path("/dir/.."), "==", "/"),
        assert(testOb->sanitize_path("/dir/../."), "==", "/"),
        assert(testOb->sanitize_path("/dir/dir/../.."), "==", "/"),
        assert(testOb->sanitize_path("/dir/dir/../../."), "==", "/"),
        assert(testOb->sanitize_path("/dir/dir/.././../."), "==", "/"),
        assert(testOb->sanitize_path("/dir/.././dir2/../."), "==", "/"),
    }) :));

    expect("sanitize_path handles ^", (: ({
        assert(testOb->sanitize_path("^"), "==", "/domain"),
        assert(testOb->sanitize_path("^."), "==", "/domain"),
        assert(testOb->sanitize_path("^/"), "==", "/domain/"),
        assert(testOb->sanitize_path("^/."), "==", "/domain/"),
        assert(testOb->sanitize_path("^/Dir/.."), "==", "/domain/"),
        assert(testOb->sanitize_path("^/Dir/../."), "==", "/domain/"),
        assert(testOb->sanitize_path("^/Dir/dir/../.."), "==", "/domain/"),
        assert(testOb->sanitize_path("^/Dir/dir/../../."), "==", "/domain/"),
        assert(testOb->sanitize_path("^/Dir/dir/.././../."), "==", "/domain/"),
        assert(testOb->sanitize_path("^/Dir/.././Dir2/../."), "==", "/domain/"),
        assert(testOb->sanitize_path("^Dir"), "==", "/domain/Dir"),
        assert(testOb->sanitize_path("^Dir/dir/.."), "==", "/domain/Dir/"),
    }) :));

    __MockCharacter = new("/std/module/id.c");
    __MockCharacter->set_key_name("test"); // must be named test
    __MockShell = new("/secure/shell/shell.c");
    __MockShell->start_shell();
    expect("sanitize_path handles ~", (: ({
        assert(testOb->sanitize_path("~"), "==", "/realm/test"),
        assert(testOb->sanitize_path("~."), "==", "/realm/test"),
        assert(testOb->sanitize_path("~/"), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/."), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/dir/.."), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/dir/../."), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/dir/dir/../.."), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/dir/dir/../../."), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/dir/dir/.././../."), "==", "/realm/test/"),
        assert(testOb->sanitize_path("~/dir/.././dir2/../."), "==", "/realm/test/"),
    }) :));

    expect("sanitize_path handles cwd", (: ({
        __MockShell->set_variable("cwd", "/realm/test/testdir/"),
        assert(testOb->sanitize_path("test"), "==", "/realm/test/testdir/"),
        assert((: testOb->sanitize_path("") :), "catch", "*Bad argument 1 to path->sanitize_path\n"),
        assert((: testOb->sanitize_path() :), "catch", "*Bad argument 1 to path->sanitize_path\n"),

        __MockShell->set_variable("cwd", "/realm/test/otherdir/"),
        assert(testOb->sanitize_path("test"), "==", "/realm/test/otherdir/"),
        assert((: testOb->sanitize_path("") :), "catch", "*Bad argument 1 to path->sanitize_path\n"),
        assert((: testOb->sanitize_path() :), "catch", "*Bad argument 1 to path->sanitize_path\n"),
    }) :));

    destruct(__MockCharacter);
    destruct(__MockShell);
}

void test_absolute_path () {
    expect_function("absolute_path", testOb);

    expect("absolute_path handles relative_to", (: ({
        assert(testOb->absolute_path("file.c", "/realm/username"), "==", "/realm/username/file.c"),
        assert(testOb->absolute_path("dir/file.c", "/realm/username"), "==", "/realm/username/dir/file.c"),
        assert(testOb->absolute_path("dir/file.c", this_object()), "==", "/secure/sefun/dir/file.c"),
    }) :));

    expect("absolute_path handles ^ alias for /domain", (: ({
        assert(testOb->absolute_path("^", "/"), "==", "/domain"),
        assert(testOb->absolute_path("^/", "/"), "==", "/domain/"),
        assert(testOb->absolute_path("^file.c", "/"), "==", "/domain/file.c"),
        assert(testOb->absolute_path("^/file.c", "/"), "==", "/domain/file.c"),
        assert(testOb->absolute_path("^dir/file.c", "/"), "==", "/domain/dir/file.c"),
    }) :));

    __MockCharacter = new("/std/module/id.c");
    __MockCharacter->set_key_name("test"); // must be named test
    __MockShell = new("/secure/shell/shell.c");
    __MockShell->start_shell();

    expect("absolute_path handles ~ alias for /realm", (: ({
        assert(testOb->absolute_path("~", "/"), "==", "/realm/test"),
        assert(testOb->absolute_path("~/", "/"), "==", "/realm/test/"),
        assert(testOb->absolute_path("~file.c", "/"), "==", "/realm/test/file.c"),
        assert(testOb->absolute_path("~/file.c", "/"), "==", "/realm/test/file.c"),
    }) :));

    destruct(__MockCharacter);
    destruct(__MockShell);
}

void test_assure_dir () {
    expect_function("assure_dir", testOb);

    expect("assure_dir creates dirs if missing", (: ({
        assert(testOb->assure_dir("/save/test"), "==", 1), // test should exist already

        rmdir(PATH_TEST_DIR),
        assert(file_size(PATH_TEST_DIR), "==", -1), // verify testdir doesn't exist

        assert(testOb->assure_dir(PATH_TEST_DIR), "==", 1), // testdir has been created
        assert(file_size(PATH_TEST_DIR), "==", -2), // verify testdir doesn't exist

        rmdir(PATH_TEST_DIR),
        assert(file_size(PATH_TEST_DIR), "==", -1), // verify testdir doesn't exist

    }) :));
}

void test_wild_card () {
    expect_function("wild_card", testOb);

    expect("wild_card matches paths", (: ({
        assert(wild_card(0, 0), "==", "({ })"),
        assert(wild_card("", ""), "==", "({ })"),

        assert(wild_card("/", "/"), "==", "({ \"/\" })"),

        assert(wild_card("/secure/sefun/path*.c", ""), "==", "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.test.c\" })"),
        assert(wild_card("/secure/sefun/path*.c", "/"), "==", "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.test.c\" })"),
        assert(wild_card("/secure/sefun/path*.c", "/domain"), "==", "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.test.c\" })"),
        assert(wild_card("../secure/sefun/path*.c", "/realm"), "==", "({ \"/secure/sefun/path.c\", \"/secure/sefun/path.test.c\" })"),
    }) :));
}