inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/color");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_hex_to_int () {
    int *values = ({}), *results = ({});

    expect_function("hex_to_int", testOb);

    values += ({ testOb->hex_to_int("00") });
    results += ({ 0 });
    values += ({ testOb->hex_to_int("0A") });
    results += ({ 10 });
    values += ({ testOb->hex_to_int("0F") });
    results += ({ 15 });
    values += ({ testOb->hex_to_int("A0") });
    results += ({ 160 });
    values += ({ testOb->hex_to_int("F0") });
    results += ({ 240 });
    values += ({ testOb->hex_to_int("AA") });
    results += ({ 170 });
    values += ({ testOb->hex_to_int("BB") });
    results += ({ 187 });
    values += ({ testOb->hex_to_int("CC") });
    results += ({ 204 });
    values += ({ testOb->hex_to_int("DD") });
    results += ({ 221 });
    values += ({ testOb->hex_to_int("EE") });
    results += ({ 238 });
    values += ({ testOb->hex_to_int("FF") });
    results += ({ 255 });
    expect_arrays_equal(values, results, "hex_to_int handled base16");

    expect_catches (({
        (: testOb->hex_to_int() :),
        (: testOb->hex_to_int(0) :),
        (: testOb->hex_to_int("") :),
        (: testOb->hex_to_int(({})) :),
        (: testOb->hex_to_int(([])) :),
        (: testOb->hex_to_int("A") :),
        (: testOb->hex_to_int("GG") :),
    }), "*Bad argument 1 to color->hex_to_int\n", "hex_to_int bandled bad inputs");

}