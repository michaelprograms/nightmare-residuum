inherit STD_ROOM;

nosave private int __Terrain;

int query_terrain () {
    return __Terrain;
}
void set_terrain (int t) {
    __Terrain = t;

    switch(__Terrain) {
    case 0..2:
        set_room_square_color("%^BLUE%^");
        set_short("deep water");
        set_long("Surrounded by deep water.");
        break;
    case 3..4:
        set_room_square_color("%^BLUE%^BOLD%^");
        set_short("shallow water");
        set_long("Surrounded by shallow water.");
        break;
    case 5:
        set_room_square_color("%^YELLOW%^");
        set_short("arid sands");
        set_long("Surrounded by arid sands.");
        break;
    case 6:
        set_room_square_color("%^GREEN%^BOLD%^");
        set_short("grassy fields");
        set_long("Surrounded by grassy fields.");
        break;
    case 7:
        set_room_square_color("%^GREEN%^");
        set_short("forests");
        set_long("Surrounded by forests.");
        break;
    case 8..10:
        set_room_square_color("%^ORANGE%^");
        set_short("mountainous terrain");
        set_long("Surrounded by mountainous terrain.");
        break;
    case 11..12:
        set_room_square_color("%^BOLD%^");
        set_short("snow covered hills");
        set_long("Surrounded by snow covered hills.");
        break;
    default:
        set_room_square_color("%^RED%^");
    }
}

int is_virtual_room () { return 1; }

void create() {
    ::create();
    set_short("a terrain somewhere");
    set_long("The terrain of a planet.");
}
