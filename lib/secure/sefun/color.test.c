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

void test_gradient () {
    expect("color_gradient behaves", (: ({
        assert_equal(testOb->color_gradient(({ 0, 0, 0 }), ({ 255, 255, 255 }), 2), ({ "0;0;0", "193;193;193" })),
        assert_equal(testOb->color_gradient(({ 0, 0, 0 }), ({ 255, 255, 255 }), 5), ({ "0;0;0", "102;102;102", "141;141;141", "170;170;170", "193;193;193" })),
    }) :));

    expect("color_gradient handles bad input", (: ({
        assert_catch((: testOb->color_gradient(0, 0, UNDEFINED) :), "*Bad argument 1 to color->color_gradient\n"),

        assert_catch((: testOb->color_gradient(({ 0, 0, 0 }), 0, UNDEFINED) :), "*Bad argument 2 to color->color_gradient\n"),

        assert_catch((: testOb->color_gradient(({ 0, 0, 0 }), ({ 0, 0, 0 }), UNDEFINED) :), "*Bad argument 3 to color->color_gradient\n"),
    }) :));
}

void test_format_message_color () {
    expect("", (: ({
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

        // TODO:
        // attack, combat hit, combat heal, combat miss, combat alert, ability hit, ability miss
    }) :));
}