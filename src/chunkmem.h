#ifndef BPRAY_CHUNKMEM
#define BPRAY_CHUNKMEM
#include <vector>
#include "utils.h"

template <typename T>
class vec {
        T* _data;
        int _curr;
        int _max;

        void grow() {
                if (_curr == _max) {
                        _max *= 2;
                        _data = (T*)realloc(_data, _max * sizeof(T));
                }
        }
public:
        vec(int s)
                : _data((T*)malloc(s *sizeof(T)))
                , _curr(0)
                , _max(s) {
        }
        ~vec() {
                free (_data);
        }
        
        void push_back(T& t) {
                grow();
                _data[_curr++] = t;
        }
        
        void push_back(T&& t) {
                grow();
                _data[_curr++] = t;
        }
        
        T* data() { return _data; }
        const T& operator[](int i) { return _data[i]; }
        int size() const { return _curr; }
        void resize(int newsize) { _curr = newsize; }
};

template <typename T>
class chunkmem {
        vec<T>& _data;
        const int _start;
        int _end;
        const int _id;
public:
        chunkmem(int id, vec<T>& vec) :
                _data(vec)
                , _start(vec.size())
                , _end(vec.size())
                , _id(id) {}
        
        void destroy() {
                if (_end != _data.size()) {
                        INFO(_id << " " <<_end << " " << _data.size());
                        abort();
                }
                _data.resize(_start);
        }
        
        void push_back(T& item) {
                _data.push_back(item);
                _end = _data.size();
        }
        
        const T& operator[](int i) { return _data[i]; }
        T* begin() const { return _data.data() + _start; }
        T* end() const { return _data.data() + _end; }
        int size() const { return _end - _start; }
        int id() const { return _id; }
};


template <typename T>
class chunkmemmanager {
        vec<T> _data;
        std::vector<chunkmem<T>> _chunks;
        int _cnt;
public:
        chunkmemmanager(int initsize) : _data(initsize), _cnt(0) {
        }

        chunkmem<T>& create() {
                _chunks.emplace_back(_cnt++, _data);
                return _chunks.back();
        }
};
#endif
