inherit M_TEST;

/**
 * @var {"/std/user/gmcp"} testOb
 */

nosave private object __Mock;
nosave private int __VitalsUpdates;

// stands in for the character, see /std/character.c
void gmcp_update_vitals() {
    __VitalsUpdates++;
}

void before_each_test() {
    __Mock = new("/std/user/gmcp.mock.c");
    __Mock->start_shadow(testOb);
}
void after_each_test() {
    if (__Mock) {
        __Mock->stop_shadow();
        destruct(__Mock);
    }
}

void test_gmcp_enabled() {
    expect("gmcp_enabled requires the setting and a gmcp connection", (: ({
        // setting is off
        assert_equal(testOb->gmcp_enabled(), 0),

        // setting is on, but the test has no gmcp connection
        __Mock->set_setting("gmcp", "on"),
        assert_equal(testOb->gmcp_enabled(), 0),
    }) :));
}

void test_gmcp_send_update() {
    expect("gmcp_send_update does nothing when gmcp is disabled", (: ({
        assert_equal(testOb->gmcp_send_update("Room.Info", ([])), UNDEFINED),
    }) :));
}

void test_gmcp_vitals() {
    __VitalsUpdates = 0;

    expect("Char.Vitals.Get updates the character's vitals", (: ({
        // no character
        testOb->gmcp("Char.Vitals.Get"),
        assert_equal(__VitalsUpdates, 0),

        // character
        __Mock->set_character(this_object()),
        testOb->gmcp("Char.Vitals.Get"),
        assert_equal(__VitalsUpdates, 1),
    }) :));
}

void test_gmcp_room_info() {
    object item = new(STD_ITEM);

    expect("Room.Info.Get handles a character with and without a room", (: ({
        // no character
        assert_equal(testOb->gmcp("Room.Info.Get"), UNDEFINED),

        // character without an environment
        __Mock->set_character($(item)),
        assert_equal(testOb->gmcp("Room.Info.Get"), UNDEFINED),

        // character in a room
        assert_equal($(item)->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(testOb->gmcp("Room.Info.Get"), UNDEFINED),

        // unknown request
        assert_equal(testOb->gmcp("Unknown"), UNDEFINED),
    }) :));

    if (item) {
        destruct(item);
    }
}
