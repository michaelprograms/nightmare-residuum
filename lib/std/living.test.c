inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_living () {
    expect_function("is_living", testOb);

    expect("is_living returns true", (: ({
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_npc(), "==", 0),
        assert(testOb->is_character(), "==", 0),
    }) :));
}

void test_species () {
    expect_function("query_species", testOb);
    expect_function("set_species", testOb);

    expect("living handles species", (: ({
        assert(testOb->query_species(), "==", "unknown"),
        assert(testOb->set_species("human"), "==", 0),
        assert(testOb->query_species(), "==", "human"),
    }) :));
}