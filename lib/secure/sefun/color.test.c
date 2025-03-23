inherit M_TEST;

/**
 * @var {"/secure/sefun/color"} testOb
 */

void test_strip_colour () {
    string text = "%^BOLD%^Text%^RESET%^";

    expect("strip_colour removes ANSI resets", (: ({
        assert_equal(strlen($(text)), 21),
        assert_equal(testOb->strip_colour($(text)), "Text"),
        assert_equal(strlen(testOb->strip_colour($(text))), 4),
        assert_equal(strlen(testOb->strip_colour("%^RESET%^%^RESET%^RESET%^")), 0),

        assert_equal(testOb->strip_colour(0), 0),
    }) :));
}

private int *Color;
void test_random_color () {
    Color = testOb->query_random_color();
    expect("query_random_color returns random triplets", (: ({
        assert_equal(sizeof(Color), 3),
        assert_equal(Color[0] >= 0 && Color[0] <= 255, 1),
        assert_equal(Color[1] >= 0 && Color[1] <= 255, 1),
        assert_equal(Color[2] >= 0 && Color[2] <= 255, 1),
    }) :));
}

void test_sRGB () {
    expect("color_to_sRGB behaves", (: ({
        assert_equal(testOb->color_to_sRGB(0.0), 0),
        assert_equal(testOb->color_to_sRGB(0.25), 137),
        assert_equal(testOb->color_to_sRGB(0.5), 188),
        assert_equal(testOb->color_to_sRGB(0.75), 225),
        assert_equal(testOb->color_to_sRGB(1.0), 255),
    }) :));
    expect("color_to_sRGB handles bad input", (: ({
        assert_catch((: testOb->color_to_sRGB(UNDEFINED) :), "*Bad argument 1 to color->color_to_sRGB\n"),
    }) :));

    expect("color_from_sRGB behaves", (: ({
        assert_equal(testOb->color_from_sRGB(0), 0.0),
        assert_equal(testOb->color_from_sRGB(137), 0.25),
        assert_equal(testOb->color_from_sRGB(188), 0.5),
        assert_equal(testOb->color_from_sRGB(225), 0.75),
        assert_equal(testOb->color_from_sRGB(255), 1.0),
    }) :));

    expect("color_from_sRGB handles bad input", (: ({
        assert_catch((: testOb->color_from_sRGB(UNDEFINED) :), "*Bad argument 1 to color->color_from_sRGB\n"),
    }) :));
}

void test_lerp () {
    expect("color_lerp behaves", (: ({
        assert_equal(testOb->color_lerp(0.0, 255.0, 0.0), 0.0),
        assert_equal(testOb->color_lerp(0.0, 255.0, 0.5), 127.5),
        assert_equal(testOb->color_lerp(0.0, 255.0, 1.0), 255.0),
    }) :));

    expect("color_lerp handles bad input", (: ({
        assert_catch((: testOb->color_lerp(0, 0, 0) :), "*Bad argument 1 to color->color_lerp\n"),

        assert_catch((: testOb->color_lerp(0.0, 0, 0) :), "*Bad argument 2 to color->color_lerp\n"),

        assert_catch((: testOb->color_lerp(0.0, 0.0, 0) :), "*Bad argument 3 to color->color_lerp\n"),
    }) :));
}

void test_color_gradient () {
    expect("color_gradient behaves", (: ({
        assert_equal(testOb->color_gradient(({ 0, 0, 0 }), ({ 255, 255, 255 }), 2), ({ "0;0;0", "255;255;255" })),
        assert_equal(testOb->color_gradient(({ 0, 0, 0 }), ({ 255, 255, 255 }), 5), ({  "0;0;0", "63;63;63", "127;127;127", "191;191;191", "255;255;255" })),
    }) :));

    expect("color_gradient handles bad input", (: ({
        assert_catch((: testOb->color_gradient(0, 0, UNDEFINED) :), "*Bad argument 1 to color->color_gradient\n"),

        assert_catch((: testOb->color_gradient(({ 0, 0, 0 }), 0, UNDEFINED) :), "*Bad argument 2 to color->color_gradient\n"),

        assert_catch((: testOb->color_gradient(({ 0, 0, 0 }), ({ 0, 0, 0 }), UNDEFINED) :), "*Bad argument 3 to color->color_gradient\n"),
    }) :));
}

void test_apply_gradient () {
    mixed *gradient1 = testOb->color_gradient(({ 255, 255, 255 }), ({ 0, 0, 0 }), 3);
    mixed *gradient2 = testOb->color_gradient(({ 0, 0, 0 }), ({ 255, 255, 255 }), 10);

    expect("apply_gradient behaves", (: ({
        assert_equal(testOb->apply_gradient("123", $(gradient1)), "\e[38;2;255;255;255m1\e[38;2;127;127;127m2\e[38;2;0;0;0m3\e[0;37;40m"),
        assert_equal(testOb->apply_gradient("1234567890", $(gradient2)), "\e[38;2;0;0;0m1\e[38;2;28;28;28m2\e[38;2;56;56;56m3\e[38;2;85;85;85m4\e[38;2;113;113;113m5\e[38;2;141;141;141m6\e[38;2;170;170;170m7\e[38;2;198;198;198m8\e[38;2;226;226;226m9\e[38;2;255;255;255m0\e[0;37;40m"),

        // smaller text than gradient works
        assert_equal(testOb->apply_gradient("123", $(gradient2)), "\e[38;2;0;0;0m1\e[38;2;28;28;28m2\e[38;2;56;56;56m3\e[0;37;40m"),
    }) :));
    expect("apply_gradient handles bad input", (: ({
        assert_catch((: testOb->apply_gradient("1234567890", $($(gradient1))) :), "*Bad arguments to color->apply_gradient: invalid sizes 10 vs 3\n"),
    }) :));
}

void test_format_message_color () {
    expect("format_message_color behaves", (: ({
        assert_equal(testOb->format_message_color("nothing", "Message."), "Message."),

        assert_equal(testOb->format_message_color("say", "Someone says: Something."), "%^CYAN%^Someone says:%^RESET%^ Something."),
        assert_equal(testOb->format_message_color("tell", "Someone tells you: Something."), "%^I_RED%^BOLD%^Someone tells you:%^RESET%^ Something."),
        assert_equal(testOb->format_message_color("go", "Someone goes %^DIR%^direction%^DEFAULT%^."), "%^I_GREEN%^BOLD%^Someone goes %^CYAN%^direction%^GREEN%^.%^RESET%^"),

        assert_equal(testOb->format_message_color("room listen", "Sounds."), "%^CYAN%^Sounds.%^RESET%^"),
        assert_equal(testOb->format_message_color("room smell", "Smells."), "%^ORANGE%^Smells.%^RESET%^"),
        assert_equal(testOb->format_message_color("room living contents", "%^I_RED%^BOLD%^Something%^DEFAULT%^, %^I_RED%^BOLD%^a different something%^DEFAULT%^, and %^I_RED%^BOLD%^something else%^DEFAULT%^."), "%^I_RED%^BOLD%^Something%^RED%^BOLD_OFF%^, %^I_RED%^BOLD%^a different something%^RED%^BOLD_OFF%^, and %^I_RED%^BOLD%^something else%^RED%^BOLD_OFF%^.%^RESET%^"),
        assert_equal(testOb->format_message_color("room item contents", "%^I_RED%^BOLD%^Something%^DEFAULT%^, %^I_RED%^BOLD%^a different something%^DEFAULT%^, and %^I_RED%^BOLD%^something else%^DEFAULT%^."), "%^I_RED%^BOLD%^Something%^MAGENTA%^BOLD_OFF%^, %^I_RED%^BOLD%^a different something%^MAGENTA%^BOLD_OFF%^, and %^I_RED%^BOLD%^something else%^MAGENTA%^BOLD_OFF%^.%^RESET%^"),
        assert_equal(testOb->format_message_color("room exits", "There are four exits: %^I_CYAN%^BOLD%^north%^BOLD_OFF%^DEFAULT%^, %^I_CYAN%^BOLD%^east%^BOLD_OFF%^DEFAULT%^, %^I_CYAN%^BOLD%^south%^BOLD_OFF%^DEFAULT%^, and west."), "%^GREEN%^There are four exits: %^I_CYAN%^BOLD%^north%^BOLD_OFF%^GREEN%^, %^I_CYAN%^BOLD%^east%^BOLD_OFF%^GREEN%^, %^I_CYAN%^BOLD%^south%^BOLD_OFF%^GREEN%^, and west.%^RESET%^"),

        assert_equal(testOb->format_message_color("channel", "Name [[channel]] Text."), "Name %^GREEN%^[%^I_GREEN%^channel%^GREEN%^]%^RESET%^ Text."),
        assert_equal(testOb->format_message_color("channel", "Name ((channel)) Text."), "Name %^CYAN%^(%^I_CYAN%^channel%^CYAN%^)%^RESET%^ Text."),
        assert_equal(testOb->format_message_color("channel error", "Channel ((eror)) Text."), "Channel %^CYAN%^(%^I_RED%^eror%^CYAN%^)%^RESET%^ Text."),

        assert_equal(testOb->format_message_color("attack", "Attack message!"), "%^I_RED%^BOLD%^Attack message!%^RESET%^"),
        assert_equal(testOb->format_message_color("combat hit", "Someone hits you."), "%^RED%^Someone hits you.%^RESET%^"),
        assert_equal(testOb->format_message_color("combat heal", "Someone heals you."), "%^I_CYAN%^Someone heals you.%^RESET%^"),
        assert_equal(testOb->format_message_color("combat miss", "Someone misses you."), "%^GREEN%^Someone misses you.%^RESET%^"),
        assert_equal(testOb->format_message_color("combat alert", "Combat alert!"), "%^I_YELLOW%^BOLD%^Combat alert!%^RESET%^"),
        assert_equal(testOb->format_message_color("ability hit", "Ability hit!"), "%^I_RED%^BOLD%^Ability hit!%^RESET%^"),
        assert_equal(testOb->format_message_color("ability miss", "Ability miss."), "%^I_WHITE%^BOLD%^Ability miss.%^RESET%^"),
    }) :));
}

void test_wrap_ansi () {
    expect("wrap_ansi behaves", (: ({
        assert_equal(testOb->wrap_ansi("1\n2\n3", 10), "1\e[0;37;40m         \n2\e[0;37;40m         \n3         "),

        assert_equal(
            testOb->wrap_ansi("01 02 03 04 05 06 07 08 09 10 11 12 %^GREEN%^BOLD%^13 14 15 16 17%^RESET%^ 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 \n\n%^A bunch of %^UNDERLINE%^text that %^RED%^has some colors%^UNDERLINE_OFF%^ %^BOLD%^and style%^BOLD_OFF%^ set into it that%^RESET%^ will %^GREEN%^wrap around to the next line and retain its color and%^RESET%^ padding.", 40),
            "01 02 03 04 05 06 07 08 09 10 11 12 \e[32m\e[1m13\e[0;37;40m  \n"+
            "\e[32m\e[1m14 15 16 17\e[0;37;40m 18 19 20 21 22 23 24 25 26  \e[0;37;40m\n"+
            "27 28 29 30 31 32 33 34 35 36 37 38 39  \e[0;37;40m\n"+
            "40 41 42 43 44 45 46 47 48 49 50 51 52  \e[0;37;40m\n"+
            "53 54 55 56 57 58 59 60                 \e[0;37;40m\n"+
            "                                        \n"+
            "A bunch of \e[4mtext that \e[31mhas some colors\e[24m \e[1mand\e[0;37;40m\n"+
            "\e[4m\e[31m\e[24m\e[1mstyle\e[22m set into it that\e[0;37;40m will \e[32mwrap around \e[0;37;40m\n"+
            "\e[32mto the next line and retain its color   \e[0;37;40m\n"+
            "\e[32mand\e[0;37;40m padding.                            "
        ),
    }) :));
}