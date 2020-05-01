#ifndef INSTANCEOF_H
#define INSTANCEOF_H

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

#endif // INSTANCEOF_H
