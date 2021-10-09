inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/sefun");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_driver_version () {
    expect_function("driver_version", testOb);
    expect_strings_equal(testOb->driver_version(), "fluffos v2019", "driver_version matched expected");
}
void test_mudlib_version () {
    expect_function("mudlib_version", testOb);
    expect_strings_equal(testOb->mudlib_version(), "NR v0.1", "mudlib_version matched expected");
}
void test_mud_name () {
    expect_function("mud_name", testOb);
    expect_strings_equal(testOb->mud_name(), "Nightmare Residuum", "mud_name matched expected");
}
void test_driver_port () {
    expect_function("driver_port", testOb);
    expect_strings_equal(""+testOb->driver_port(), "6666", "driver_port matched expected");
}