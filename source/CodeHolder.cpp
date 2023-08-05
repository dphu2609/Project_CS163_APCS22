#include <CodeHolder.hpp>

void CodeHolder::load() {
    mCodeMap.resize(Code::CodeCount);
    mCodeMap[Code::InsertAVL] = {
        "insert node",
        "check balance factor",
        "   case 1: this.rotateRight",
        "   case 2: this.left.rotateLeft, this.rotateRight",
        "   case 3: this.rotateLeft",
        "   case 4: this.right.rotateRight, this.rotateLeft",
    };
    
    mCodeMap[Code::DeleteAVL] = {
        "delete node",
        "check balance factor",
        "   case 1: this.rotateRight",
        "   case 2: this.left.rotateLeft, this.rotateRight",
        "   case 3: this.rotateLeft",
        "   case 4: this.right.rotateRight, this.rotateLeft",
    };

    mCodeMap[Code::SearchAVL] = {
        "if this == null",
        "   return null",
        "else if this key == search value",
        "   return this",
        "else if this key > search value",
        "   search this.left",
        "else",
        "   search this.right"
    };

    mCodeMap[Code::Insert234] = {
        "traverse to leaf",
        "   if this numKeys == 3",
        "       split this",
        "if this is leaf",
        "   insert node to this",
    };

    mCodeMap[Code::Delete234] = {
        "search node to delete",
        "if root has two children and two children numKeys == 1, merge root",
        "traverse to delete node",
        "if this is leaf",
        "   if this numKeys == 1",
        "       if this is root, delete this",
        "       else, borrow from or merge with siblings",
        "   else, delete node from this",
        "else",
            "find replacement leaf node, replace node with replacement node",
            "delete replacement node as above if statement",
    };

    mCodeMap[Code::Search234] = {
        "while this is not null",
        "   compare search value with all keys in this node",
        "   if found",
        "       return this",
        "   go to next appropriate child node",
        "return null",
    };

    mCodeMap[Code::InsertTrie] = {
        "while not end of string",
        "   if this.children does not contain this char",
        "       create new node",
        "   this = this.children[this char]",
        "this.isEndOfWord = true",
    };

    mCodeMap[Code::DeleteTrie] = {
        "search string in trie",
        "if this contain children",
        "   this.isEndOfWord = false",
        "else",
        "   while this is not contain any children or this is not end of word",
        "       this = this.parent",
        "       delete this",
    };

    mCodeMap[Code::SearchTrie] = {
        "while not end of string",
        "   if this.children does not contain this char",
        "       return false",
        "   this = this.children[this char]",
        "return this.isEndOfWord",
    };

    mCodeMap[Code::InsertMaxHeap] = {
        "insert node to last position",
        "while this.parent is not null and this.parent.value < this.value",
        "   swap this and this.parent",
    };

    mCodeMap[Code::InsertMinHeap] = {
        "insert node to last position",
        "while this.parent is not null and this.parent.value > this.value",
        "   swap this and this.parent",
    };

    mCodeMap[Code::DeleteMaxHeap] = {
        "set this.value to top value + 1",
        "while this.parent is not null and this.parent.value < this.value",
        "   swap this and this.parent",
        "swap this and last node",
        "delete last node",
        "while this < this.children",
        "   swap this and max of this.left and this.right",
    };

    mCodeMap[Code::DeleteMinHeap] = {
        "set this.value to top value - 1",
        "while this.parent is not null and this.parent.value > this.value",
        "   swap this and this.parent",
        "swap this and last node",
        "delete last node",
        "while this < this.children",
        "   swap this and max of this.left and this.right",
    };

    mCodeMap[Code::GetTopHeap] = {
        "return value of top node",
    };

    mCodeMap[Code::Djikstra] = {
        "set all distance to infinity",
        "set distance of source to 0",
        "create min priority queue",
        "add source to priority queue",
        "while priority queue is not empty",
        "   get node with minimum distance from priority queue",
        "   if node has been visited then continue",
        "   for each neighbor of this node",
        "       if distance of this node + distance of edge < distance of neighbor",
        "           set distance of neighbor to distance of this node + distance of edge",
        "           add neighbor to priority queue",
    };

    mCodeMap[Code::Prim] = {
        "set all distance to infinity",
        "set distance of source to 0",
        "create min priority queue",
        "add source to priority queue",
        "while priority queue is not empty",
        "   get node with minimum distance from priority queue",
        "   if node has been visited then continue",
        "   for each neighbor of this node",
        "       if distance of edge < distance of neighbor",
        "           set distance of neighbor to distance of edge",
        "           add neighbor to priority queue",
    };

    mCodeMap[Code::InsertHashTable] = {
        "index = hash(value)",
        "while table[index] is not null",
        "   index = (index + 1) mod table size",
        "table[index] = value",
    };

    mCodeMap[Code::DeleteHashTable] = {
        "index = hash(value)",
        "while table[index] is not null",
        "   if table[index] == value",
        "       table[index] = null",
        "       break",
        "   index = (index + 1) mod table size",
    };

    mCodeMap[Code::SearchHashTable] = {
        "index = hash(value)",
        "while table[index] is not null",
        "   if table[index] == value",
        "       return true",
        "   index = (index + 1) mod table size",
        "return false",
    };
}

std::vector<std::string>& CodeHolder::operator[] (int id) {
    return mCodeMap[id];
}

const std::vector<std::string>& CodeHolder::operator [] (int id) const {
    return mCodeMap[id];
}