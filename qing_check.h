#ifndef QING_CHECK_H
#define QING_CHECK_H

//false: d==d, so that d is a valid number, not a nan
//true: d!=d, so that d is nan,
inline bool qing_is_nan(float d) {
    return !(d==d);
}

inline bool qing_is_number(float d) {
    return d==d;
}

#endif // QING_CHECK_H
