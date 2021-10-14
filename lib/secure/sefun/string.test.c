inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/string");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

nosave private object __MockAccount;
object query_account () {
    return __MockAccount;
}


void test_strip_colour () {
    string text = "%^BOLD%^Text%^RESET%^";
    expect_function("strip_colour", testOb);
    expect_strings_equal(testOb->strip_colour(text), "Text", "strip_colour removed ANSI tags");
    expect_true(strlen(text) != 4 && strlen(testOb->strip_colour(text)) == 4, "strip_colour has correct strlen");
    expect_true(strlen(testOb->strip_colour("%^RESET%^%^RESET%^RESET%^")) == 0, "strip_colour handled fluffos ANSI resets");
}

void test_center () {
    string text = "Text";
    string centered12 = "    Text    ";
    string centered80 = sprintf("%38s", " ") + "Text" + sprintf("%38s", " ");
    string undefStr;

    expect_function("center", testOb);

    expect_strings_equal(testOb->center(""), "", "center handled empty string");
    expect_strings_equal(testOb->center(undefStr), "", "center handled undefined string");
    expect_array_strings_equal(({
        testOb->center(text),
        testOb->center(text, 80)
    }), centered80, "center handled width 80");
    expect_strings_equal(testOb->center(text, 12), centered12, "center handled width 12");
    expect_array_strings_equal(({
        testOb->center(text, 2),
        testOb->center(text, -20)
    }), text, "center handled width < strlen");
}

void test_pad_left () {
    string *values = ({}), *results = ({});

    expect_function("pad_left", testOb);

    values += ({ testOb->pad_left("test", 4) });
    results += ({ "test" });
    values += ({ testOb->pad_left("test", 5) });
    results += ({ " test" });
    values += ({ testOb->pad_left("test", 10) });
    results += ({ "      test" });
    values += ({ testOb->pad_left("test", 20) });
    results += ({ "                test" });
    values += ({ testOb->pad_left("test", 2) });
    results += ({ "test" });

    expect_arrays_equal (values, results, "pad_left handled padding");
}

void test_pad_right () {
    string *values = ({}), *results = ({});

    expect_function("pad_right", testOb);

    values += ({ testOb->pad_right("test", 4) });
    results += ({ "test" });
    values += ({ testOb->pad_right("test", 5) });
    results += ({ "test " });
    values += ({ testOb->pad_right("test", 10) });
    results += ({ "test      " });
    values += ({ testOb->pad_right("test", 20) });
    results += ({ "test                " });
    values += ({ testOb->pad_right("test", 2) });
    results += ({ "test" });
    expect_arrays_equal (values, results, "pad_right handled padding");
}

void test_identify () {
    int tInt = 123, undefInt;
    float tFloat = 123.0, undefFloat;
    object tOb, undefOb;
    string tString = "Here it is: \"abc123\".", undefStr;
    mapping tMap = ([ "test1": "abc", "test2": 123 ]), undefMap;
    function tFn = function(int a, int b) { return a + b; }, undefFn;

    expect_function("identify", testOb);

    expect_array_strings_equal(({
        testOb->identify(),
        testOb->identify(undefInt),
        testOb->identify(undefFloat),
        testOb->identify(undefOb),
        testOb->identify(undefStr),
        testOb->identify(undefMap),
        testOb->identify(undefFn),
    }), "UNDEFINED", "identify handled undefined");

    // @TODO how create test nullp when undefinedp is true also
    // expect_strings_equal(testOb->identify(nullParam), "NULL", "identify handled null");

    expect_arrays_equal(({
        testOb->identify(1),
        testOb->identify(0),
        testOb->identify(-1),
        testOb->identify(MAX_INT),      // from driver
        testOb->identify(MIN_INT),      // from driver
        testOb->identify(tInt),
    }), ({
        "1",
        "0",
        "-1",
        "9223372036854775807",
        "-9223372036854775807",         // defined as +1
        "123",
    }), "identify handled int");

    expect_arrays_equal(({
        testOb->identify(1.0),
        testOb->identify(0.0),
        testOb->identify(-1.0),
        testOb->identify(MAX_FLOAT),    // from driver
        testOb->identify(MIN_FLOAT),    // from driver
        testOb->identify(tFloat),
    }), ({
        "1.000000",
        "0.000000",
        "-1.000000",
        "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000",
        "0.000000",
        "123.000000",
    }), "identify handled float");

    tOb = new(STD_OBJECT);
    expect_strings_regexp(testOb->identify(tOb), "OBJ\\("+replace_string(STD_OBJECT[0..<3], "/", "\\/")+"#(.+)\\)", "identify handled object");
    destruct(tOb);

    // string
    expect_arrays_equal(({
        testOb->identify("abc"),
        testOb->identify("123"),
        testOb->identify("!@#"),
        testOb->identify(tString),
    }), ({
        "\"abc\"",
        "\"123\"",
        "\"!@#\"",
        "\"Here it is: \\\"abc123\\\".\"",
    }), "identify handled string");

    // map
    expect_arrays_equal(({
        testOb->identify(([])),
        testOb->identify((["key1":"value1","key2":"value2",])),
        testOb->identify(([0:1,1:2,])),
        testOb->identify(tMap),
    }), ({
        "([ ])",
        "([ \"key1\": \"value1\", \"key2\": \"value2\" ])",
        "([ 0: 1, 1: 2 ])",
        "([ \"test1\": \"abc\", \"test2\": 123 ])",
    }), "identify handled map");

    // function
    expect_arrays_equal(({
        testOb->identify(function() {}),
        testOb->identify((: uptime :)),
        testOb->identify((: $1 + $2 :)),
        testOb->identify((: call_other, this_object(), "???" :)),
        testOb->identify(tFn),
    }), ({
        "(: <code>() :)",
        "(: uptime :)",
        "(: <code>($1, $2) :)",
        "(: call_other, "+file_name(this_object())+", \"???\" :)",
        "(: <code>($1, $2) :)",
    }), "identify handled function");

    // class
    // @TODO

    // pointer
    // @TODO

    // unknown
    // @TODO
}

// @TODO swap this halfway thru tests from ansi to unknown
// string getenv(string str) {
//     debug_message("getenv");
//     return "unknown";
// }

void test_wrap () {
    string *values = ({}), *results = ({});
    // string linewrap = "\e[0;37;40m\n"; // @TODO when unknown ansi
    string linewrap = "\n";

    expect_function("wrap", testOb);

    values += ({ testOb->wrap("test", 80) });
    results += ({ "test" });
    values += ({ testOb->wrap("testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest") });
    results += ({ "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest" + linewrap + "test" });
    values += ({ testOb->wrap("testtest", 4) });
    results += ({ "test" + linewrap + "test" });

    values += ({ testOb->wrap("testtesttest", 10) });
    results += ({ "testtestte" + linewrap + "st" });
    values += ({ testOb->wrap("testtesttest", 10, 2) });
    results += ({ "testtestte" + linewrap + "  st" });

    values += ({ testOb->wrap("", 80) });
    results += ({ "" });
    values += ({ testOb->wrap("test", -10) });
    results += ({ "test" });

    __MockAccount = new(STD_ACCOUNT);
    __MockAccount->set_setting("ansi", "on");

    values += ({ testOb->wrap("%^BOLD_OFF%^test", 80) });
    results += ({ "\e[22mtest" });
    values += ({ testOb->wrap("%^RED%^test%^RESET%^", 80) });
    results += ({ "\e[31mtest\e[0;37;40m" });

    expect_arrays_equal (values, results, "wrap handled wrapping");

    destruct(__MockAccount);
}

void test_string_compare_same_until () {
    int *values = ({}), *results = ({});

    expect_function("string_compare_same_until", testOb);

    values += ({ testOb->string_compare_same_until("abc", "abc") });
    results += ({ 3 });
    values += ({ testOb->string_compare_same_until("abc", "ab") });
    results += ({ 2 });
    values += ({ testOb->string_compare_same_until("ab", "abc") });
    results += ({ 2 });
    values += ({ testOb->string_compare_same_until("a", "abc") });
    results += ({ 1 });
    values += ({ testOb->string_compare_same_until("abc", "a") });
    results += ({ 1 });
    values += ({ testOb->string_compare_same_until("f", "abc") });
    results += ({ 0 });
    values += ({ testOb->string_compare_same_until("abc", "f") });
    results += ({ 0 });
    values += ({ testOb->string_compare_same_until("staff", "staves") });
    results += ({ 3 });
    values += ({ testOb->string_compare_same_until("staffs", "staves") });
    results += ({ 3 });
    expect_arrays_equal (values, results, "string_compare_same_until handled comparing");
}

void test_sanitize_name () {
    string *values = ({}), *results = ({});

    expect_function("sanitize_name", testOb);

    values += ({ testOb->sanitize_name("test") });
    results += ({ "test" });
    values += ({ testOb->sanitize_name("t'e's't") });
    results += ({ "test" });
    values += ({ testOb->sanitize_name("t e s t") });
    results += ({ "test" });
    values += ({ testOb->sanitize_name("t-e-s-t") });
    results += ({ "test" });
    values += ({ testOb->sanitize_name("TEST") });
    results += ({ "test" });
    values += ({ testOb->sanitize_name("T'- E'- S'- T") });
    results += ({ "test" });
    expect_arrays_equal (values, results, "sanitize_name handled names");
}
