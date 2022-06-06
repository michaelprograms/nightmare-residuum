#include <verb.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/vendor_inventory.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
