#ifndef TESTS_AUX_H
#define TESTS_AUX_H

#include <vector>
#include "tipos.h"

namespace test {
    template<class T>
    unsigned int cuenta(const T &x, const std::vector<T> &v) {
        unsigned int cant = 0;
        for (unsigned int i = 0; i < v.size(); ++i) {
            if (x == v[i]) ++cant;
        }
        return cant;
    }

    template<class T>
    bool mismos(const std::vector<T> &a, const std::vector<T> &b) {
        bool res = a.size() == b.size();
        for (unsigned int i = 0; res && i < a.size(); ++i) {
            res = cuenta(a[i], a) == cuenta(a[i], b);
        }
        return res;
    }
}

#endif //TESTS_AUX_H
