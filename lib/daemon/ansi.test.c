inherit M_TEST;

mapping term;
void test_ansi_terms () {
    int fail;

    term = testOb->query_ansi_term();
    expect("query_ansi_term has 541 items", (: ({
        assert_equal(mapp(term), 1),
        assert_equal(sizeof(term), 543)
    }) :));

    foreach (string key in keys(term)) {
        if (term[key] == "") {
            fail ++;
        }
    }
    expect("query_ansi_term has definitions", (: ({
        assert_equal($(fail), 0),
    }) :));
}

void test_unknown_terms () {
    int fail = 0;

    term = testOb->query_unknown_term();
    expect("query_unknown_term has 541 items", (: ({
        assert_equal(mapp(term), 1),
        assert_equal(sizeof(term), 543),
    }) :));

    foreach (string key in keys(term)) {
        if (term[key] != "") {
            fail ++;
        }
    }
    expect("query_unknown_term has no definitions", (: ({
        assert_equal($(fail), 0),
    }) :));
}

void test_keys () {
    expect("query_keys matches terms", (: ({
        assert_equal(sizeof(testOb->query_keys()), sizeof(testOb->query_ansi_term())),
        assert_equal(sizeof(testOb->query_keys()), sizeof(testOb->query_unknown_term())),
    }) :));
}

void test_parse () {
    expect("parse parses all basic colors", (: ({
        assert_equal(testOb->parse("%^RESET%^"), "\e[0m"),
        assert_equal(testOb->parse("%^BOLD%^"), "\e[1m"),
        assert_equal(testOb->parse("%^ITALIC%^"), "\e[3m"),
        assert_equal(testOb->parse("%^UNDERLINE%^"), "\e[4m"),
        assert_equal(testOb->parse("%^INVERSE%^"), "\e[7m"),
        assert_equal(testOb->parse("%^STRIKE%^"), "\e[9m"),
        assert_equal(testOb->parse("%^BOLD_OFF%^"), "\e[22m"),
        assert_equal(testOb->parse("%^ITALIC_OFF%^"), "\e[23m"),
        assert_equal(testOb->parse("%^UNDERLINE_OFF%^"), "\e[24m"),
        assert_equal(testOb->parse("%^INVERSE_OFF%^"), "\e[27m"),
        assert_equal(testOb->parse("%^STRIKE_OFF%^"), "\e[29m"),
        assert_equal(testOb->parse("%^BLACK%^"), "\e[30m"),
        assert_equal(testOb->parse("%^RED%^"), "\e[31m"),
        assert_equal(testOb->parse("%^GREEN%^"), "\e[32m"),
        assert_equal(testOb->parse("%^ORANGE%^"), "\e[33m"),
        assert_equal(testOb->parse("%^YELLOW%^"), "\e[1;33m"),
        assert_equal(testOb->parse("%^BLUE%^"), "\e[34m"),
        assert_equal(testOb->parse("%^MAGENTA%^"), "\e[35m"),
        assert_equal(testOb->parse("%^CYAN%^"), "\e[36m"),
        assert_equal(testOb->parse("%^WHITE%^"), "\e[37m"),
        assert_equal(testOb->parse("%^B_BLACK%^"), "\e[40m"),
        assert_equal(testOb->parse("%^B_RED%^"), "\e[41m"),
        assert_equal(testOb->parse("%^B_GREEN%^"), "\e[42m"),
        assert_equal(testOb->parse("%^B_ORANGE%^"), "\e[43m"),
        assert_equal(testOb->parse("%^B_YELLOW%^"), "\e[1;43m"),
        assert_equal(testOb->parse("%^B_BLUE%^"), "\e[44m"),
        assert_equal(testOb->parse("%^B_MAGENTA%^"), "\e[45m"),
        assert_equal(testOb->parse("%^B_CYAN%^"), "\e[46m"),
        assert_equal(testOb->parse("%^B_WHITE%^"), "\e[47m"),
    }) :));

    expect("parse parses xterm colors", (: ({
        assert_equal(testOb->parse("%^AAA%^"), "\e[38;5;16m"),
        assert_equal(testOb->parse("%^BBB%^"), "\e[38;5;59m"),
        assert_equal(testOb->parse("%^CCC%^"), "\e[38;5;102m"),
        assert_equal(testOb->parse("%^DDD%^"), "\e[38;5;145m"),
        assert_equal(testOb->parse("%^EEE%^"), "\e[38;5;188m"),
        assert_equal(testOb->parse("%^FFF%^"), "\e[38;5;231m"),
    }) :));
}