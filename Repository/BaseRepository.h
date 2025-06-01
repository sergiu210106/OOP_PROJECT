#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <vector>
#include <memory>

template <typename T>
class BaseRepository {
public:
    virtual ~BaseRepository() = default;

    virtual void add(const T& item) = 0;
    virtual void remove(int id) = 0;
    virtual void update(const T& item) = 0;
    virtual std::vector<T> getAll() const = 0;
};

#endif // BASEREPOSITORY_H