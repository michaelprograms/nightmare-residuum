inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/class.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_adjust_vitals () {
    expect_function("query_adjust_hp", testOb);
    expect_function("query_adjust_sp", testOb);
    expect_function("query_adjust_mp", testOb);

    expect("hp adjustments are queryable", (: ({
        assert(testOb->query_adjust_hp("warrior"), "==", 3),
        assert(testOb->query_adjust_hp("templar"), "==", 2),
        assert(testOb->query_adjust_hp("scoundrel"), "==", 1),
        assert(testOb->query_adjust_hp("ranger"), "==", 0),
        assert(testOb->query_adjust_hp("mentalist"), "==", 0),
        assert(testOb->query_adjust_hp("paladin"), "==", 2),
    }) :));

    expect("sp adjustments are queryable", (: ({
        assert(testOb->query_adjust_sp("warrior"), "==", 1),
        assert(testOb->query_adjust_sp("templar"), "==", 2),
        assert(testOb->query_adjust_sp("scoundrel"), "==", 3),
        assert(testOb->query_adjust_sp("ranger"), "==", 2),
        assert(testOb->query_adjust_sp("mentalist"), "==", 1),
        assert(testOb->query_adjust_sp("paladin"), "==", 0),
    }) :));

    expect("mp adjustments are queryable", (: ({
        assert(testOb->query_adjust_mp("warrior"), "==", 0),
        assert(testOb->query_adjust_mp("templar"), "==", 0),
        assert(testOb->query_adjust_mp("scoundrel"), "==", 0),
        assert(testOb->query_adjust_mp("ranger"), "==", 2),
        assert(testOb->query_adjust_mp("mentalist"), "==", 3),
        assert(testOb->query_adjust_mp("paladin"), "==", 2),
    }) :));
}