mixed *distinct_array (mixed *arr) {
    int i, maxi = sizeof(arr);
    mapping tmp = m_allocate(maxi);
    for (i = 0; i < maxi; i ++) {
        tmp[arr[i]] = 1;
    }
    return m_indices(tmp);
}