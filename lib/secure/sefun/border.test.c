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

void test_format_border_item () {
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

void test_format_border () {
    mapping b = testOb->query_border_charset();
    mapping dataTitle = ([
        "title": "TITLE",
        "subtitle": "subtitle"
    ]);
    mapping dataHeader = ([
        "header": ({
            ([
                "header": ({ "Header Header 1", "Header Header 2" }),
                "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", }),
            ]),
            ([
                "header": ({ "Header Header 1", "Header Header 2" }),
                "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", }),
            ]),
        })
    ]);
    mapping dataBody = ([
        "body": ({
            ([
                "header": ({ "Body Header 1", "Body Header 2" }),
                "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", }),
            ]),
            ([
                "header": ({ "Body Header 1", "Body Header 2" }),
                "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", }),
            ]),
        }),
    ]);
    mapping dataFooter = ([
        "footer": ({
            ([
                "header": ({ "Footer Header 1", "Footer Header 2" }),
                "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", }),
            ]),
            ([
                "header": ({ "Footer Header 1", "Footer Header 2" }),
                "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", }),
            ]),
        }),
    ]);

    expect("border is formatted", (: ({
        assert_equal(__Width, 40),

        // Empty
        assert_equal(testOb->format_border(([]), $(b), __Width, 0), ({
            "╭──────────────────────────────────────╮",
            "╰──────────────────────────────────────╯"
        })),
        assert_equal(testOb->format_border(([]), $(b), __Width, "16"), ({
            "\e[36m╭──────────────────────────────────────╮\e[0;37;40m",
            "\e[36m╰──────────────────────────────────────╯\e[0;37;40m"
        })),
        assert_equal(testOb->format_border(([]), $(b), __Width, "256"), ({
            "\e[38;2;64;106;229m╭\e[38;2;71;109;227m─\e[38;2;78;112;225m─\e[38;2;83;115;223m─\e[38;2;88;117;221m─\e[38;2;93;120;219m─\e[38;2;97;122;218m─\e[38;2;101;124;216m─\e[38;2;105;126;214m─\e[38;2;109;128;213m─\e[38;2;112;130;211m─\e[38;2;115;132;210m─\e[38;2;118;134;208m─\e[38;2;121;136;206m─\e[38;2;124;138;205m─\e[38;2;127;140;203m─\e[38;2;129;141;202m─\e[38;2;132;143;200m─\e[38;2;134;144;199m─\e[38;2;136;146;197m─\e[38;2;138;148;196m─\e[38;2;141;149;195m─\e[38;2;143;150;193m─\e[38;2;145;152;192m─\e[38;2;147;153;190m─\e[38;2;149;155;189m─\e[38;2;150;156;188m─\e[38;2;152;157;186m─\e[38;2;154;158;185m─\e[38;2;156;160;184m─\e[38;2;157;161;182m─\e[38;2;159;162;181m─\e[38;2;161;163;180m─\e[38;2;162;165;179m─\e[38;2;164;166;177m─\e[38;2;165;167;176m─\e[38;2;167;168;175m─\e[38;2;168;169;174m─\e[38;2;170;170;172m─\e[38;2;171;171;171m╮\e[0;37;40m",
            "\e[38;2;171;171;171m╰\e[38;2;170;170;172m─\e[38;2;168;169;174m─\e[38;2;167;168;175m─\e[38;2;165;167;176m─\e[38;2;164;166;177m─\e[38;2;162;165;179m─\e[38;2;161;163;180m─\e[38;2;159;162;181m─\e[38;2;157;161;182m─\e[38;2;156;160;184m─\e[38;2;154;158;185m─\e[38;2;152;157;186m─\e[38;2;150;156;188m─\e[38;2;149;155;189m─\e[38;2;147;153;190m─\e[38;2;145;152;192m─\e[38;2;143;150;193m─\e[38;2;141;149;195m─\e[38;2;138;148;196m─\e[38;2;136;146;197m─\e[38;2;134;144;199m─\e[38;2;132;143;200m─\e[38;2;129;141;202m─\e[38;2;127;140;203m─\e[38;2;124;138;205m─\e[38;2;121;136;206m─\e[38;2;118;134;208m─\e[38;2;115;132;210m─\e[38;2;112;130;211m─\e[38;2;109;128;213m─\e[38;2;105;126;214m─\e[38;2;101;124;216m─\e[38;2;97;122;218m─\e[38;2;93;120;219m─\e[38;2;88;117;221m─\e[38;2;83;115;223m─\e[38;2;78;112;225m─\e[38;2;71;109;227m─\e[38;2;64;106;229m╯\e[0;37;40m"
        })),

        // Title
        assert_equal(testOb->format_border($(dataTitle), $(b), __Width, 0), ({
            "   ╭─────────────────╮",
            "╭──┤ TITLE: subtitle ├─────────────────╮",
            "│  ╰─────────────────╯                 │",
            "╰──────────────────────────────────────╯"
        })),
        assert_equal(testOb->format_border($(dataTitle), $(b), __Width, "16"), ({
            "   \e[36m╭─────────────────╮\e[0;37;40m",
            "\e[36m╭──┤ \e[0;37;40;1mTITLE: subtitle\e[22;36m ├─────────────────╮\e[0;37;40m",
            "\e[36m│\e[0;37;40m  \e[36m╰─────────────────╯\e[0;37;40m                 \e[36m│\e[0;37;40m",
            "\e[36m╰──────────────────────────────────────╯\e[0;37;40m"
        })),
        assert_equal(testOb->format_border($(dataTitle), $(b), __Width, "256"), ({
            "   \e[38;2;64;106;229m╭\e[38;2;71;109;227m─\e[38;2;78;112;225m─\e[38;2;83;115;223m─\e[38;2;88;117;221m─\e[38;2;93;120;219m─\e[38;2;97;122;218m─\e[38;2;101;124;216m─\e[38;2;105;126;214m─\e[38;2;109;128;213m─\e[38;2;112;130;211m─\e[38;2;115;132;210m─\e[38;2;118;134;208m─\e[38;2;121;136;206m─\e[38;2;124;138;205m─\e[38;2;127;140;203m─\e[38;2;129;141;202m─\e[38;2;132;143;200m─\e[38;2;134;144;199m╮\e[0;37;40m",
            "\e[38;2;64;106;229m╭\e[38;2;71;109;227m─\e[38;2;78;112;225m─\e[38;2;83;115;223m┤\e[0;37;40m\e[0;37;40;1m TITLE:\e[22m subtitle \e[38;2;141;149;195m├\e[38;2;143;150;193m─\e[38;2;145;152;192m─\e[38;2;147;153;190m─\e[38;2;149;155;189m─\e[38;2;150;156;188m─\e[38;2;152;157;186m─\e[38;2;154;158;185m─\e[38;2;156;160;184m─\e[38;2;157;161;182m─\e[38;2;159;162;181m─\e[38;2;161;163;180m─\e[38;2;162;165;179m─\e[38;2;164;166;177m─\e[38;2;165;167;176m─\e[38;2;167;168;175m─\e[38;2;168;169;174m─\e[38;2;170;170;172m─\e[38;2;171;171;171m╮\e[0;37;40m",
            "\e[38;2;64;106;229m│\e[0;37;40m  \e[38;2;83;115;223m╰\e[38;2;88;117;221m─\e[38;2;93;120;219m─\e[38;2;97;122;218m─\e[38;2;101;124;216m─\e[38;2;105;126;214m─\e[38;2;109;128;213m─\e[38;2;112;130;211m─\e[38;2;115;132;210m─\e[38;2;118;134;208m─\e[38;2;121;136;206m─\e[38;2;124;138;205m─\e[38;2;127;140;203m─\e[38;2;129;141;202m─\e[38;2;132;143;200m─\e[38;2;134;144;199m─\e[38;2;136;146;197m─\e[38;2;138;148;196m─\e[38;2;141;149;195m╯\e[0;37;40m                 0",
            "\e[38;2;171;171;171m╰\e[38;2;170;170;172m─\e[38;2;168;169;174m─\e[38;2;167;168;175m─\e[38;2;165;167;176m─\e[38;2;164;166;177m─\e[38;2;162;165;179m─\e[38;2;161;163;180m─\e[38;2;159;162;181m─\e[38;2;157;161;182m─\e[38;2;156;160;184m─\e[38;2;154;158;185m─\e[38;2;152;157;186m─\e[38;2;150;156;188m─\e[38;2;149;155;189m─\e[38;2;147;153;190m─\e[38;2;145;152;192m─\e[38;2;143;150;193m─\e[38;2;141;149;195m─\e[38;2;138;148;196m─\e[38;2;136;146;197m─\e[38;2;134;144;199m─\e[38;2;132;143;200m─\e[38;2;129;141;202m─\e[38;2;127;140;203m─\e[38;2;124;138;205m─\e[38;2;121;136;206m─\e[38;2;118;134;208m─\e[38;2;115;132;210m─\e[38;2;112;130;211m─\e[38;2;109;128;213m─\e[38;2;105;126;214m─\e[38;2;101;124;216m─\e[38;2;97;122;218m─\e[38;2;93;120;219m─\e[38;2;88;117;221m─\e[38;2;83;115;223m─\e[38;2;78;112;225m─\e[38;2;71;109;227m─\e[38;2;64;106;229m╯\e[0;37;40m"
        })),

        // Header
        assert_equal(testOb->format_border(([ "header": $(dataHeader)["header"][0] ]), $(b), __Width, 0), ({
            "╭┬────────────────────────────────────┬╮",
            "││  Header Header 1 Header Header 2   ││",
            "││  Header Item 1   Header Item 2     ││",
            "││  Header Item 3                     ││",
            "│╰────────────────────────────────────╯│",
            "╰──────────────────────────────────────╯"
        })),
        assert_equal(testOb->format_border($(dataHeader), $(b), __Width, 0), ({
            "╭┬────────────────────────────────────┬╮",
            "││  Header Header 1 Header Header 2   ││",
            "││  Header Item 1   Header Item 2     ││",
            "││  Header Item 3                     ││",
            "││                                    ││",
            "││  Header Header 1 Header Header 2   ││",
            "││  Header Item 1   Header Item 2     ││",
            "││  Header Item 3                     ││",
            "│╰────────────────────────────────────╯│",
            "╰──────────────────────────────────────╯"
        })),
        assert_equal(testOb->format_border($(dataHeader), $(b), __Width, "16"), ({
            "\e[36m╭┬────────────────────────────────────┬╮\e[0;37;40m",
            "\e[36m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Header Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Header Header 2%^RESET%^   \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Header Item 1   Header Item 2     \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Header Item 3                     \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m                                    \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Header Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Header Header 2%^RESET%^   \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Header Item 1   Header Item 2     \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Header Item 3                     \e[36m││\e[0;37;40m",
            "\e[36m│╰────────────────────────────────────╯│\e[0;37;40m",
            "\e[36m╰──────────────────────────────────────╯\e[0;37;40m"
        })),
        assert_equal(testOb->format_border($(dataHeader), $(b), __Width, "256"), ({
            "\e[38;2;64;106;229m╭\e[38;2;71;109;227m┬\e[38;2;78;112;225m─\e[38;2;83;115;223m─\e[38;2;88;117;221m─\e[38;2;93;120;219m─\e[38;2;97;122;218m─\e[38;2;101;124;216m─\e[38;2;105;126;214m─\e[38;2;109;128;213m─\e[38;2;112;130;211m─\e[38;2;115;132;210m─\e[38;2;118;134;208m─\e[38;2;121;136;206m─\e[38;2;124;138;205m─\e[38;2;127;140;203m─\e[38;2;129;141;202m─\e[38;2;132;143;200m─\e[38;2;134;144;199m─\e[38;2;136;146;197m─\e[38;2;138;148;196m─\e[38;2;141;149;195m─\e[38;2;143;150;193m─\e[38;2;145;152;192m─\e[38;2;147;153;190m─\e[38;2;149;155;189m─\e[38;2;150;156;188m─\e[38;2;152;157;186m─\e[38;2;154;158;185m─\e[38;2;156;160;184m─\e[38;2;157;161;182m─\e[38;2;159;162;181m─\e[38;2;161;163;180m─\e[38;2;162;165;179m─\e[38;2;164;166;177m─\e[38;2;165;167;176m─\e[38;2;167;168;175m─\e[38;2;168;169;174m─\e[38;2;170;170;172m┬\e[38;2;171;171;171m╮\e[0;37;40m",
            "\e[38;2;90;118;221m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Header Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Header Header 2%^RESET%^   \e[38;2;165;166;177m││\e[0;37;40m",
            "\e[38;2;107;128;213m││\e[0;37;40m  Header Item 1   Header Item 2     \e[38;2;158;161;182m││\e[0;37;40m",
            "\e[38;2;121;136;206m││\e[0;37;40m  Header Item 3                     \e[38;2;150;156;188m││\e[0;37;40m",
            "\e[38;2;132;143;200m││\e[0;37;40m                                    \e[38;2;142;150;194m││\e[0;37;40m",
            "\e[38;2;142;150;194m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Header Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Header Header 2%^RESET%^   \e[38;2;132;143;200m││\e[0;37;40m",
            "\e[38;2;150;156;188m││\e[0;37;40m  Header Item 1   Header Item 2     \e[38;2;121;136;206m││\e[0;37;40m",
            "\e[38;2;158;161;182m││\e[0;37;40m  Header Item 3                     \e[38;2;107;128;213m││\e[0;37;40m",
            "\e[38;2;158;158;169m│\e[38;2;157;157;171m╰\e[38;2;156;157;172m─\e[38;2;155;156;173m─\e[38;2;153;155;175m─\e[38;2;152;154;176m─\e[38;2;151;153;178m─\e[38;2;149;152;179m─\e[38;2;148;152;180m─\e[38;2;147;151;182m─\e[38;2;145;150;183m─\e[38;2;144;149;184m─\e[38;2;143;148;186m─\e[38;2;141;147;187m─\e[38;2;140;146;188m─\e[38;2;138;145;190m─\e[38;2;137;144;191m─\e[38;2;135;143;192m─\e[38;2;133;142;194m─\e[38;2;132;141;195m─\e[38;2;130;140;196m─\e[38;2;129;139;198m─\e[38;2;127;138;199m─\e[38;2;125;137;200m─\e[38;2;123;136;202m─\e[38;2;121;135;203m─\e[38;2;119;134;204m─\e[38;2;118;133;206m─\e[38;2;116;132;207m─\e[38;2;113;130;208m─\e[38;2;111;129;209m─\e[38;2;109;128;211m─\e[38;2;107;127;212m─\e[38;2;105;126;213m─\e[38;2;102;124;215m─\e[38;2;100;123;216m─\e[38;2;97;122;217m─\e[38;2;95;120;218m─\e[38;2;92;119;220m╯\e[38;2;89;118;221m│\e[0;37;40m",
            "\e[38;2;171;171;171m╰\e[38;2;170;170;172m─\e[38;2;168;169;174m─\e[38;2;167;168;175m─\e[38;2;165;167;176m─\e[38;2;164;166;177m─\e[38;2;162;165;179m─\e[38;2;161;163;180m─\e[38;2;159;162;181m─\e[38;2;157;161;182m─\e[38;2;156;160;184m─\e[38;2;154;158;185m─\e[38;2;152;157;186m─\e[38;2;150;156;188m─\e[38;2;149;155;189m─\e[38;2;147;153;190m─\e[38;2;145;152;192m─\e[38;2;143;150;193m─\e[38;2;141;149;195m─\e[38;2;138;148;196m─\e[38;2;136;146;197m─\e[38;2;134;144;199m─\e[38;2;132;143;200m─\e[38;2;129;141;202m─\e[38;2;127;140;203m─\e[38;2;124;138;205m─\e[38;2;121;136;206m─\e[38;2;118;134;208m─\e[38;2;115;132;210m─\e[38;2;112;130;211m─\e[38;2;109;128;213m─\e[38;2;105;126;214m─\e[38;2;101;124;216m─\e[38;2;97;122;218m─\e[38;2;93;120;219m─\e[38;2;88;117;221m─\e[38;2;83;115;223m─\e[38;2;78;112;225m─\e[38;2;71;109;227m─\e[38;2;64;106;229m╯\e[0;37;40m"
        })),

        // Body
        assert_equal(testOb->format_border(([ "body": $(dataBody)["body"][0] ]), $(b), __Width, 0), ({
            "╭──────────────────────────────────────╮",
            "│                                      │",
            "│   Body Header 1   Body Header 2      │",
            "│   Body Item 1     Body Item 2        │",
            "│   Body Item 3                        │",
            "│                                      │",
            "╰──────────────────────────────────────╯"
        })),
        assert_equal(testOb->format_border($(dataBody), $(b), __Width, 0), ({
            "╭──────────────────────────────────────╮",
            "│                                      │",
            "│   Body Header 1   Body Header 2      │",
            "│   Body Item 1     Body Item 2        │",
            "│   Body Item 3                        │",
            "│                                      │",
            "│   Body Header 1   Body Header 2      │",
            "│   Body Item 1     Body Item 2        │",
            "│   Body Item 3                        │",
            "│                                      │",
            "╰──────────────────────────────────────╯"
        })),
        assert_equal(testOb->format_border($(dataBody), $(b), __Width, "16"), ({
            "\e[36m╭──────────────────────────────────────╮\e[0;37;40m",
            "\e[36m│ \e[0;37;40m                                    \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Body Header 1%^RESET%^   %^RESET%^BOLD%^UNDERLINE%^Body Header 2%^RESET%^     \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m  Body Item 1     Body Item 2       \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m  Body Item 3                       \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m                                    \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Body Header 1%^RESET%^   %^RESET%^BOLD%^UNDERLINE%^Body Header 2%^RESET%^     \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m  Body Item 1     Body Item 2       \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m  Body Item 3                       \e[36m │\e[0;37;40m",
            "\e[36m│ \e[0;37;40m                                    \e[36m │\e[0;37;40m",
            "\e[36m╰──────────────────────────────────────╯\e[0;37;40m"
        })),
        assert_equal(testOb->format_border($(dataBody), $(b), __Width, "256"), ({
            "\e[38;2;64;106;229m╭\e[38;2;71;109;227m─\e[38;2;78;112;225m─\e[38;2;83;115;223m─\e[38;2;88;117;221m─\e[38;2;93;120;219m─\e[38;2;97;122;218m─\e[38;2;101;124;216m─\e[38;2;105;126;214m─\e[38;2;109;128;213m─\e[38;2;112;130;211m─\e[38;2;115;132;210m─\e[38;2;118;134;208m─\e[38;2;121;136;206m─\e[38;2;124;138;205m─\e[38;2;127;140;203m─\e[38;2;129;141;202m─\e[38;2;132;143;200m─\e[38;2;134;144;199m─\e[38;2;136;146;197m─\e[38;2;138;148;196m─\e[38;2;141;149;195m─\e[38;2;143;150;193m─\e[38;2;145;152;192m─\e[38;2;147;153;190m─\e[38;2;149;155;189m─\e[38;2;150;156;188m─\e[38;2;152;157;186m─\e[38;2;154;158;185m─\e[38;2;156;160;184m─\e[38;2;157;161;182m─\e[38;2;159;162;181m─\e[38;2;161;163;180m─\e[38;2;162;165;179m─\e[38;2;164;166;177m─\e[38;2;165;167;176m─\e[38;2;167;168;175m─\e[38;2;168;169;174m─\e[38;2;170;170;172m─\e[38;2;171;171;171m╮\e[0;37;40m",
            "\e[38;2;88;117;221m│\e[0;37;40m                                      \e[38;2;165;167;176m│\e[0;37;40m",
            "\e[38;2;104;126;215m│\e[0;37;40m   %^RESET%^BOLD%^UNDERLINE%^Body Header 1%^RESET%^   %^RESET%^BOLD%^UNDERLINE%^Body Header 2%^RESET%^      \e[38;2;159;162;181m│\e[0;37;40m",
            "\e[38;2;117;134;208m│\e[0;37;40m   Body Item 1     Body Item 2        \e[38;2;153;158;186m│\e[0;37;40m",
            "\e[38;2;128;141;202m│\e[0;37;40m   Body Item 3                        \e[38;2;146;152;191m│\e[0;37;40m",
            "\e[38;2;137;147;197m│\e[0;37;40m                                      \e[38;2;137;147;197m│\e[0;37;40m",
            "\e[38;2;146;152;191m│\e[0;37;40m   %^RESET%^BOLD%^UNDERLINE%^Body Header 1%^RESET%^   %^RESET%^BOLD%^UNDERLINE%^Body Header 2%^RESET%^      \e[38;2;128;141;202m│\e[0;37;40m",
            "\e[38;2;153;158;186m│\e[0;37;40m   Body Item 1     Body Item 2        \e[38;2;117;134;208m│\e[0;37;40m",
            "\e[38;2;159;162;181m│\e[0;37;40m   Body Item 3                        \e[38;2;104;126;215m│\e[0;37;40m",
            "\e[38;2;165;167;176m│\e[0;37;40m                                      \e[38;2;88;117;221m│\e[0;37;40m",
            "\e[38;2;171;171;171m╰\e[38;2;170;170;172m─\e[38;2;168;169;174m─\e[38;2;167;168;175m─\e[38;2;165;167;176m─\e[38;2;164;166;177m─\e[38;2;162;165;179m─\e[38;2;161;163;180m─\e[38;2;159;162;181m─\e[38;2;157;161;182m─\e[38;2;156;160;184m─\e[38;2;154;158;185m─\e[38;2;152;157;186m─\e[38;2;150;156;188m─\e[38;2;149;155;189m─\e[38;2;147;153;190m─\e[38;2;145;152;192m─\e[38;2;143;150;193m─\e[38;2;141;149;195m─\e[38;2;138;148;196m─\e[38;2;136;146;197m─\e[38;2;134;144;199m─\e[38;2;132;143;200m─\e[38;2;129;141;202m─\e[38;2;127;140;203m─\e[38;2;124;138;205m─\e[38;2;121;136;206m─\e[38;2;118;134;208m─\e[38;2;115;132;210m─\e[38;2;112;130;211m─\e[38;2;109;128;213m─\e[38;2;105;126;214m─\e[38;2;101;124;216m─\e[38;2;97;122;218m─\e[38;2;93;120;219m─\e[38;2;88;117;221m─\e[38;2;83;115;223m─\e[38;2;78;112;225m─\e[38;2;71;109;227m─\e[38;2;64;106;229m╯\e[0;37;40m"
        })),

        // Footer
        assert_equal(testOb->format_border(([ "footer": $(dataFooter)["footer"][0] ]), $(b), __Width, 0), ({
            "╭──────────────────────────────────────╮",
            "│╭────────────────────────────────────╮│",
            "││  Footer Header 1 Footer Header 2   ││",
            "││  Footer Item 1   Footer Item 2     ││",
            "││  Footer Item 3                     ││",
            "╰┴────────────────────────────────────┴╯"
        })),
        assert_equal(testOb->format_border($(dataFooter), $(b), __Width, 0), ({
            "╭──────────────────────────────────────╮",
            "│╭────────────────────────────────────╮│",
            "││  Footer Header 1 Footer Header 2   ││",
            "││  Footer Item 1   Footer Item 2     ││",
            "││  Footer Item 3                     ││",
            "││                                    ││",
            "││  Footer Header 1 Footer Header 2   ││",
            "││  Footer Item 1   Footer Item 2     ││",
            "││  Footer Item 3                     ││",
            "╰┴────────────────────────────────────┴╯"
        })),
        assert_equal(testOb->format_border($(dataFooter), $(b), __Width, "16"), ({
            "\e[36m╭──────────────────────────────────────╮\e[0;37;40m",
            "\e[36m│╭────────────────────────────────────╮│\e[0;37;40m",
            "\e[36m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Footer Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Footer Header 2%^RESET%^   \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Footer Item 1   Footer Item 2     \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Footer Item 3                     \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m                                    \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Footer Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Footer Header 2%^RESET%^   \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Footer Item 1   Footer Item 2     \e[36m││\e[0;37;40m",
            "\e[36m││\e[0;37;40m  Footer Item 3                     \e[36m││\e[0;37;40m",
            "\e[36m╰┴────────────────────────────────────┴╯\e[0;37;40m"
        })),
        assert_equal(testOb->format_border($(dataFooter), $(b), __Width, "256"), ({
            "\e[38;2;64;106;229m╭\e[38;2;71;109;227m─\e[38;2;78;112;225m─\e[38;2;83;115;223m─\e[38;2;88;117;221m─\e[38;2;93;120;219m─\e[38;2;97;122;218m─\e[38;2;101;124;216m─\e[38;2;105;126;214m─\e[38;2;109;128;213m─\e[38;2;112;130;211m─\e[38;2;115;132;210m─\e[38;2;118;134;208m─\e[38;2;121;136;206m─\e[38;2;124;138;205m─\e[38;2;127;140;203m─\e[38;2;129;141;202m─\e[38;2;132;143;200m─\e[38;2;134;144;199m─\e[38;2;136;146;197m─\e[38;2;138;148;196m─\e[38;2;141;149;195m─\e[38;2;143;150;193m─\e[38;2;145;152;192m─\e[38;2;147;153;190m─\e[38;2;149;155;189m─\e[38;2;150;156;188m─\e[38;2;152;157;186m─\e[38;2;154;158;185m─\e[38;2;156;160;184m─\e[38;2;157;161;182m─\e[38;2;159;162;181m─\e[38;2;161;163;180m─\e[38;2;162;165;179m─\e[38;2;164;166;177m─\e[38;2;165;167;176m─\e[38;2;167;168;175m─\e[38;2;168;169;174m─\e[38;2;170;170;172m─\e[38;2;171;171;171m╮\e[0;37;40m",
            "\e[38;2;89;118;221m│\e[38;2;92;119;220m╭\e[38;2;95;120;218m─\e[38;2;97;122;217m─\e[38;2;100;123;216m─\e[38;2;102;124;215m─\e[38;2;105;126;213m─\e[38;2;107;127;212m─\e[38;2;109;128;211m─\e[38;2;111;129;209m─\e[38;2;113;130;208m─\e[38;2;116;132;207m─\e[38;2;118;133;206m─\e[38;2;119;134;204m─\e[38;2;121;135;203m─\e[38;2;123;136;202m─\e[38;2;125;137;200m─\e[38;2;127;138;199m─\e[38;2;129;139;198m─\e[38;2;130;140;196m─\e[38;2;132;141;195m─\e[38;2;133;142;194m─\e[38;2;135;143;192m─\e[38;2;137;144;191m─\e[38;2;138;145;190m─\e[38;2;140;146;188m─\e[38;2;141;147;187m─\e[38;2;143;148;186m─\e[38;2;144;149;184m─\e[38;2;145;150;183m─\e[38;2;147;151;182m─\e[38;2;148;152;180m─\e[38;2;149;152;179m─\e[38;2;151;153;178m─\e[38;2;152;154;176m─\e[38;2;153;155;175m─\e[38;2;155;156;173m─\e[38;2;156;157;172m─\e[38;2;157;157;171m╮\e[38;2;158;158;169m│\e[0;37;40m",
            "\e[38;2;107;128;213m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Footer Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Footer Header 2%^RESET%^   \e[38;2;158;161;182m││\e[0;37;40m",
            "\e[38;2;121;136;206m││\e[0;37;40m  Footer Item 1   Footer Item 2     \e[38;2;150;156;188m││\e[0;37;40m",
            "\e[38;2;132;143;200m││\e[0;37;40m  Footer Item 3                     \e[38;2;142;150;194m││\e[0;37;40m",
            "\e[38;2;142;150;194m││\e[0;37;40m                                    \e[38;2;132;143;200m││\e[0;37;40m",
            "\e[38;2;150;156;188m││\e[0;37;40m  %^RESET%^BOLD%^UNDERLINE%^Footer Header 1%^RESET%^ %^RESET%^BOLD%^UNDERLINE%^Footer Header 2%^RESET%^   \e[38;2;121;136;206m││\e[0;37;40m",
            "\e[38;2;158;161;182m││\e[0;37;40m  Footer Item 1   Footer Item 2     \e[38;2;107;128;213m││\e[0;37;40m",
            "\e[38;2;165;166;177m││\e[0;37;40m  Footer Item 3                     \e[38;2;90;118;221m││\e[0;37;40m",
            "\e[38;2;171;171;171m╰\e[38;2;170;170;172m┴\e[38;2;168;169;174m─\e[38;2;167;168;175m─\e[38;2;165;167;176m─\e[38;2;164;166;177m─\e[38;2;162;165;179m─\e[38;2;161;163;180m─\e[38;2;159;162;181m─\e[38;2;157;161;182m─\e[38;2;156;160;184m─\e[38;2;154;158;185m─\e[38;2;152;157;186m─\e[38;2;150;156;188m─\e[38;2;149;155;189m─\e[38;2;147;153;190m─\e[38;2;145;152;192m─\e[38;2;143;150;193m─\e[38;2;141;149;195m─\e[38;2;138;148;196m─\e[38;2;136;146;197m─\e[38;2;134;144;199m─\e[38;2;132;143;200m─\e[38;2;129;141;202m─\e[38;2;127;140;203m─\e[38;2;124;138;205m─\e[38;2;121;136;206m─\e[38;2;118;134;208m─\e[38;2;115;132;210m─\e[38;2;112;130;211m─\e[38;2;109;128;213m─\e[38;2;105;126;214m─\e[38;2;101;124;216m─\e[38;2;97;122;218m─\e[38;2;93;120;219m─\e[38;2;88;117;221m─\e[38;2;83;115;223m─\e[38;2;78;112;225m─\e[38;2;71;109;227m┴\e[38;2;64;106;229m╯\e[0;37;40m"
        })),

        // Border Colors
        assert_equal(testOb->format_border(([ "borderColors": ({ ({ 65, 105, 225 }), ({ 65, 105, 225 }) }) ]), $(b), __Width, "256"), ({
            "\e[38;2;64;106;229m╭\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m╮\e[0;37;40m",
            "\e[38;2;64;106;229m╰\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m─\e[38;2;64;106;229m╯\e[0;37;40m"

        })),
    }) :));
}

void test_tree () {
    expect("tree behaves", (: ({
        assert_equal(testOb->tree(([ "00": "abcdefghi", "01": "abcdefghi" ])), ({ "0. 00abcdefghi", "1. 01abcdefghi" })),
        assert_equal(testOb->tree(([ "A": ([ "1": ([ "a": ([ ]), "b": ([ ]) ]), "2": ([ ]), ]), "B": ([ ]), "C": ([ "1": ([ ]), "2": ([ "a": ([ ]), "b": ([ ]) ]) ]) ]), ), ({  "0. A", "├─0. 1", "│ ├─0. a", "│ └─1. b", "└─1. 2", "1. B", "2. C", "├─0. 1", "└─1. 2", "  ├─0. a", "  └─1. b" })),

        assert_catch((: testOb->tree(UNDEFINED) :), "*Bad argument 1 to border->tree\n"),
    }) :));
}