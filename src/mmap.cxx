/*!
 * \file
 * \brief MmapReader realization.
*/

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <tsearch/tsearch.h>

static size_t const FS_PRT = 4096;

namespace tsearch
{
    /*!
     * Constructor with parameters.
     *
     * \param[in] _file_name File name for read.
    */
    MmapReader::MmapReader(std::string const _file_name)
        : file_name(_file_name)
        , word("")
        , btw_word("")
        , buf(nullptr)
        , buf_len(0)
        , file_len(0)
        , offset(0)
        , fd(-1)
        , part_size(0)
    {
        struct stat statbuf;
        int rv = stat(file_name.c_str(), &statbuf);
        if (rv)
        {
            throw TgError::FILE_NOT_FOUND;
        }

        file_len = static_cast<size_t>(statbuf.st_size);

        fd = open(file_name.c_str(), O_RDONLY);
        if (fd < 0)
        {
            throw TgError::OPEN_FAILED;
        }
    }


    MmapReader::~MmapReader()
    {
        if (buf)
        {
#ifdef _WIN32
            UnmapViewOfFile(buf);
#else
            if (buf != MAP_FAILED)
            {
                munmap(buf, buf_len);
            }
#endif
        }
        if (fd != -1)
        {
            close(fd);
        }
    }

    /*!
     * Get buffer for between parts searching.
     *
     * \param[out] part string for between buffer.
     * \return Buffer availability.
    */
    bool MmapReader::betweenbuf(std::string &part)
    {
        if (btw_word.size() == 2 * (word.size() - 1))
        {
            part = btw_word;
            return true;
        }
        return false;
    }

    /*!
     * Place a part of file in the memory.
     *
     * \param[out] buf Data buffer.
     * \return part data size.
    */
    size_t MmapReader::nextpart(char **_buf)
    {
        size_t map_size = file_len - offset;

        if (buf)
        {
            if (word.size())
            {
                btw_word.clear();
                btw_word.append(buf + buf_len - word.size() + 1, word.size() - 1);
            }

#ifdef _WIN32
            UnmapViewOfFile(buf);
            buf = nullptr;
#else
            if (buf != MAP_FAILED)
            {
                munmap(buf, buf_len);
                buf = nullptr;
            }
#endif
        }
        buf_len = std::min(map_size, part_size);
        if (offset >= file_len || not buf_len || word.size() * 2 > file_len)
        {
            *_buf = nullptr;
            return 0;
        }

#ifdef _WIN32
        {
            buf_len = FS_PRT + buf_len - buf_len % FS_PRT;
            buf_len = std::min(map_size, buf_len);
            HANDLE hmmap = CreateFileMapping(
                (HANDLE)_get_osfhandle(fd)
              , nullptr
              , PAGE_READONLY
              , 0
              , buf_len
              , NULL
            );
            *_buf = buf = reinterpret_cast<char *>(MapViewOfFile(
                hmmap
              , FILE_SHARE_READ
              , 0
              , offset
              , buf_len
            ));
            if (hmmap != NULL)
            {
                CloseHandle(hmmap);
            }
        }
        if (not buf)
        {
            buf_len = 0;
        }
#else
        auto page_size = static_cast<size_t>(sysconf(_SC_PAGE_SIZE));
        buf_len = page_size + buf_len - buf_len % page_size;
        buf_len = std::min(map_size, buf_len);

        *_buf = buf = reinterpret_cast<char*>(mmap(
            nullptr
          , buf_len
          , PROT_READ
          , MAP_PRIVATE
          , fd
          , static_cast<ssize_t>(offset)
        ));
        if (buf == MAP_FAILED)
        {
            buf_len = 0;
            *_buf = buf = nullptr;
            // TODO: make a warning
        }
#endif
        else
        {
            if (word.size())
            {
                btw_word.append(buf, word.size() - 1);
            }

            offset += buf_len;
        }
        return buf_len;
    }
}

