inherit M_TEST;

private nosave string __ANSI = "on";
mixed query_setting (string setting) {
    if (setting == "ansi") {
        return __ANSI;
    }
}

class TestClass {
    string  str;
    string *strArr;
    int     i;
    float   f;
    mapping m;
    object  o;
}
class TestClass tc = new(class TestClass);

void test_identify () {
    int tInt = 123, undefInt;
    float tFloat = 123.0, undefFloat;
    object tOb, undefOb;
    string tString = "Here it is: \"abc123\".", undefStr;
    mapping tMap = ([ "test1": "abc", "test2": 123 ]), undefMap;
    function tFn = function(int a, int b) { return a + b; }, undefFn;

    expect("identify handles undefined", (: ({
        assert_equal(testOb->identify(), "UNDEFINED"),
        assert_equal(testOb->identify($(undefInt)), "UNDEFINED"),
        assert_equal(testOb->identify($(undefFloat)), "UNDEFINED"),
        assert_equal(testOb->identify($(undefOb)), "UNDEFINED"),
        assert_equal(testOb->identify($(undefStr)), "UNDEFINED"),
        assert_equal(testOb->identify($(undefMap)), "UNDEFINED"),
        assert_equal(testOb->identify($(undefFn)), "UNDEFINED"),
    }) :));

    expect("identify handles int", (: ({
        assert_equal(testOb->identify(1), "1"),
        assert_equal(testOb->identify(0), "0"),
        assert_equal(testOb->identify(-1), "-1"),
        assert_equal(testOb->identify(MAX_INT), "9223372036854775807"),
        assert_equal(testOb->identify(MIN_INT), "-9223372036854775807"),
        assert_equal(testOb->identify($(tInt)), "123"),
    }) :));

    expect("identify handles float", (: ({
        assert_equal(testOb->identify(1.0), "1.000000"),
        assert_equal(testOb->identify(0.0), "0.000000"),
        assert_equal(testOb->identify(-1.0), "-1.000000"),
        assert_equal(testOb->identify(MAX_FLOAT), "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000"),
        assert_equal(testOb->identify(MIN_FLOAT), "0.000000"),
        assert_equal(testOb->identify($(tFloat)), "123.000000"),
    }) :));

    tOb = new(STD_OBJECT);
    expect("identify handles object", (: ({
        assert_regex(testOb->identify($(tOb)), "OBJ\\("+replace_string(STD_OBJECT[0..<3], "/", "\\/")+"#(.+)\\)"),

        $(tOb)->set_key_name("tester"),
        assert_regex(testOb->identify($(tOb)), "OBJ\\(tester "+replace_string(STD_OBJECT[0..<3], "/", "\\/")+"#(.+)\\)"),
    }) :));
    destruct(tOb);

    expect("identify handles string", (: ({
        assert_equal(testOb->identify("abc"), "\"abc\""),
        assert_equal(testOb->identify("123"), "\"123\""),
        assert_equal(testOb->identify("!@#"), "\"!@#\""),
        assert_equal(testOb->identify($(tString)), "\"Here it is: \\\"abc123\\\".\""),
    }) :));

    expect("identify handles map", (: ({
        assert_equal(testOb->identify(([ ])), "([ ])"),
        assert_equal(testOb->identify((["key1":"value1","key2":"value2",])), "([ \"key1\": \"value1\", \"key2\": \"value2\" ])"),
        assert_equal(testOb->identify(([0:1,1:2,])), "([ 0: 1, 1: 2 ])"),
        assert_equal(testOb->identify($(tMap)), "([ \"test1\": \"abc\", \"test2\": 123 ])"),
    }) :));

    expect("identify handles function", (: ({
        assert_equal(testOb->identify(function() {}), "(: <code>() :)"),
        assert_equal(testOb->identify((: uptime :)), "(: uptime :)"),
        assert_equal(testOb->identify((: $1 + $2 :)), "(: <code>($1, $2) :)"),
        assert_equal(testOb->identify((: call_other, this_object(), "query_name" :)), "(: call_other, " + file_name() + ", \"query_name\" :)"),
        assert_equal(testOb->identify($(tFn)), "(: <code>($1, $2) :)"),
    }) :));

    expect("identify handles class", (: ({
        assert_equal(testOb->identify(tc), "CLASS( 6 elements  0,  0,  0,  0,  0,  0 )"),
        tc->str = "test string",
        tc->strArr = ({ "test string 1", "test string 2", }),
        tc->i = 123,
        tc->f = 1.0,
        tc->m = ([ "test": 123, ]),
        tc->o = this_object(),
        assert_equal(testOb->identify(tc), "CLASS( 6 elements  \"test string\",  ({ /* sizeof() == 2 */    \"test string 1\",    \"test string 2\"  }),  123,  1.000000,  ([ /* sizeof() == 1 */    \"test\" : 123,  ]),  " + file_name() + " )"),
    }) :));

    expect("identify handles array", (: ({
        assert_equal(testOb->identify(({ 1, 2, 3})), "({ 1, 2, 3 })"),
    }) :));

    expect("identify handles buffer", (: ({
        assert_equal(testOb->identify(read_buffer("/secure/sefun/string.test.c", 0, 8)), "BUFFER (0x69, 0x6E, 0x68, 0x65, 0x72, 0x69, 0x74, 0x20)"),
    }) :));
}

void test_wrap () {
    string resetANSI = "\e[0m", linewrap = "\n";

    expect("wrap handles wrapping text", (: ({
        assert_equal(this_object()->query_setting("ansi"), "on"),

        assert_equal(testOb->wrap("test", 80), "test"),
        assert_equal(testOb->wrap("testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest"), "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest" + $(resetANSI) + $(linewrap) + "test"),
        assert_equal(testOb->wrap("testtest", 4), "test" + $(resetANSI) + $(linewrap) + "test"),
        assert_equal(testOb->wrap("testtesttest", 10), "testtestte" + $(resetANSI) + $(linewrap) + "st"),
        assert_equal(testOb->wrap("testtesttest", 10, 2), "testtestte" + $(resetANSI) + $(linewrap) + "  st"),
        assert_equal(testOb->wrap("", 80), ""),
        assert_equal(testOb->wrap("test", -10), "test"),
        assert_equal(testOb->wrap("%^BOLD_OFF%^test", 80), "\e[22mtest"),
        assert_equal(testOb->wrap("%^RED%^test%^RESET%^", 80), "\e[31mtest" + $(resetANSI)),

        assert_equal(__ANSI = "off", "off"),

        assert_equal(testOb->wrap("test", 80), "test"),
        assert_equal(testOb->wrap("testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest"), "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest" + $(linewrap) + "test"),
        assert_equal(testOb->wrap("testtest", 4), "test" + $(linewrap) + "test"),
        assert_equal(testOb->wrap("testtesttest", 10), "testtestte" + $(linewrap) + "st"),
        assert_equal(testOb->wrap("testtesttest", 10, 2), "testtestte" + $(linewrap) + "  st"),
        assert_equal(testOb->wrap("", 80), ""),
        assert_equal(testOb->wrap("test", -10), "test"),
        assert_equal(testOb->wrap("%^BOLD_OFF%^test", 80), "test"),
        assert_equal(testOb->wrap("%^RED%^test%^RESET%^", 80), "test"),

        // test indent
        assert_equal(testOb->wrap("%^RED%^testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest%^RESET%^", 40, 4), "testtesttesttesttesttesttesttesttesttest\n    testtesttesttesttesttesttesttesttest\n    testtest"),

        // test raw ANSI
        assert_equal(testOb->wrap("%%^^RED%%^^testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest%%^^RESET%%^^", 20, 0, 1), "%^RED%^testtesttestt\nesttesttesttesttestt\nesttesttesttesttestt\nesttesttesttesttestt\nesttesttest%^RESET%^"),
    }) :));
}

void test_string_compare_same_until () {
    expect("string_compare_same_until handles comparing strings", (: ({
        assert_equal(testOb->string_compare_same_until("abc", "abc"), 3),
        assert_equal(testOb->string_compare_same_until("abc", "ab"), 2),
        assert_equal(testOb->string_compare_same_until("ab", "abc"), 2),
        assert_equal(testOb->string_compare_same_until("a", "abc"), 1),
        assert_equal(testOb->string_compare_same_until("abc", "a"), 1),
        assert_equal(testOb->string_compare_same_until("f", "abc"), 0),
        assert_equal(testOb->string_compare_same_until("abc", "f"), 0),
        assert_equal(testOb->string_compare_same_until("staff", "staves"), 3),
        assert_equal(testOb->string_compare_same_until("staffs", "staves"), 3),
    }) :));
}

void test_sanitize_name () {
    expect("sanitize_name handles names", (: ({
        assert_equal(testOb->sanitize_name("test"), "test"),
        assert_equal(testOb->sanitize_name("t'e's't"), "test"),
        assert_equal(testOb->sanitize_name("t e s t"), "test"),
        assert_equal(testOb->sanitize_name("t-e-s-t"), "test"),
        assert_equal(testOb->sanitize_name("TEST"), "test"),
        assert_equal(testOb->sanitize_name("T'- E'- S'- T"), "test"),
    }) :));
    expect("sanitize_name handles bad input", (: ({
        assert_catch((: testOb->sanitize_name(UNDEFINED) :), "*Bad argument 1 to string->sanitize_name\n"),
        assert_catch((: testOb->sanitize_name(0) :), "*Bad argument 1 to string->sanitize_name\n"),
    }) :));
}

void test_parse_command_flags () {
    expect("parse_command_flags parses valid flags", (: ({
        // check no flags
        assert_equal(testOb->parse_command_flags("something"), ({ "something", ([ ]) })),
        assert_equal(testOb->parse_command_flags(0), ({ 0, ([ ]) })),

        // check flags at front
        assert_equal(testOb->parse_command_flags("-a something"), ({ "something", ([ "a": 1, ]) })),
        assert_equal(testOb->parse_command_flags("-abc something"), ({ "something", ([ "abc": 1, ]) })),
        assert_equal(testOb->parse_command_flags("-a -b -c something"), ({ "something", ([ "a": 1, "b": 1, "c": 1, ]) })),
        assert_equal(testOb->parse_command_flags("-a=1 -b=2 -c=3 something"), ({ "something", ([ "a": "1", "b": "2", "c": "3", ]) })),
        assert_equal(testOb->parse_command_flags("-a=x -b=y -c=z something"), ({ "something", ([ "a": "x", "b": "y", "c": "z", ]) })),
        assert_equal(testOb->parse_command_flags("-path=/path/with/special_characters-in.it something else"), ({ "something else", ([ "path": "/path/with/special_characters-in.it", ]) })),
    }) :));

    expect("parse_command_flags parses invalid flags", (: ({
        assert_equal(testOb->parse_command_flags("- something"), ({ "something", ([ ]) })),
        assert_equal(testOb->parse_command_flags("something -"), ({ "something -", ([ ]) })),
        assert_equal(testOb->parse_command_flags("something -invalidflag"), ({ "something -invalidflag", ([ ]) })),
        assert_equal(testOb->parse_command_flags("x y z -a"), ({ "x y z -a", ([ ]) })),
        assert_equal(testOb->parse_command_flags("x y z -abc"), ({ "x y z -abc", ([ ]) })),
    }) :));
}