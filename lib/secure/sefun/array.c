/**
 * Return unique / distinct elements from an array.
 *
 * @param arr the array to make distinct
 * @returns mixed array with distinct elements
 */
mixed *distinct_array (mixed *arr) {
    int i, l;
    mapping tmp;
    l = sizeof(arr);
    tmp = allocate_mapping(l);
    for (i = 0; i < l; i ++) {
        tmp[arr[i]] = 1;
    }
    return keys(tmp);
}

/**
 * Process an array in a reduce pattern.
 *
 * @param arr the array to reduce
 * @param fn the function to process array items
 * @param result accumulator field
 * @returns the result of the array being reduced
 */
mixed reduce (mixed *arr, function fn, mixed result) {
    int i, l;

    if (!arrayp(arr) || !sizeof(arr)) {
        error("Bad argument 1 to array->reduce");
    }
    if (!functionp(fn)) {
        error("Bad argument 2 to array->reduce");
    }
    if (undefinedp(result)) {
        error("Bad argument 3 to array->reduce");
    }

    l = sizeof(arr);
    for (i = 0; i < l; i ++) {
        result = evaluate(fn, result, arr[i], i);
        // callback parameters:
        // function (mixed result, mixed item, int i)
    }

    return result;
}