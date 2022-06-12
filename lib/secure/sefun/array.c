mixed *distinct_array (mixed *arr) {
    int i, maxi;
    mapping tmp;
    if (!arrayp(arr) || !sizeof(arr)) error("Bad argument 1 to array->distinct_array");
    tmp = allocate_mapping(maxi = sizeof(arr));
    for (i = 0; i < maxi; i ++) {
        tmp[arr[i]] = 1;
    }
    return keys(tmp);
}