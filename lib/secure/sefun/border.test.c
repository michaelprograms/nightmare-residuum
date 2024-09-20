inherit M_TEST;

object __MockCharacter;
object query_character () {
    return __MockCharacter;
}

private nosave string __ANSI = "on", __Screenreader = "off";
private nosave int __Width = 80;
mixed query_setting (string setting) {
    if (setting == "ansi") {
        return __ANSI;
    }
    if (setting == "width") {
        return __Width;
    }
    if (setting == "screenreader") {
        return __Screenreader;
    }
}

void test_character_colors () {
    __MockCharacter = new("/std/living.c");
    __MockCharacter->set_key_name("test"); // must be named test

    expect("colors are returned", (: ({
        assert_equal(testOb->query_character_border_colors(), ({ ({ 191, 63, 191 }), ({ 63, 191, 191 }) })),

        __MockCharacter->set_class("psionist"),
        assert_equal(testOb->query_character_border_colors(), ({ ({ 65, 105, 225 }), ({ 192, 192, 192 }) })),
    }) :));
}

void test_border_charset () {
    expect("border_charset is returned", (: ({
        assert_equal(testOb->query_border_charset()["h"],  "─"),

        __Screenreader = "on",
        assert_equal(testOb->query_border_charset()["h"], " "),
        __Screenreader = "off",
    }) :));
}

void test_border_item () {
    expect("border header is formatted", (: ({
        assert_equal(testOb->format_border_item(([ "header": ({ "Header1", "Header2" }), "columns": 2 ]), "256", "L", "R"), ({ "L  %^RESET%^BOLD%^UNDERLINE%^Header1%^RESET%^                             %^RESET%^BOLD%^UNDERLINE%^Header2%^RESET%^                               R" })),
        assert_equal(testOb->format_border_item(([ "header": ({ "A123456789", "B123456789", "C123456789", "D123456789", "E123456789", "F123456789", "G123456789", "H123456789", "I123456789", "J123456789", "K123456789", "L123456789" }), "columns": 10 ]), "256", "L", "R"), ({ "L  A123456B123456C123456D123456E123456F123456G123456H123456I123456J123456    R" })),
    }) :));
    expect("border item is formatted", (: ({
        assert_equal(__Width = 20, 20),
        assert_equal(testOb->format_border_item(([ "items": ({ "1", "2", "3", }) ]), "256", "L", "R"), ({  "L  1     2       R", "L  3             R" })),

        assert_equal(__Width = 40, 40),
        assert_equal(testOb->format_border_item(([ "items": ({ "1", "2", "3", }) ]), "256", "L", "R"), ({ "L  1               2                 R", "L  3                                 R" })),
    }) :));
}

void test_tree () {
    expect("tree behaves", (: ({
        assert_equal(testOb->tree(([ "00": "abcdefghi", "01": "abcdefghi" ])), ({ "0. 00abcdefghi", "1. 01abcdefghi" })),
        assert_equal(testOb->tree(([ "A": ([ "1": ([ "a": ([ ]), "b": ([ ]) ]), "2": ([ ]), ]), "B": ([ ]), "C": ([ "1": ([ ]), "2": ([ "a": ([ ]), "b": ([ ]) ]) ]) ]), ), ({  "0. A", "├─0. 1", "│ ├─0. a", "│ └─1. b", "└─1. 2", "1. B", "2. C", "├─0. 1", "└─1. 2", "  ├─0. a", "  └─1. b" })),

        assert_catch((: testOb->tree(UNDEFINED) :), "*Bad argument 1 to border->tree\n"),
    }) :));
}