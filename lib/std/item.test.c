#include <time.h>

inherit M_TEST;
inherit STD_OBJECT;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/item.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_item_verb_applies () {
    mixed *values = ({}), *results = ({});
    object r;

    r = new(STD_ROOM);

    expect_function("direct_look_at_obj", testOb);
    expect_function("direct_look_obj", testOb);

    values += ({ environment(testOb) });
    results += ({ 0 });
    values += ({ environment(this_object()) });
    results += ({ 0 });

    values += ({ testOb->direct_look_at_obj() });
    results += ({ 0 });
    values += ({ testOb->direct_look_obj() });
    results += ({ 0 });

    values += ({ testOb->handle_move(r) });
    results += ({ 1 });
    values += ({ environment(testOb) });
    results += ({ r });
    values += ({ this_object()->handle_move(r) });
    results += ({ 1 });

    // values += ({ environment(this_object()) });
    // results += ({ r });

    // values += ({ testOb->direct_look_at_obj() });
    // results += ({ 1 });
    // values += ({ testOb->direct_look_obj() });
    // results += ({ 1 });

    values += ({ this_object()->handle_move("/domain/Nowhere/void.c") });
    results += ({ 1 });

    expect_arrays_equal(values, results, "exits handled verb applies");

    if (r) destruct(r);
}