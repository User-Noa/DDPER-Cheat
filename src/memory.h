#pragma once
#include <cstdint>
#include <sys/uio.h>
#include <unistd.h>
#include <cstring>

template <typename T>
T ReadMem(uintptr_t address) {
    if (!address) return T();
    T buffer = T();
    struct iovec local[1], remote[1];
    local[0].iov_base = &buffer;
    local[0].iov_len = sizeof(T);
    remote[0].iov_base = (void*)address;
    remote[0].iov_len = sizeof(T);
    ssize_t bytesRead = process_vm_readv(getpid(), local, 1, remote, 1, 0);
    if (bytesRead != sizeof(T)) return T();
    return buffer;
}

template <typename T>
void WriteMem(uintptr_t address, T value) {
    if (!address) return;
    struct iovec local[1], remote[1];
    local[0].iov_base = &value;
    local[0].iov_len = sizeof(T);
    remote[0].iov_base = (void*)address;
    remote[0].iov_len = sizeof(T);
    process_vm_writev(getpid(), local, 1, remote, 1, 0);
}

uintptr_t GetModuleBase(const char* module_name, int target_index = 0);
