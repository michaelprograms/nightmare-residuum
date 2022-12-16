inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/npc/pet.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}