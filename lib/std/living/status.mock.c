inherit "/std/shadow.c";

int *MockHeals = ({ });
void reset_mock_heals () {
    MockHeals = ({ });
}
int *query_mock_heals () {
    return MockHeals;
}
void heal (int n) {
    MockHeals += ({ n });
}

mapping MockAddVitals = ([ "hp": 0, "sp": 0, "mp": 0 ]);
void reset_mock_add_vitals () {
    MockAddVitals = ([ "hp": 0, "sp": 0, "mp": 0 ]);
}
mapping query_mock_add_vitals () {
    return MockAddVitals;
}
void add_hp (int n) {
    MockAddVitals["hp"] += n;
}
void add_sp (int n) {
    MockAddVitals["sp"] += n;
}
void add_mp (int n) {
    MockAddVitals["mp"] += n;
}