/*!
 * \file
 * \brief Realization of the base procedures.
*/
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <tsearch/tsearch.h>



extern size_t alpha_skip_lookup[];
extern size_t *find_skip_lookup;

void generate_alpha_skip(char const *find, size_t f_len, size_t skip_lookup[]);
void generate_find_skip(const char *find, const size_t f_len, size_t *skip_lookup);



/* Boyer-Moore strstr */
char const *bm_strnstr(
    char const *const s
  , char const *const find
  , size_t const s_len
  , size_t const f_len
  , size_t const _alpha_skip_lookup[]
  , size_t const *_find_skip_lookup
){
    ssize_t i;
    ssize_t pos = static_cast<ssize_t>(f_len - 1);

    while (pos < static_cast<ssize_t>(s_len))
    {
        for (
            i = static_cast<ssize_t>(f_len - 1);
#ifndef CASE_SENSITIVE
            i >= 0 && tolower(s[pos]) == tolower(find[i]);
#else
            i >= 0 && s[pos] == find[i];
#endif
            pos--, i--
        );
        if (i < 0)
        {
            return s + pos + 1;
        }
        pos += static_cast<ssize_t>(
            std::max(_alpha_skip_lookup[(unsigned char)s[pos]], _find_skip_lookup[i])
        );
    }

    return nullptr;
}



namespace tsearch
{
    size_t count_words(std::string filename, std::string word)
    {
        char *buf = nullptr;
        size_t count = 0;

        MmapReader file(filename, word);
        file.set_part_size(4194304);
        std::string trans;

        if (word.empty())
        {
            return count;
        }
        if (!file.is_exist())
        {
            throw TgError::FILE_NOT_FOUND;
        }

        generate_alpha_skip(word.c_str(), word.size(), alpha_skip_lookup);
        find_skip_lookup = new size_t[word.size()];
        generate_find_skip(word.c_str(), word.size(), find_skip_lookup);

        do
        {
            auto len = file.nextpart(&buf);
            char const *match = nullptr;

            if (file.betweenbuf(trans))
            {
                match = bm_strnstr(
                    trans.c_str()
                  , word.c_str()
                  , trans.size()
                  , word.size()
                  , alpha_skip_lookup
                  , find_skip_lookup
                );
                if (match != nullptr)
                {
                    count++;
                }
            }

            match = buf;
            while (static_cast<size_t>(match - buf) < len)
            {
                match = bm_strnstr(
                    match + word.size()
                  , word.c_str()
                  , len
                  , word.size()
                  , alpha_skip_lookup
                  , find_skip_lookup
                );
                if (match == nullptr)
                {
                    break;
                }
                count++;
            }
        } while (buf != nullptr);

        delete[] find_skip_lookup;
        return count;
    }

    unsigned long calc_checksum(std::string filename)
    {
        return 12;
    }



    MmapReader::MmapReader(std::string const _file_name, std::string const _word)
        : file_name(_file_name)
        , word(_word)
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
        if (rv != 0) {
            //log_err("Skipping %s: Error fstating file.", file_full_path);
            throw TgError::FSTAT_FAILED;
        }

        file_len = static_cast<size_t>(statbuf.st_size);

        fd = open(file_name.c_str(), O_RDONLY);
        if (fd < 0) {
            //log_err("Skipping %s: Error opening file: %s", file_full_path, strerror(errno));
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


    bool MmapReader::betweenbuf(std::string &part)
    {
        if (btw_word.size() == 2 * (word.size() - 1))
        {
            part = btw_word;
            return true;
        }
        return false;
    }


    size_t MmapReader::nextpart(char **_buf)
    {
        size_t map_size = file_len - offset;

        if (buf != nullptr)
        {
            btw_word.clear();
            btw_word.append(buf + buf_len - word.size() + 1, word.size() - 1);
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
        if (!buf_len)
        {
            *_buf = nullptr;
            return 0;
        }

#ifdef _WIN32
        {
            HANDLE hmmap = CreateFileMapping(
                (HANDLE)_get_osfhandle(fd)
              , 0
              , PAGE_READONLY
              , 0
              , buf_len
              , NULL
            );
            *_buf = buf = (char *)MapViewOfFile(
                hmmap
              , FILE_SHARE_READ
              , 0
              , offset
              , buf_len
            );
            if (hmmap != NULL)
            {
                CloseHandle(hmmap);
            }
        }
#else
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
            //log_err("File %s failed to load: %s.", file_full_path, strerror(errno));
        }
        else
        {
            btw_word.append(buf, word.size() - 1);
            offset += buf_len;
        }
#endif
        return buf_len;
    }

}

