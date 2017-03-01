#ifndef QING_CHECK_H
#define QING_CHECK_H

//int isNumber(double d){return (d==d);}
inline bool qing_is_nan(float d) {
    return !(d==d);
}

#endif // QING_CHECK_H
