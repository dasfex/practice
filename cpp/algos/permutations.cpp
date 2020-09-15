void BubbleSort(std::vector<int>& cur, size_t l, size_t r) {
    for (size_t i = l; i < r; ++i) {
        for (size_t j = i + 1; j <= r; ++j) {
            if (cur[i] > cur[j]) {
                std::swap(cur[i], cur[j]);
            }
        }
    }
}

int FindCeil(std::vector<int>& cur, int first, size_t l, size_t h) {
    size_t ceil_index = l;

    for (size_t i = l + 1; i <= h; ++i) {
        if (cur[i] > first && cur[i] < cur[ceil_index]) {
            ceil_index = i;
        }
    }

    return ceil_index;
}

std::vector<std::vector<int>> GeneratePermutations(size_t len) {
    std::vector<std::vector<int>> res;

    std::vector<int> cur;
    for (size_t i = 0; i < len; ++i) {
        cur.push_back(i);
    }

    bool is_finished = false;
    while (!is_finished) {
        res.push_back(cur);
        int i;
        for (i = len - 2; i >= 0; --i) {
            if (cur[i] < cur[i + 1]) {
                break;
            }
        }
        if (i == -1) {
            is_finished = true;
        } else {
            int ceil_index = FindCeil(cur, cur[i], i + 1, len - 1);
            std::swap(cur[i], cur[ceil_index]);
            BubbleSort(cur, i + 1, len - 1);
        }
    }

    return res;
}
