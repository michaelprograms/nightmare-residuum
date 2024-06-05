inherit M_TEST;

void test_json_encode () {
    mapping m = ([ "a": 1 ]);
    mixed *arr = ({ m, ({ m }) });

    expect("json_encode stringifies correctly", (: ({
        assert_equal(testOb->json_encode(), "null"),

        assert_equal(testOb->json_encode(123), "123"),
        assert_equal(testOb->json_encode(123.45), "123.450000"),

        assert_equal(testOb->json_encode("String"), "\"String\""),
        assert_equal(testOb->json_encode("\\"), "\"\\\\\""),
        assert_equal(testOb->json_encode("\\\""), "\"\\\"\""),

        assert_equal(testOb->json_encode("\b"), "\"\\b\""),
        assert_equal(testOb->json_encode("\n"), "\"\\n\""),
        assert_equal(testOb->json_encode("\r"), "\"\\r\""),
        assert_equal(testOb->json_encode("\t"), "\"\\t\""),
        assert_equal(testOb->json_encode("\x0c"), "\"\\f\""),
        assert_equal(testOb->json_encode("\x1b"), "\"\\u001b\""),

        assert_equal(testOb->json_encode(({ 1, 2, 3, "a", "b", "c" })), "[1,2,3,\"a\",\"b\",\"c\"]"),
        assert_equal(testOb->json_encode(({ })), "[]"),

        assert_equal(testOb->json_encode(([ "key1": "value1", "keyA": "valueA", ])), "{\"keyA\":\"valueA\",\"key1\":\"value1\"}"),
        assert_equal(testOb->json_encode(([ "key1": this_object() ])), "{\"key1\":null}"),
        assert_equal(testOb->json_encode(([ "key1": 123, "key2": 1.23 ])), "{\"key2\":1.230000,\"key1\":123}"),

        assert_equal(testOb->json_encode(([ 123: 123, "key": 123 ])), "{\"key\":123}"),

        assert_equal(testOb->json_encode(({ ([ "a": 1 ]), ({ ([ "a": 1 ]) }) })), "[{\"a\":1},[{\"a\":1}]]"),

        assert_equal(testOb->json_encode(this_object()), "null"),
        assert_equal(testOb->json_encode(({ this_object() })), "[null]"),
        assert_equal(testOb->json_encode(({ this_object(), this_object() })), "[null,null]"),
    }) :));
}