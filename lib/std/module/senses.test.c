inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/senses.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_listen () {
    expect_function("set_listen", testOb);
    expect_function("query_listen", testOb);

    expect("listens are settable and queryable", (: ({
        // verify initial default
        assert(testOb->query_listen("default"), "==", UNDEFINED),
        // verify default
        testOb->set_listen("default", "Default sounds."),
        assert(testOb->query_listen("default"), "==", "Default sounds."),
        // verify non-default
        testOb->set_listen("something", "Some noise."),
        assert(testOb->query_listen("something"), "==", "Some noise."),
        // verify function
        testOb->set_listen("fn", function () { return "Function sound."; }),
        assert(testOb->query_listen("fn"), "==", function () { return "Function sound."; }),
    }) :));

    expect("set_listen handles invalid argument 1", (: ({
        assert((: testOb->set_listen() :), "catch", "*Bad argument 1 to senses->set_listen\n"),
        assert((: testOb->set_listen(0) :), "catch", "*Bad argument 1 to senses->set_listen\n"),
        assert((: testOb->set_listen(0.0) :), "catch", "*Bad argument 1 to senses->set_listen\n"),
        assert((: testOb->set_listen((: 0 :)) :), "catch", "*Bad argument 1 to senses->set_listen\n"),
        assert((: testOb->set_listen(({ })) :), "catch", "*Bad argument 1 to senses->set_listen\n"),
        assert((: testOb->set_listen(([ ])) :), "catch", "*Bad argument 1 to senses->set_listen\n"),
    }) :));

    expect("set_listen handles invalid argument 2", (: ({
        assert((: testOb->set_listen("sound", ) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("sound", 0) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("sound", 0.0) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("sound", ({ })) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("sound", ([ ])) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
    }) :));
}

void test_smell () {
    expect_function("set_smell", testOb);
    expect_function("query_smell", testOb);

    expect("smells are settable and queryable", (: ({
        // verify initial default
        assert(testOb->query_smell("default"), "==", 0),
        // verify default
        testOb->set_smell("default", "Default scents."),
        assert(testOb->query_smell("default"), "==", "Default scents."),
        // verify non-default
        testOb->set_smell("something", "Some smell."),
        assert(testOb->query_smell("something"), "==", "Some smell."),
        // verify function
        testOb->set_smell("fn", function () { return "Function scent."; }),
        assert(testOb->query_smell("fn"), "==", function () { return "Function scent."; }),
    }) :));

    expect("set_smell handles invalid argument 1", (: ({
        assert((: testOb->set_smell() :), "catch", "*Bad argument 1 to senses->set_smell\n"),
        assert((: testOb->set_smell(0) :), "catch", "*Bad argument 1 to senses->set_smell\n"),
        assert((: testOb->set_smell(0.0) :), "catch", "*Bad argument 1 to senses->set_smell\n"),
        assert((: testOb->set_smell((: 0 :)) :), "catch", "*Bad argument 1 to senses->set_smell\n"),
        assert((: testOb->set_smell(({ })) :), "catch", "*Bad argument 1 to senses->set_smell\n"),
        assert((: testOb->set_smell(([ ])) :), "catch", "*Bad argument 1 to senses->set_smell\n"),
    }) :));

    expect("set_listen handles invalid argument 2", (: ({
        assert((: testOb->set_listen("smell", ) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("smell", 0) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("smell", 0.0) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("smell", ({ })) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
        assert((: testOb->set_listen("smell", ([ ])) :), "catch", "*Bad argument 2 to senses->set_listen\n"),
    }) :));
}