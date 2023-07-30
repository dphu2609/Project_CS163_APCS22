#pragma once

#include <bits/stdc++.h>

namespace Code {
    enum ID {
        InsertAVL,
        DeleteAVL,
        SearchAVL,
        Insert234,
        Delete234,
        Search234,
        InsertTrie,
        DeleteTrie,
        SearchTrie,
        InsertHeap,
        DeleteHeap,
        Djikstra,
        Prim,
        InsertHashTable,
        DeleteHashTable,
        SearchHashTable,
        CodeCount
    };
};

class CodeHolder {
public:
    void load();
    std::vector<std::string>& operator [] (int id);
    const std::vector<std::string>& operator [] (int id) const;
    std::vector<bool> mStateActivated = {};
private:
    std::vector<std::vector<std::string>> mCodeMap; 
};