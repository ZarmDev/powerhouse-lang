export module fileloader;

import <string>;
import <vector>;

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif
export namespace fileloader {
    std::vector<char> load_file_into_buffer(const std::string& path)
    {
#ifdef _WIN32
        // --- WINDOWS VERSION (ReadFile) ---
        HANDLE h = CreateFileA(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (h == INVALID_HANDLE_VALUE)
            return {};

        LARGE_INTEGER size;
        GetFileSizeEx(h, &size);

        std::vector<char> buffer(size.QuadPart);

        DWORD bytesRead = 0;
        ReadFile(h, buffer.data(), (DWORD)buffer.size(), &bytesRead, nullptr);

        CloseHandle(h);
        return buffer;

#else
        // --- LINUX / MACOS VERSION (POSIX read) ---
        int fd = open(path.c_str(), O_RDONLY);
        if (fd < 0)
            return {};

        struct stat st;
        fstat(fd, &st);

        std::vector<char> buffer(st.st_size);

        read(fd, buffer.data(), buffer.size());
        close(fd);

        return buffer;
#endif
    }
}