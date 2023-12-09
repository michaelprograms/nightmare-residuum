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
        assert(testOb->json_encode(), "==", "null"),

        assert(testOb->json_encode(123), "==", "123"),
        assert(testOb->json_encode(123.45), "==", "123.450000"),

        assert(testOb->json_encode("String"), "==", "\"String\""),

        assert(testOb->json_encode(({ 1, 2, 3, "a", "b", "c" })), "==", "[1,2,3,\"a\",\"b\",\"c\"]"),

        assert(testOb->json_encode(([ "key1": "value1", "keyA": "valueA", ])), "==", "{\"keyA\":\"valueA\",\"key1\":\"value1\"}"),

        assert(testOb->json_encode(this_object()), "==", "null"),
        assert(testOb->json_encode(({ this_object() })), "==", "[null]"),
    }) :));
}