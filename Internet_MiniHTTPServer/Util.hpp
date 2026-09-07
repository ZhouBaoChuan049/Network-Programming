#ifndef __UTIL__
#define __UTIL__
#include "./BasicElement/Common.hpp"

namespace HttpUtilModule
{
    class Util
    {
    public:
        long GetFileSize(std::string filename)
        {
            std::ifstream in(filename, std::ios::binary);
            in.seekg(0, std::ios::end);
            std::streamoff size = in.tellg();
            in.seekg(0, std::ios::beg);
            return (long)size;
        }
        void GetFileContent(std::string &filename, std::string &content)
        {
            long n = GetFileSize(filename);
            int size = n;
            content.resize(size);
            std::ifstream in(filename, std::ios::binary);
            in.read(content.data(), size);
            std::cout << "FileName: " << filename << std::endl;
            std::cout << "FileSize: " << n << std::endl;
            std::cout << "ContentSize: " << content.size() << std::endl;
        }

    private:
    };
}

#endif