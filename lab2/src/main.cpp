#include <rb_tree.hpp>

#include <iostream>
#include <fstream>

using namespace cust;

struct var {
    std::string first;
    uint64_t second;
};

bool operator==(var const& a, var const& b) {
    return a.first == b.first;
}

bool operator<(var const& a, var const& b) {
    return a.first < b.first;
}

void lower(std::string& s) {
    for (auto& c : s) {
        if ('A' <= c && c <= 'Z') {
            c += 'a' - 'A';
        }
    }
}

std::istream& operator>>(std::istream& iss, var& v) {
    iss >> v.first >> v.second;
    lower(v.first);
    return iss;
}

std::ostream& operator<<(std::ostream& os, var& v) {
    return os << v.first << " " << v.second;
}

int main() {   
    RBTree<var> tree;

    std::string word;
    while (std::cin >> word) {
        if (word == "+") {
            var v; std::cin >> v;
            try {
                tree.add(v);
                std::cout << "OK\n";  
            } catch (...) {
                std::cout << "Exist\n";
            }
        } else if (word == "-") {
            std::cin >> word;
            lower(word);
            var v{word, 0};
            try {
                tree.remove(v);
                std::cout << "OK\n";
            } catch (...) {
                std::cout << "NoSuchWord\n";
            }
        } else if (word == "!") {
            std::string filename;
            std::cin >> word; std::cin.get();
            std::getline(std::cin, filename);
            if (word == "Save") {
                std::ofstream off(filename, std::ios_base::trunc);
                tree.saveInStream(off);
                off.close();
            } else {
                std::ifstream iff(filename);
                tree = std::move(RBTree<var>::readFromStream(iff));
                iff.close();
            }
            std::cout << "OK\n";
        } else if (word == "print") {
            tree.printTree(std::cout);
            std::cout << "\n";
        } else {
            try {
                lower(word);
                auto v = tree.find(var{word, 0});
                std::cout << "OK: " << v->second << "\n";
            } catch (...) {
                std::cout << "NoSuchWord\n";
            }
        }
    }

    return 0;
}