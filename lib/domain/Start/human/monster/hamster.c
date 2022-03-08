inherit STD_MONSTER;

void create () {
    ::create();
    if (secure_random(2)) {
        set_gender("male");
    } else {
        set_gender("female");
    }
    set_id(({ "hamster" }));
    set_adjective(({ "small" }));
    set_name("small hamster");
    set_short("small hamster");
    set_long("A hamster foraging through the grass.");
    set_level(1);
    set_species("hamster");
}