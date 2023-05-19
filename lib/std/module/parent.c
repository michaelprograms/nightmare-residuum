nosave private object __Parent = 0;

object query_parent () {
    return __Parent;
}

int set_parent (object ob) {
    if (!regexp(base_name(ob), "^("+STD_USER[0..<3]+"|"+M_PARENT[0..<3]+".test)")) {
        return 0;
    }
    __Parent = ob;
    return 1;
}

void create () {
    __Parent = previous_object();
}