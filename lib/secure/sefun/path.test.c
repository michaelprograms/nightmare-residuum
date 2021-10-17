#define PATH_TEST_DIR "/save/test/testdir"

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/sefun/path");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_ignore () { return ::test_ignore() + ({ "query_character", "query_shell" }); }

// -----------------------------------------------------------------------------

object mockCharacter;
object query_character () {
    return mockCharacter;
}

object mockShell;
object query_shell () {
    return mockShell;
}

// -----------------------------------------------------------------------------

void test_user_path () {
    expect_function("user_path", testOb);

    expect_strings_equal(testOb->user_path("username"), "/realm/username", "user_path(username) returned /realm/username");

    expect_array_strings_equal(({
        testOb->user_path(0),
        testOb->user_path("")
    }), "/realm", "user_path(invalid) returned /realm");
}

void test_split_path () {
    expect_function("split_path", testOb);

    expect_arrays_array_equal(({
        testOb->split_path("/domain/area"),
        testOb->split_path("/domain/area/"),
    }), ({ "/domain/", "area" }), "get_include_path handled path");
    expect_arrays_array_equal(({
        testOb->split_path("/domain/area/dir"),
        testOb->split_path("/domain/area/dir/"),
     }), ({ "/domain/", "area/", "dir" }), "get_include_path handled subdir");
}

void test_base_path() {
    expect_function("base_path", testOb);

    expect_array_strings_equal(({
        testOb->base_path("/domain/area"),
        testOb->base_path("/domain/area/"),
    }), "/domain/", "get_include_path handled path");
}

void test_sanitize_path () {
    expect_function("sanitize_path", testOb);

    expect_array_strings_equal(({
        testOb->sanitize_path("dir/"),
        testOb->sanitize_path("/dir/"),
        testOb->sanitize_path("//dir/"),
        testOb->sanitize_path("/dir//"),
        testOb->sanitize_path("//dir//"),
    }), "/dir/", "sanitize_path handled //");

    expect_array_strings_equal(({
        testOb->sanitize_path(""),
        testOb->sanitize_path("."),
        testOb->sanitize_path("/"),
        testOb->sanitize_path("/."),
        testOb->sanitize_path("/dir/.."),
        testOb->sanitize_path("/dir/../."),
        testOb->sanitize_path("/dir/dir/../.."),
        testOb->sanitize_path("/dir/dir/../../."),
        testOb->sanitize_path("/dir/dir/.././../."),
        testOb->sanitize_path("/dir/.././dir2/../."),
    }), "/", "sanitize_path handled . and ..");

    expect_arrays_equal(({
        testOb->sanitize_path("^"),
        testOb->sanitize_path("^."),
        testOb->sanitize_path("^/"),
        testOb->sanitize_path("^/."),
        testOb->sanitize_path("^/Dir/.."),
        testOb->sanitize_path("^/Dir/../."),
        testOb->sanitize_path("^/Dir/dir/../.."),
        testOb->sanitize_path("^/Dir/dir/../../."),
        testOb->sanitize_path("^/Dir/dir/.././../."),
        testOb->sanitize_path("^/Dir/.././Dir2/../."),
        testOb->sanitize_path("^Dir"),
        testOb->sanitize_path("^Dir/dir/.."),
    }), ({
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/",
        "/domain/Dir/",
        "/domain/Dir/",
    }), "sanitize_path handled ^");

    mockCharacter = new("/std/object/id.c");
    mockCharacter->set_key_name("tester");
    mockShell = new("/secure/shell/shell.c");
    mockShell->start_shell();
    expect_array_strings_equal(({
        testOb->sanitize_path("~"),
        testOb->sanitize_path("~."),
        testOb->sanitize_path("~/"),
        testOb->sanitize_path("~/."),
        testOb->sanitize_path("~/dir/.."),
        testOb->sanitize_path("~/dir/../."),
        testOb->sanitize_path("~/dir/dir/../.."),
        testOb->sanitize_path("~/dir/dir/../../."),
        testOb->sanitize_path("~/dir/dir/.././../."),
        testOb->sanitize_path("~/dir/.././dir2/../."),
    }), "/realm/tester/", "sanitize_path handled ~");

    mockShell->set_variable("cwd", "/realm/tester/testdir/");
    expect_array_strings_equal(({
        testOb->sanitize_path(""),
        testOb->sanitize_path(),
        testOb->sanitize_path("test"),
    }), "/realm/tester/testdir/", "sanitize_path handled cwd");
}

void test_absolute_path () {
    expect_function("absolute_path", testOb);

    expect_strings_equal(testOb->absolute_path("file.c", "/realm/username"), "/realm/username/file.c", "absolute_path handled realm file");
    expect_strings_equal(testOb->absolute_path("dir/file.c", "/realm/username"), "/realm/username/dir/file.c", "absolute_path handled realm dir/file");
    expect_strings_equal(testOb->absolute_path("dir/file.c", this_object()), "/secure/sefun/dir/file.c", "absolute_path handled relative_to dir/file");

    // @TODO
    // expect_strings_equal(testOb->absolute_path("^", "/"), "/realm/username", "absolute_path handled ^");
    // expect_strings_equal(testOb->absolute_path("^/", "/"), "/realm/username", "absolute_path handled ^/");
    // expect_strings_equal(testOb->absolute_path("^file.c", "/"), "/realm/username/file.c", "absolute_path handled ^file");
    // expect_strings_equal(testOb->absolute_path("~", "/"), "/domain", "absolute_path handled ^");
    // expect_strings_equal(testOb->absolute_path("~file.c", "/"), "/domain/file.c", "absolute_path handled ~file");
    // expect_strings_equal(testOb->absolute_path("~dir/file.c", "/"), "/domain/dir/file.c", "absolute_path handled ~dir/file");
}

void test_assure_dir () {
    int *values = ({}), *results = ({});

    expect_function("assure_dir", testOb);

    values += ({ testOb->assure_dir("/save/test") });
    results += ({ 1 }); // test should exist already

    unguarded((: rmdir, PATH_TEST_DIR :));
    values += ({ file_size(PATH_TEST_DIR) });
    results += ({ -1 }); // verify testdir doesn't exist

    values += ({ testOb->assure_dir(PATH_TEST_DIR) });
    results += ({ 1 }); // testdir has been created
    values += ({ file_size(PATH_TEST_DIR) });
    results += ({ -2 }); // verify testdir doesn't exist

    unguarded((: rmdir, PATH_TEST_DIR :));
    values += ({ file_size(PATH_TEST_DIR) });
    results += ({ -1 }); // verify testdir doesn't exist

    expect_arrays_equal(values, results, "assure_dir handled dirs");
}
