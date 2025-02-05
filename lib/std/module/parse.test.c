inherit M_TEST;

/**
 * @var {"/std/module/parse"} testOb
 */

void test_add_rules_with_mock () {
    object mockParse = new("/std/module/parse.mock.c");

    expect("rules and synonyms are added", (: ({
        assert_equal($(mockParse)->start_shadow(testOb), 1),

        assert_equal($(mockParse)->query_rules(), ({ })),
        assert_equal($(mockParse)->query_synonym(), ({ })),

        testOb->add_rules(({ "", "LVS", }), ({ "synonym1", "synonym2" })),

        assert_equal($(mockParse)->query_rules(), ({ ({ "module/parse", "" }), ({ "module/parse", "LVS" }) })),
        assert_equal($(mockParse)->query_synonym(), ({ ({ "synonym1", "module/parse", "" }), ({ "synonym2", "module/parse", "" }), ({ "synonym1", "module/parse", "LVS" }), ({ "synonym2", "module/parse", "LVS" }) })),

        assert_equal($(mockParse)->stop_shadow(), 1),
    }) :));
    if (mockParse) destruct(mockParse);
}