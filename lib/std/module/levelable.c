private int __Level = 1;

int query_level () {
    return __Level;
}
void set_level (int l) {
    if (l < 0) {
        l *= -1;
    }
    __Level = l;
}