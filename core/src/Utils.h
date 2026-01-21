#ifndef UTILS_H
#define UTILS_H

#include <cmath>

namespace Utils {
    static inline float mapRange(float value, float fromStart, float fromEnd, float toStart, float toEnd) {
        return toStart + (toEnd - toStart) * ((value - fromStart) / (fromEnd - fromStart));
    }
}

#endif // UTILS_H
