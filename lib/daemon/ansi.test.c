inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/ansi.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

mapping term;
void test_ansi_terms () {
    int fail;

    term = testOb->query_ansi_term();
    expect("query_ansi_term has 541 items", (: ({
        assert(mapp(term), "==", 1),
        assert(sizeof(term), "==", 541)
    }) :));

    foreach (string key in keys(term)) {
        if (term[key] == "") {
            fail ++;
        }
    }
    expect("query_ansi_term has definitions", (: ({
        assert($(fail), "==", 0),
    }) :));
}

void test_unknown_terms () {
    int fail = 0;

    term = testOb->query_unknown_term();
    expect("query_unknown_term has 541 items", (: ({
        assert(mapp(term), "==", 1),
        assert(sizeof(term), "==", 541),
    }) :));

    foreach (string key in keys(term)) {
        if (term[key] != "") {
            fail ++;
        }
    }
    expect("query_unknown_term has no definitions", (: ({
        assert($(fail), "==", 0),
    }) :));
}

void test_keys () {
    expect("query_keys matches terms", (: ({
        assert(sizeof(testOb->query_keys()), "==", sizeof(testOb->query_ansi_term())),
        assert(sizeof(testOb->query_keys()), "==", sizeof(testOb->query_unknown_term())),
    }) :));
}

void test_parse () {
    expect("parse parses all basic colors", (: ({
        assert(testOb->parse("%^RESET%^"), "==", "\e[0m"),
        assert(testOb->parse("%^BOLD%^"), "==", "\e[1m"),
        assert(testOb->parse("%^ITALIC%^"), "==", "\e[3m"),
        assert(testOb->parse("%^UNDERLINE%^"), "==", "\e[4m"),
        assert(testOb->parse("%^INVERSE%^"), "==", "\e[7m"),
        assert(testOb->parse("%^STRIKE%^"), "==", "\e[9m"),
        assert(testOb->parse("%^BOLD_OFF%^"), "==", "\e[22m"),
        assert(testOb->parse("%^ITALIC_OFF%^"), "==", "\e[23m"),
        assert(testOb->parse("%^UNDERLINE_OFF%^"), "==", "\e[24m"),
        assert(testOb->parse("%^INVERSE_OFF%^"), "==", "\e[27m"),
        assert(testOb->parse("%^STRIKE_OFF%^"), "==", "\e[29m"),
        assert(testOb->parse("%^BLACK%^"), "==", "\e[30m"),
        assert(testOb->parse("%^RED%^"), "==", "\e[31m"),
        assert(testOb->parse("%^GREEN%^"), "==", "\e[32m"),
        assert(testOb->parse("%^ORANGE%^"), "==", "\e[33m"),
        assert(testOb->parse("%^YELLOW%^"), "==", "\e[1;33m"),
        assert(testOb->parse("%^BLUE%^"), "==", "\e[34m"),
        assert(testOb->parse("%^MAGENTA%^"), "==", "\e[35m"),
        assert(testOb->parse("%^CYAN%^"), "==", "\e[36m"),
        assert(testOb->parse("%^WHITE%^"), "==", "\e[37m"),
        assert(testOb->parse("%^B_BLACK%^"), "==", "\e[40m"),
        assert(testOb->parse("%^B_RED%^"), "==", "\e[41m"),
        assert(testOb->parse("%^B_GREEN%^"), "==", "\e[42m"),
        assert(testOb->parse("%^B_ORANGE%^"), "==", "\e[43m"),
        assert(testOb->parse("%^B_YELLOW%^"), "==", "\e[1;43m"),
        assert(testOb->parse("%^B_BLUE%^"), "==", "\e[44m"),
        assert(testOb->parse("%^B_MAGENTA%^"), "==", "\e[45m"),
        assert(testOb->parse("%^B_CYAN%^"), "==", "\e[46m"),
        assert(testOb->parse("%^B_WHITE%^"), "==", "\e[47m"),
    }) :));

    expect("parse parses xterm colors", (: ({
        assert(testOb->parse("%^AAA%^"), "==", "\e[38;5;16m"),
        assert(testOb->parse("%^BBB%^"), "==", "\e[38;5;59m"),
        assert(testOb->parse("%^CCC%^"), "==", "\e[38;5;102m"),
        assert(testOb->parse("%^DDD%^"), "==", "\e[38;5;145m"),
        assert(testOb->parse("%^EEE%^"), "==", "\e[38;5;188m"),
        assert(testOb->parse("%^FFF%^"), "==", "\e[38;5;231m"),
    }) :));
}