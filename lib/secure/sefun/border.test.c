inherit M_TEST;

object __MockCharacter;
object query_character () {
    return __MockCharacter;
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