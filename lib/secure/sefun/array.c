mixed *distinct_array (mixed *arr) {
    int i, maxi = sizeof(arr);
    mapping tmp = allocate_mapping(maxi);
    for (i = 0; i < maxi; i ++) {
        tmp[arr[i]] = 1;
    }
    return keys(tmp);
}