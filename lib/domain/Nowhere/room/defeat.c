inherit STD_ROOM;
inherit M_STORY;

void story_action_final (object target) {
    if (target->query_defeated()) {
        target->set_defeated(0);
    }
    target->handle_move("/domain/Origin/room/center.c");
    target->describe_environment();
}

int handle_receive (object ob) {
    int result = ::handle_receive(ob);
    if (result && ob->is_character()) {
        story_start(ob);
    }
    return result;
}

void create () {
    ::create();
    set_short("defeated");
    set_long("A heavy darkness that impedes all senses.");
    set_story_delay(15);
    set_story_lines(({
        "You begin to awaken from your incapacitated state.\n",
        "You recover from your defeat.\n",
    }));

}