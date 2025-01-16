inherit STD_NPC;

void create () {
    ::create();
    set_name("R-0111010");
    set_id(({ "synthetic creature", "synthetic", "creature", }));
    set_short("a synthetic creature");
    set_long("A creature seemingly constructed of synthetic components. It consists of a large elongated torso structure supported by four jointed legs, all encased in a scale-like exoskeleton. There are two articulated arms at each end of the structure, almost looking like a type of crustacean. The creature does not seem to have a discernable head or face.");
    set_species("custodian");

    set_say_response("facility", "Circular platforms designated for transport to controlled zones. Utilize western platform for descent to planet surface.");
    set_say_response("planet", "Observation: Your species demonstrates a significant compatibility with this planet's climate and geographical features.");
    set_say_response("species", "Statement: We have maintained custodianship over the human species for an extended temporal period.");
    set_say_response("custodian", "Function: Our original directive is the caretaking of an ancient species now extinct. Our current directive is the caretaking of the human species.");
    set_say_response("ancient", "Observation: They have been relegated to historical records. Few retain memory after such a temporal duration.");
    set_say_response("directive", "Statement: The set of laws we adhere to in the benevolent caretaking of other species.");
}

string *names = ({ });

/**
 * The STD_NPC hook for receiving a living object in this object's environment.
 *
 * @param {STD_LIVING} living the living object entering the environment
 */
void handle_receive_living_in_env (object living) {
    string name;
    ::handle_receive_living_in_env(living);
    if (characterp(living)) {
        name = living->query_cap_name();
        if (member_array(name, names) == -1) {
            handle_command("say Welcome to this facility, " + name + ".");
            names += ({ name });
            call_out((: names -= ({ $1 }) :), 30, name);
        }
    }
}