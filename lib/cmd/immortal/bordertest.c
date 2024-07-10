void command (string input, mapping flags) {

    border(([
    ]));

    border(([
        "title": "TITLE",
    ]));

    border(([
        "title": "TITLE",
        "subtitle": "Subtitle",
    ]));

    border(([
        "title": "TITLE",
        "subtitle": "%^I_RED%^BOLD%^Subtitle%^RESET%^",
    ]));

    border(([
        "title": "TITLE",
        "subtitle": "Subtitle",
        "header": ([
            "header": "Header Header",
            "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", "Header Item 4", }),
        ]),
    ]));

    border(([
        "header": ([
            "header": "Header Header",
            "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", "Header Item 4", }),
        ]),
        "body": ([
            "header": "Body Header",
            "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", "Body Item 4", }),
        ]),
        "footer": ([
            "header": "Footer Header",
            "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", "Footer Item 4", }),
        ]),
    ]));

    border(([
        "header": ([
            "header": ({ "Header Header 1", "Header Header 2", "Header Header 3", "Header Header 4", }),
            "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", "Header Item 4", }),
            "columns": 4,
        ]),
        "body": ([
            "header": ({ "Body Header 1", "Body Header 2", "Body Header 3", "Body Header 4", }),
            "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", "Body Item 4", }),
            "columns": 4,
        ]),
        "footer": ([
            "header": ({ "Footer Header 1", "Footer Header 2", "Footer Header 3", "Footer Header 4", }),
            "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", "Footer Item 4", }),
            "columns": 4,
        ]),
    ]));

    border(([
        "header": ([
            "header": ({ "Header Header 1", "Header Header 2", "Header Header 3", "Header Header 4", }),
            "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", "Header Item 4", }),
            "columns": 4,
            "align": "center",
        ]),
        "body": ([
            "header": ({ "Body Header 1", "Body Header 2", "Body Header 3", "Body Header 4", }),
            "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", "Body Item 4", }),
            "columns": 4,
            "align": "center",
        ]),
        "footer": ([
            "header": ({ "Footer Header 1", "Footer Header 2", "Footer Header 3", "Footer Header 4", }),
            "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", "Footer Item 4", }),
            "columns": 4,
            "align": "center",
        ]),
    ]));

    border(([
        "body": ([
            "items": ({
                "Test test test test test test test test test test test test test test test.",
                "%^UNDERLINE%^Test test test test test test test test test test test test test test test.%^RESET%^",
                "%^CYAN%^Test test test test test test test test test test test test test test test.%^RESET%^",
                "",
                "Test. %^CYAN%^<test1>%^RESET%^" + format_syntax("<test2>"),
            }),
            "align": "left",
            "columns": 1,
        ]),
    ]));

    border(([
        "body": ([
            "items": ({
                "Test.",
            }),
            "align": "left",
            "columns": 1,
        ]),
        "ansi": 1,
    ]));

    border(([
        "header": ([
            "header": ({ "Header Header 1", "Header Header 2", "Header Header 3", }),
            "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", }),
            "columns": ({ 3, 2, 1, }),
            "align": "left",
        ]),
        "body": ([
            "header": ({ "Body Header 1", "Body Header 2", "Body Header 3", }),
            "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", }),
            "columns": ({ 3, 2, 1, }),
            "align": "left",
        ]),
        "footer": ([
            "header": ({ "Footer Header 1", "Footer Header 2", "Footer Header 3", }),
            "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", }),
            "columns": ({ 3, 2, 1, }),
            "align": "left",
        ]),
    ]));

    border(([
        "title": "TITLE",
        "subtitle": "Subtitle",
        "header": ({
            ([
                "header": ({ "Header Header 1", "Header Header 2", "Header Header 3", }),
                "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", }),
                "columns": ({ 3, 2, 1, }),
                "align": "left",
            ]),
            ([
                "header": ({ "Header Header 1", "Header Header 2", "Header Header 3", }),
                "items": ({ "Header Item 1", "Header Item 2", "Header Item 3", }),
                "columns": ({ 3, 2, 1, }),
                "align": "left",
            ]),
        }),
        "body": ([
            "header": ({ "Body Header 1", "Body Header 2", "Body Header 3", }),
            "items": ({ "Body Item 1", "Body Item 2", "Body Item 3", }),
            "columns": ({ 3, 2, 1, }),
            "align": "left",
        ]),
        "footer": ({
            ([
                "header": ({ "Footer Header 1", "Footer Header 2", "Footer Header 3", }),
                "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", }),
                "columns": ({ 3, 2, 1, }),
                "align": "left",
            ]),
            ([
                "header": ({ "Footer Header 1", "Footer Header 2", "Footer Header 3", }),
                "items": ({ "Footer Item 1", "Footer Item 2", "Footer Item 3", }),
                "columns": ({ 3, 2, 1, }),
                "align": "left",
            ]),
        }),
    ]));
}