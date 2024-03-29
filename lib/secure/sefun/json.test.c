inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/json.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_remove_article () {
    expect_function("json_encode", testOb);

    expect("json_encode stringifies correctly", (: ({
        assert_equal(testOb->json_encode(), "null"),

        assert_equal(testOb->json_encode(123), "123"),
        assert_equal(testOb->json_encode(123.45), "123.450000"),

        assert_equal(testOb->json_encode("String"), "\"String\""),

        assert_equal(testOb->json_encode(({ 1, 2, 3, "a", "b", "c" })), "[1,2,3,\"a\",\"b\",\"c\"]"),

        assert_equal(testOb->json_encode(([ "key1": "value1", "keyA": "valueA", ])), "{\"keyA\":\"valueA\",\"key1\":\"value1\"}"),
        assert_equal(testOb->json_encode(([ "key1": this_object() ])), "{\"key1\":null}"),
        assert_equal(testOb->json_encode(([ "key1": 123, "key2": 1.23 ])), "{\"key2\":1.230000,\"key1\":123}"),

        assert_equal(testOb->json_encode(this_object()), "null"),
        assert_equal(testOb->json_encode(({ this_object() })), "[null]"),
    }) :));
}