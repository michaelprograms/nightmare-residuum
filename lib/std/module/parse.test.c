inherit M_TEST;

void test_add_rules () {
    object mockParse = new("/std/module/parse.mock.c");

    expect("rules and synonyms are added", (: ({
        assert_equal($(mockParse)->start_shadow(testOb), 1),

        assert_equal($(mockParse)->query_rules(), ({ })),
        assert_equal($(mockParse)->query_synonym(), ({ })),

        testOb->add_rules(({ "", "LVS", }), ({ "synonym1", "synonym2" })),

        assert_equal($(mockParse)->query_rules(), ({ ({ "mockparse", "" }), ({ "mockparse", "LVS" }) })),
        assert_equal($(mockParse)->query_synonym(), ({ ({ "synonym1", "mockparse", "" }), ({ "synonym2", "mockparse", "" }), ({ "synonym1", "mockparse", "LVS" }), ({ "synonym2", "mockparse", "LVS" }) })),

        assert_equal($(mockParse)->stop_shadow(), 1),
    }) :));
    if (mockParse) destruct(mockParse);
}