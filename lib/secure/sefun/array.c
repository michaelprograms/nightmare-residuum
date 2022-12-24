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

mixed reduce (mixed *arr, function fn, mixed result) {
    int i, l;

    if (!arrayp(arr) || !sizeof(arr)) error("Bad argument 1 to array->reduce");
    if (!functionp(fn)) error("Bad argument 2 to array->reduce");
    if (undefinedp(result)) error("Bad argument 3 to array->reduce");

    l = sizeof(arr);
    for (i = 0; i < l; i ++) {
        result = evaluate(fn, result, arr[i], i);
        // callback parameters:
        // function (mixed result, mixed item, int i)
    }

    return result;
}