inherit M_TEST;

/**
 * @var {"/secure/sefun/json"} testOb
 */

void test_json_encode () {
    expect("json_encode stringifies correctly", (: ({
        assert_equal(testOb->json_encode(), "null"),

        assert_equal(testOb->json_encode(123), "123"),
        assert_equal(testOb->json_encode(123.45), "123.450000"),

        assert_equal(testOb->json_encode("String"), "\"String\""),
        assert_equal(testOb->json_encode("\\"), "\"\\\\\""),
        assert_equal(testOb->json_encode("\\\""), "\"\\\\\\\"\""),
        assert_equal(testOb->json_encode("a\\b\"c"), "\"a\\\\b\\\"c\""),

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

        assert_equal(testOb->json_encode(({ ([ "a": 1 ]), ({ ([ "a": 1 ]) }) })), "[{\"a\":1},[{\"a\":1}]]"),

        assert_equal(testOb->json_encode(this_object()), "null"),
        assert_equal(testOb->json_encode(({ this_object() })), "[null]"),
        assert_equal(testOb->json_encode(({ this_object(), this_object() })), "[null,null]"),

        assert_equal(testOb->json_encode("🤔"), "\"🤔\""),
        assert_equal(testOb->json_encode("y̖̠͍̘͇͗̏̽̎͞"), "\"\u0079\u0316\u0320\u034D\u0318\u0347\u0357\u030F\u033D\u030E\u035E\""),
    }) :));
    expect("json_encode catches errors correctly", (: ({
        assert_catch((: testOb->json_encode(([ 123: 123, "key": 123 ])) :), "*Bad key type in json_encode: mapping keys must be strings\n"),
    }) :));
}

void test_json_decode () {
    expect("json_decode parses correctly", (: ({
        assert_equal(testOb->json_decode(), UNDEFINED),

        assert_equal(testOb->json_decode("\"test123\""), "test123"),

        assert_equal(testOb->json_decode("0"), 0),
        assert_equal(testOb->json_decode("123"), 123),
        assert_equal(testOb->json_decode("123.450000"), 123.45),
        assert_equal(testOb->json_decode("-123"), -123),
        assert_equal(testOb->json_decode("0e0"), 0.0),
        assert_equal(testOb->json_decode("0E12328"), 0.0),
        assert_equal(testOb->json_decode("123E+1"), 1230.0),
        assert_equal(testOb->json_decode("-1.5"), -1.5),

        assert_equal(testOb->json_decode("true"), 1),
        assert_equal(testOb->json_decode("false"), 0),
        assert_equal(testOb->json_decode("null"), 0),

        assert_equal(testOb->json_decode("[]"), ({ })),
        assert_equal(testOb->json_decode("[\"a\",\"b\",\"c\"]"), ({ "a", "b", "c" })),
        assert_equal(testOb->json_decode("[1,2,3]"), ({ 1, 2, 3 })),
        assert_equal(testOb->json_decode("[1\n,2\r,3]"), ({ 1, 2, 3 })),

        assert_equal(testOb->json_decode("{}"), ([ ])),
        assert_equal(testOb->json_decode(" {} "), ([ ])),
        assert_equal(testOb->json_decode("\n{}\n"), ([ ])),
        assert_equal(testOb->json_decode("{\n}"), ([ ])),
        assert_equal(testOb->json_decode("{\"a\":1,\"b\":2,\"c\":3}"), ([ "a": 1, "b": 2, "c": 3 ])),
        assert_equal(testOb->json_decode("{\"1\":\"a\",\"2\":\"b\",\"3\":\"c\"}"), ([ "1": "a", "2": "b", "3": "c" ])),

        assert_equal(testOb->json_decode("\"\\\\n\""), "\\n"),
        assert_equal(testOb->json_decode("\"\\\\t\""), "\\t"),
        assert_equal(testOb->json_decode("\"\\\\\\\\\""), "\\\\"),
        assert_equal(testOb->json_decode("\"hello\\\\nworld\""), "hello\\nworld"),

        assert_equal(testOb->json_decode("\"\\/\""), "/"),
        assert_equal(testOb->json_decode("\"\\ud83d\\ude04\""), "😄"),
        assert_equal(testOb->json_decode("\"🤔\""), "🤔"),
        assert_equal(testOb->json_decode("\"y̖̠͍̘͇͗̏̽̎͞\""), "\u0079\u0316\u0320\u034D\u0318\u0347\u0357\u030F\u033D\u030E\u035E"),

        assert_equal(testOb->json_decode("{ \"key1\": true,\r\t\"key2\": false, \"key3\": null }"), ([ "key1": 1, "key2": 0, "key3": 0, ])),
        assert_equal(testOb->json_decode("{ \"key1\" \n: true,\r\t\"key2\": false }"), ([ "key1": 1, "key2": 0, ])),
        assert_equal(testOb->json_decode("{\"k\":1\n}"), ([ "k": 1 ])),

        assert_equal(testOb->json_decode("\"\\b\""), "\b"),
        assert_equal(testOb->json_decode("\"\\f\""), "\x0c"),
        assert_equal(testOb->json_decode("\"\\r\""), "\r"),
        assert_equal(testOb->json_decode("\"\\u0041\""), "A"),
    }) :));

    expect("json_decode catches errors correctly", (: ({
        assert_catch((: testOb->json_decode("") :), "*Unexpected end of data in json_decode\n"),
        assert_catch((: testOb->json_decode("{ \"key\": [") :), "*Unexpected end of data in json_decode\n"),
        assert_catch((: testOb->json_decode("{}fail") :), "*Unexpected character in json_decode: f\n"),
    }) :));
    expect("json_decode_value catches errors correctly", (: ({
        assert_catch((: testOb->json_decode("fail{}") :), "*Unexpected character in json_decode_value: f\n"),
        assert_catch((: testOb->json_decode("{ \"key\": n_fail }") :), "*Unexpected character in json_decode_value: n\n"),
        assert_catch((: testOb->json_decode("{ \"key\": f_fail }") :), "*Unexpected character in json_decode_value: f\n"),
        assert_catch((: testOb->json_decode("{ \"key\": t_fail }") :), "*Unexpected character in json_decode_value: t\n"),
        assert_catch((: testOb->json_decode("}") :), "*Unexpected character in json_decode_value: }\n"),
        assert_catch((: testOb->json_decode("{ \"key\": [ }") :), "*Unexpected character in json_decode_value: }\n"),
    }) :));
    expect("json_decode_number catches errors correctly", (: ({
        assert_catch((: testOb->json_decode("0E123E123") :), "*Unexpected character in json_decode_number: E\n"),
        assert_catch((: testOb->json_decode("{ \"key\": 1.f }") :), "*Unexpected character in json_decode_number: f\n"),
        assert_catch((: testOb->json_decode("123E+1.0") :), "*Unexpected character in json_decode_number: .\n"),
        assert_catch((: testOb->json_decode("0f") :), "*Unexpected character in json_decode: f\n"),
        assert_catch((: testOb->json_decode("-") :), "*Unexpected end of data in json_decode_number\n"),
        assert_catch((: testOb->json_decode("-x") :), "*Unexpected character in json_decode_number: x\n"),
    }) :));
    expect("json_decode_object catches errors correctly", (: ({
        assert_catch((: testOb->json_decode("{") :), "*Unexpected end of data in json_decode_object\n"),
        assert_catch((: testOb->json_decode("{\n") :), "*Unexpected end of data in json_decode_object\n"),
        assert_catch((: testOb->json_decode("{ \"key\"") :), "*Unexpected end of data in json_decode_object\n"),
        assert_catch((: testOb->json_decode("{ \"key\"f") :), "*Unexpected character in json_decode_object: f\n"),
        assert_catch((: testOb->json_decode("{\"k\":1") :), "*Unexpected end of data in json_decode_object\n"),
        assert_catch((: testOb->json_decode("{\"k\":1 x}") :), "*Unexpected character in json_decode_object: x\n"),
    }) :));
    expect("json_decode_array catches errors correctly", (: ({
        assert_catch((: testOb->json_decode("{ \"key\": [ 1, 2, 3") :), "*Unexpected end of data in json_decode_array\n"),
        assert_catch((: testOb->json_decode("[1 x]") :), "*Unexpected character in json_decode_array: x\n"),
    }) :));
    expect("json_decode_string catches errors correctly", (: ({
        assert_catch((: testOb->json_decode("{fail}") :), "*Unexpected character in json_decode_string: f\n"),
        assert_catch((: testOb->json_decode("{ \"key\": \"") :), "*Unexpected end of data in json_decode_string\n"),
        assert_catch((: testOb->json_decode("\"\\uzzzz\"") :), "*Invalid hex digit in json_decode_string: 122\n"),
        assert_catch((: testOb->json_decode("\"\\ud800\"") :), "*Invalid string, missing surrogate pair in json_decode_string\n"),
        assert_catch((: testOb->json_decode("\"\\ud800\\uzzzz\"") :), "*Invalid hex digit in json_decode_string: 122\n"),
    }) :));
    expect("json_decode_string handles unknown escapes", (: ({
        assert_equal(testOb->json_decode("\"\\x\""), "\\x"),
    }) :));
}
