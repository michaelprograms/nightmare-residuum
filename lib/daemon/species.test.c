inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/species.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_species_bodytypes () {
    expect_function("query_bodytypes", testOb);
    expect_function("query_species", testOb);

    expect("bodytypes exist", (: ({
        assert(mappingp(testOb->query_bodytypes()), "==", 1),
        assert(sizeof(testOb->query_bodytypes()), ">", 0),
        // verify common bodytype
        assert(mappingp(testOb->query_bodytypes()["quadruped"]), "==", 1),
    }) :));

    expect("species exist", (: ({
        assert(mappingp(testOb->query_species()), "==", 1),
        assert(sizeof(testOb->query_species()), ">", 0),
        // verify common species
        assert(mappingp(testOb->query_species()["human"]), "==", 1),
    }) :));
}

void test_setup_body () {
    object ob;
    mapping limbs = ([ ]);

    expect_function("setup_body", testOb);

    // setting a species will setup the body
    ob = new(STD_LIVING);
    ob->set_species("human");
    // store the limb data
    foreach (string limb in ob->query_limbs()) {
        limbs[limb] = ob->query_limb(limb);
    }

    expect("species bodytypes exist", (: ({
        // verify limbs exist
        assert(sizeof(keys($(limbs))), ">", 0),
        // compare body to limbs
        assert(testOb->setup_body($(ob)), "==", $(limbs)),
    }) :));

    destruct(ob);
}