inherit M_TEST;
inherit STD_OBJECT;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/biography.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_ignore () {
    return ::test_ignore() + ({ "query_level", "is_living" });
}
string *test_order () {
    return ({ "test_experience", "test_add_victory", "test_add_defeat", "test_handle_victory", });
}

void test_experience () {
    int *values = ({}), *results = ({});

    expect_function("add_experience", testOb);
    expect_function("spend_experience", testOb);
    expect_function("query_experience", testOb);
    expect_function("query_total_experience", testOb);

    values += ({ testOb->query_experience() });
    results += ({ 0 });
    values += ({ testOb->query_total_experience() });
    results += ({ 0 });

    testOb->add_experience(123);
    values += ({ testOb->query_experience() });
    results += ({ 123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 0 });

    testOb->add_experience(9000);
    values += ({ testOb->query_experience() });
    results += ({ 9123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 0 });

    testOb->spend_experience(1000);
    values += ({ testOb->query_experience() });
    results += ({ 8123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 1000 });

    testOb->spend_experience(2000);
    values += ({ testOb->query_experience() });
    results += ({ 6123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 3000 });

    expect_arrays_equal(values, results, "handled experience");

    expect_catches (({
        (: testOb->spend_experience(10000) :),
        (: testOb->spend_experience(0) :),
        (: testOb->spend_experience(-10000) :),
    }), "*Bad argument 1 to body->spend_experience\n", "spend_experience handled bad inputs ");
}

nosave private int __MockLevel, __MockLiving;
int query_level () { return __MockLevel; }
int is_living () { return __MockLiving; }

void test_add_victory () {
    int *values = ({}), *results = ({});

    expect_function("add_victory", testOb);
    expect_function("query_victory", testOb);

    values += ({ testOb->query_victory() });
    results += ({ 0 });

    testOb->add_victory();
    values += ({ testOb->query_victory() });
    results += ({ 1 });

    testOb->add_victory();
    values += ({ testOb->query_victory() });
    results += ({ 2 });

    expect_arrays_equal(values, results, "add_victory and query_victory behave");
}

void test_add_defeat () {
    int *values = ({}), *results = ({});

    expect_function("add_defeat", testOb);
    expect_function("query_defeat", testOb);

    values += ({ testOb->query_defeat() });
    results += ({ 0 });

    testOb->add_defeat();
    values += ({ testOb->query_defeat() });
    results += ({ 1 });

    testOb->add_defeat();
    values += ({ testOb->query_defeat() });
    results += ({ 2 });

    expect_arrays_equal(values, results, "add_defeat and query_defeat behave");
}

void test_handle_victory () {
    int *values = ({}), *results = ({});

    expect_function("handle_victory", testOb);

    // setup test object
    __MockLiving = 1;
    __MockLevel = 1;
    values += ({ testOb->query_experience() });
    results += ({ 0 });
    values += ({ testOb->query_victory() });
    results += ({ 0 });

    // test victory
    testOb->handle_victory(this_object());
    values += ({ testOb->query_experience() > 0 });
    results += ({ 1 });
    values += ({ testOb->query_victory() });
    results += ({ 1 });

    expect_arrays_equal(values, results, "handle_victory behaved");

    // cleanup
    __MockLiving = 0;
    __MockLevel = 0;
}

void test_handle_defeat () {
    mixed *values = ({}), *results = ({});
    object r;

    r = new(STD_ROOM);

    expect_function("handle_defeat", testOb);

    // setup test object
    if (testOb) destruct(testOb);
    testOb = new(STD_LIVING); // need living for handle_move
    values += ({ testOb->query_experience() });
    results += ({ 0 });
    values += ({ testOb->query_defeat() });
    results += ({ 0 });
    values += ({ testOb->handle_move(r) });
    results += ({ 1 });
    values += ({ sizeof(r->query_living_contents()) });
    results += ({ 1 });
    values += ({ sizeof(r->query_item_contents()) });
    results += ({ 0 });

    // test defeat with keep
    testOb->handle_defeat(1);
    values += ({ objectp(testOb) });
    results += ({ 1 });
    values += ({ testOb->query_defeat() });
    results += ({ 1 });
    values += ({ sizeof(r->query_living_contents()) });
    results += ({ 0 });
    values += ({ sizeof(r->query_item_contents()) });
    results += ({ 1 });
    values += ({ !!present("corpse", r) });
    results += ({ 1 });
    values += ({ present("corpse", r)->handle_remove() });
    results += ({ 1 });

    // move test object back
    values += ({ testOb->handle_move(r) });
    results += ({ 1 });
    values += ({ sizeof(r->query_living_contents()) });
    results += ({ 1 });
    values += ({ sizeof(r->query_item_contents()) });
    results += ({ 0 });

    // test defeat with keep
    testOb->handle_defeat(0);
    values += ({ objectp(testOb) });
    results += ({ 0 });
    values += ({ sizeof(r->query_living_contents()) });
    results += ({ 0 });
    values += ({ sizeof(r->query_item_contents()) });
    results += ({ 1 });
    values += ({ !!present("corpse", r) });
    results += ({ 1 });
    values += ({ present("corpse", r)->handle_remove() });
    results += ({ 1 });

    expect_arrays_equal(values, results, "handle_defeat behaved");

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}