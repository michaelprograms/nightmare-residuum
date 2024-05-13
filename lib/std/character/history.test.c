inherit M_TEST;

void test_history () {
    expect("null history is initialized", (: ({
        assert_equal(testOb->query_tell_history(), ({ })),
        assert_equal(testOb->query_say_history(), ({ })),
        store_variable("__Tells", UNDEFINED, testOb),
        store_variable("__Says", UNDEFINED, testOb),
        assert_equal(testOb->query_tell_history(), ({ })),
        assert_equal(testOb->query_say_history(), ({ })),
    }) :));

    expect("tell history is settable and queryable", (: ({
        assert_equal(testOb->query_tell_history(), ({ })),
        testOb->add_tell_history("tell1"),
        assert_equal(testOb->query_tell_history(), ({ "tell1" })),
        testOb->add_tell_history("tell2"),
        assert_equal(testOb->query_tell_history(), ({ "tell1", "tell2" })),
        testOb->add_tell_history("tell3"),
        assert_equal(testOb->query_tell_history(), ({ "tell1", "tell2", "tell3" })),
        testOb->add_tell_history("tell4"),
        assert_equal(testOb->query_tell_history(), ({ "tell1", "tell2", "tell3", "tell4" })),
        testOb->add_tell_history("tell5"),
        assert_equal(testOb->query_tell_history(), ({ "tell1", "tell2", "tell3", "tell4", "tell5" })),
        testOb->add_tell_history("tell6"),
        testOb->add_tell_history("tell7"),
        testOb->add_tell_history("tell8"),
        testOb->add_tell_history("tell9"),
        testOb->add_tell_history("tell10"),
        testOb->add_tell_history("tell11"),
        testOb->add_tell_history("tell12"),
        testOb->add_tell_history("tell13"),
        testOb->add_tell_history("tell14"),
        testOb->add_tell_history("tell15"),
        testOb->add_tell_history("tell16"),
        testOb->add_tell_history("tell17"),
        testOb->add_tell_history("tell18"),
        testOb->add_tell_history("tell19"),
        testOb->add_tell_history("tell20"),
        testOb->add_tell_history("tell21"),
        testOb->add_tell_history("tell22"),
        testOb->add_tell_history("tell23"),
        testOb->add_tell_history("tell24"),
        testOb->add_tell_history("tell25"),
        assert_equal(testOb->query_tell_history(), ({ "tell1", "tell2", "tell3", "tell4", "tell5", "tell6", "tell7", "tell8", "tell9","tell10", "tell11", "tell12", "tell13", "tell14", "tell15", "tell16", "tell17", "tell18", "tell19", "tell20", "tell21", "tell22", "tell23", "tell24", "tell25" })),
        testOb->add_tell_history("tell26"),
        assert_equal(testOb->query_tell_history(), ({ "tell2", "tell3", "tell4", "tell5", "tell6", "tell7", "tell8", "tell9","tell10", "tell11", "tell12", "tell13", "tell14", "tell15", "tell16", "tell17", "tell18", "tell19", "tell20", "tell21", "tell22", "tell23", "tell24", "tell25", "tell26" })),
    }) :));

    expect("say history is settable and queryable", (: ({
        assert_equal(testOb->query_say_history(), ({ })),
        testOb->add_say_history("say1"),
        assert_equal(testOb->query_say_history(), ({ "say1" })),
        testOb->add_say_history("say2"),
        assert_equal(testOb->query_say_history(), ({ "say1", "say2" })),
        testOb->add_say_history("say3"),
        assert_equal(testOb->query_say_history(), ({ "say1", "say2", "say3" })),
        testOb->add_say_history("say4"),
        assert_equal(testOb->query_say_history(), ({ "say1", "say2", "say3", "say4" })),
        testOb->add_say_history("say5"),
        assert_equal(testOb->query_say_history(), ({ "say1", "say2", "say3", "say4", "say5" })),
        testOb->add_say_history("say6"),
        testOb->add_say_history("say7"),
        testOb->add_say_history("say8"),
        testOb->add_say_history("say9"),
        testOb->add_say_history("say10"),
        testOb->add_say_history("say11"),
        testOb->add_say_history("say12"),
        testOb->add_say_history("say13"),
        testOb->add_say_history("say14"),
        testOb->add_say_history("say15"),
        testOb->add_say_history("say16"),
        testOb->add_say_history("say17"),
        testOb->add_say_history("say18"),
        testOb->add_say_history("say19"),
        testOb->add_say_history("say20"),
        testOb->add_say_history("say21"),
        testOb->add_say_history("say22"),
        testOb->add_say_history("say23"),
        testOb->add_say_history("say24"),
        testOb->add_say_history("say25"),
        assert_equal(testOb->query_say_history(), ({ "say1", "say2", "say3", "say4", "say5", "say6", "say7", "say8", "say9","say10", "say11", "say12", "say13", "say14", "say15", "say16", "say17", "say18", "say19", "say20", "say21", "say22", "say23", "say24", "say25" })),
        testOb->add_say_history("say26"),
        assert_equal(testOb->query_say_history(), ({ "say2", "say3", "say4", "say5", "say6", "say7", "say8", "say9","say10", "say11", "say12", "say13", "say14", "say15", "say16", "say17", "say18", "say19", "say20", "say21", "say22", "say23", "say24", "say25", "say26" })),
    }) :));

    expect("history catches bad inputs", (: ({
        assert_catch((: testOb->add_tell_history(0) :), "*Bad argument 1 to history->add_tell_history\n"),

        testOb->add_tell_history("Test1"),
        assert_catch((: testOb->add_say_history(0) :), "*Bad argument 1 to history->add_say_history\n"),
    }) :));
}

void test_tell_reply () {
    expect("tell reply is settable and queryable", (: ({
        assert_equal(testOb->query_tell_reply(), UNDEFINED),

        testOb->set_tell_reply("test1"),
        assert_equal(testOb->query_tell_reply(), "test1"),

        testOb->set_tell_reply("test2"),
        assert_equal(testOb->query_tell_reply(), "test2"),
    }) :));
}