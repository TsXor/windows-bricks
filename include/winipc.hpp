#include <minwindef.h>
#include <minwinbase.h>
#include <handleapi.h>
#include <memoryapi.h>
#include "winexc.hpp"
#include "winsynch.hpp"
#include "unicode/encoding.hpp"
#include "unicode/tdef.hpp"

namespace Windows {

class SysNamedPipe {
protected:
    HANDLE _handle;
    Tstring* _name;
public:
    // WIP
};

template <bool Writable>
class SysSharedMemory {
protected:
    HANDLE _handle;
    void* _ptr;
    Tstring* _name;
    size_t _size;
public:
    SysSharedMemory(bool create, const Tstring_view& name, size_t size) {
        this->_name = new Tstring(name); // copy name to heap
        DWORD create_perm = Writable ? PAGE_READWRITE : PAGE_READONLY;
        DWORD open_perm = Writable ? FILE_MAP_WRITE : FILE_MAP_READ;
        DWORD size_high_part = size >> 32;
        DWORD size_low_part = size & 0xffffffff;
        HANDLE file_mapping = create
            ? CreateFileMapping(INVALID_HANDLE_VALUE, NULL, create_perm, size_high_part, size_low_part, this->_name->c_str())
            : OpenFileMapping(open_perm, TRUE, this->_name->c_str());
        ASSERT_THROW_WINA(file_mapping != NULL, "failed to create file mapping");
        LPVOID ptr = MapViewOfFile(file_mapping, open_perm, 0, 0, size);
        this->_handle = file_mapping;
        this->_ptr = ptr;
        this->_size = size;
    }
    EXCEPTABLE_DTOR(SysSharedMemory,
        delete this->_name;
        BOOL success = CloseHandle(this->_handle);
        ASSERT_THROW_WINA(success, "failed to destroy file mapping");
    )
    Tstring* get_name(void) {
        return this->_name;
    }
    size_t get_size(void) {
        return this->_size;
    }
    void* get_ptr(void) {
        return this->_ptr;
    }
};

template <typename T, bool Writable>
class SysSharedValue {
protected:
    SysSharedMemory<Writable>* _mem;
    SysLock* _lock;
public:
    SysSharedValue(bool create, const Tstring_view& name) {
        this->_mem = new SysSharedMemory<Writable>(create, name, sizeof(T));
        Tstring lock_name = std::string(name).append(TSTR("_lck"));
        this->_lock = new SysLock(create, Tstring_view(lock_name));
    }
    SysSharedValue(bool create, const Tstring_view& name, const T& val):
        SysSharedValue(create, name) {
            auto ptr = this->get_ptr();
            *ptr = val;
        }
    ~SysSharedValue() {
        delete this->_mem;
        delete this->_lock;
    }
    Tstring get_name(void) {
        return this->_mem->get_name();
    }
    T* get_ptr(void) {
        return (T* )this->_mem->get_ptr();
    }
    operator T() {
        T* ptr = this->get_ptr();
        this->_lock->acquire();
        T ret = *ptr;
        this->_lock->release();
        return ret;
    }
    template <typename = void> requires Writable
    void operator=(const T& obj) {
        T* ptr = this->get_ptr();
        this->_lock->acquire();
        *ptr = obj;
        this->_lock->release();
    }
};

} // namespace Windows