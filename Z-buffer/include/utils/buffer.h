#ifndef BUFFER_H
#define BUFFER_H

#include <assert.h>
#include <vector>

template <class T>
class buffer {
protected:
    int _size;
    std::vector<T> _buf;

public:
    buffer()
    {
        _size = 0;
        _buf = std::vector<T>();
    }

    buffer(int size)
    {
        _buf = std::vector<T>(size);
        _size = size;
    }

    ~buffer()
    {
        _buf.clear();
        _size = 0;
    }

    void init(T value)
    {
        _buf = std::vector<T>(_size, value);
    }

    void init(int size, T value)
    {
        _size = size;
        _buf = std::vector<T>(size, value);
    }

    int size() { return _size; }

    /*buffer operator =(const buffer& buf)
    {

    }*/

    T* data() { return _buf.data(); }

    T operator [](const int& index) {
        assert(index < _size && index >= 0);
        return _buf[index];
    }
};

template <class T>
class buffer2D : public buffer<T> {
private:
    int width;
    int height;

    void assertRange(int x, int y)
    {
        bool x_range = x >= 0 && x < height;
        bool y_range = y >= 0 && y < width;
        assert(x_range && y_range);
    }

public:
    buffer2D() : buffer<T>() {};

    buffer2D(int width, int height)
    {
        this->_size = width * height;
        this->_buf = std::vector<T>(this->_size);
        this->width = width;
        this->height = height;
    }

    ~buffer2D()
    {
        width = 0;
        height = 0;
    }

    void init(int width, int height, T value)
    {
        this->_size = width * height;
        this->_buf = std::vector<T>(this->_size, value);
        this->width = width;
        this->height = height;
    }

    int getWidth() { return width; }

    int getHeight() { return height; }

    T get(int row, int col) 
    {
        assertRange(row, col);
        return this->_buf[row * width + col];
    }

    void set(int row, int col, T value)
    {
        assertRange(row, col);
        this->_buf[row * width + col] = value;
    }
};
#endif // !BUFFER_H
