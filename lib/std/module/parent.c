nosave private object __Parent = 0;

object query_parent () {
    return __Parent;
}

void create () {
    __Parent = previous_object();
}