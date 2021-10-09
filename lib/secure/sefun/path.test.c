inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/path");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

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
}
void test_absolute_path () {
    expect_function("absolute_path", testOb);
    expect_strings_equal(testOb->absolute_path("file.c", "/realm/username"), "/realm/username/file.c", "absolute_path handled realm file");
    expect_strings_equal(testOb->absolute_path("dir/file.c", "/realm/username"), "/realm/username/dir/file.c", "absolute_path handled realm dir/file");
    expect_strings_equal(testOb->absolute_path("dir/file.c", this_object()), "/secure/sefun/dir/file.c", "absolute_path handled relative_to dir/file");
    // @TODO
    // expect_strings_equal(testOb->absolute_path("^", "/domain"), "/realm/username", "absolute_path handled ^");
    // expect_strings_equal(testOb->absolute_path("^/", "/domain"), "/realm/username", "absolute_path handled ^/");
    // expect_strings_equal(testOb->absolute_path("^file.c", "/domain"), "/realm/username/file.c", "absolute_path handled ^file");
    // expect_strings_equal(testOb->absolute_path("~", "/realm"), "/domain", "absolute_path handled ^");
    // expect_strings_equal(testOb->absolute_path("~file.c", "/realm"), "/domain/file.c", "absolute_path handled ~file");
    // expect_strings_equal(testOb->absolute_path("~dir/file.c", "/realm"), "/domain/dir/file.c", "absolute_path handled ~dir/file");
}