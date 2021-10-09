inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/daemon/ansi");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_ansi_terms () {
    mapping term;
    int fail;
    expect_function("query_ansi_term", testOb);
    term = testOb->query_ansi_term();
    expect_true(sizeof(term) == 34, "query_ansi_term has 34 items");
    foreach (string key in keys(term)) {
        if (key == "ENDTERM") continue; // skip this one
        if (term[key] == "") {
            fail ++;
        }
    }
    expect_true(fail == 0, "query_ansi_term has definitions");
}

void test_unknown_terms () {
    mapping term;
    int fail = 0;
    expect_function("query_unknown_term", testOb);
    term = testOb->query_unknown_term();
    expect_true(sizeof(term) == 34, "query_unknown_term has 34 items");
    foreach (string key in keys(term)) {
        if (term[key] != "") {
            fail ++;
        }
    }
    expect_true(fail == 0, "query_unknown_term are empty");
}

void test_parse () {

    expect_function("parse", testOb);

    expect_arrays_equal (({
        testOb->parse("%^RESET%^"),
        testOb->parse("%^BOLD%^"),
        testOb->parse("%^ITALIC%^"),
        testOb->parse("%^UNDERLINE%^"),
        testOb->parse("%^FLASH%^"),
        testOb->parse("%^INVERSE%^"),
        testOb->parse("%^STRIKE%^"),
        testOb->parse("%^BOLD_OFF%^"),
        testOb->parse("%^ITALIC_OFF%^"),
        testOb->parse("%^UNDERLINE_OFF%^"),
        testOb->parse("%^FLASH_OFF%^"),
        testOb->parse("%^INVERSE_OFF%^"),
        testOb->parse("%^STRIKE_OFF%^"),
        testOb->parse("%^BLACK%^"),
        testOb->parse("%^RED%^"),
        testOb->parse("%^GREEN%^"),
        testOb->parse("%^ORANGE%^"),
        testOb->parse("%^YELLOW%^"),
        testOb->parse("%^BLUE%^"),
        testOb->parse("%^MAGENTA%^"),
        testOb->parse("%^CYAN%^"),
        testOb->parse("%^WHITE%^"),
        testOb->parse("%^B_BLACK%^"),
        testOb->parse("%^B_RED%^"),
        testOb->parse("%^B_GREEN%^"),
        testOb->parse("%^B_ORANGE%^"),
        testOb->parse("%^B_YELLOW%^"),
        testOb->parse("%^B_BLUE%^"),
        testOb->parse("%^B_MAGENTA%^"),
        testOb->parse("%^B_CYAN%^"),
        testOb->parse("%^B_WHITE%^"),
    }), ({
        "\e[0;37;40m",
        "\e[1m",
        "\e[3m",
        "\e[4m",
        "\e[5m",
        "\e[7m",
        "\e[9m",
        "\e[22m",
        "\e[23m",
        "\e[24m",
        "\e[25m",
        "\e[27m",
        "\e[29m",
        "\e[30m",
        "\e[31m",
        "\e[32m",
        "\e[33m",
        "\e[1;33m",
        "\e[34m",
        "\e[35m",
        "\e[36m",
        "\e[37m",
        "\e[40m",
        "\e[41m",
        "\e[42m",
        "\e[43m",
        "\e[1;43m",
        "\e[44m",
        "\e[45m",
        "\e[46m",
        "\e[47m",
    }), "parse parsed all colors");
}