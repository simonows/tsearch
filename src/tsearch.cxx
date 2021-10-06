/*!
 * \file
 * \brief Realization of the base search and checksum procedures.
*/

#include <tsearch/tsearch.h>

/// 4 Mb - part reading
static size_t const MB4 = 4194304;

extern size_t alpha_skip_lookup[];
extern size_t *find_skip_lookup;

void generate_alpha_skip(char const *find, size_t f_len, size_t skip_lookup[])
    noexcept;
void generate_find_skip(const char *find, const size_t f_len, size_t *skip_lookup)
    noexcept;

/*!
 * Boyer-Moore strstr for substring search.
 *
 * \param[in] s Data buffer.
 * \param[in] find Search string.
 * \param[in] s_len Data buffer size.
 * \param[in] f_len Search string size.
 * \param[in] _alpha_skip_lookup Skip sequence.
 * \param[in] _find_skip_lookup Find string skip sequence.
 *
 * \return Pointer to the found substring.
*/
char const *bm_strnstr(
    char const *const s
  , char const *const find
  , size_t const s_len
  , size_t const f_len
  , size_t const _alpha_skip_lookup[]
  , size_t const *_find_skip_lookup
) noexcept {
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
            std::max(_alpha_skip_lookup[static_cast<unsigned char>(s[pos])], _find_skip_lookup[i])
        );
    }

    return nullptr;
}


namespace tsearch
{
    /*!
     * Finding the number of occurrences of a substring in a string.
     *
     * \param[in] filename File name to search for.
     * \param[in] filename Word to search for.
     *
     * \return Number of occurrences.
    */
    size_t count_words(std::string filename, std::string word)
    {
        char *buf = nullptr;
        size_t count = 0;

        MmapReader file(filename, word);
        file.set_part_size(MB4);
        std::string trans;

        if (word.empty() || word.size() > file.size())
        {
            return count;
        }
        if (not file.is_exist())
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
                  , len - static_cast<size_t>(match - buf)
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
        } while (buf);

        delete[] find_skip_lookup;

        if (not file.eof())
        {
            throw TgError::FILE_READ_ERROR;
        }
        return count;
    }

    /*!
     * Calculating the checksum.
     *
     * \param[in] filename File name to calc for.
     *
     * \return uint32_t checksum.
    */
    uint32_t calc_checksum(std::string filename)
    {
        uint32_t checksum = 0;
        char *buf = nullptr;

        MmapReader file(filename);
        file.set_part_size(MB4);

        if (file.size() < sizeof(uint32_t))
        {
            return checksum;
        }
        if (not file.is_exist())
        {
            throw TgError::FILE_NOT_FOUND;
        }

        while (true)
        {
            auto len = file.nextpart(&buf);

            if (not buf)
            {
                break;
            }

            for (size_t i = 0; i < len; i += sizeof(uint32_t))
            {
                checksum ^= *reinterpret_cast<uint32_t*>(buf + i);
            }
        }

        if (not file.eof())
        {
            throw TgError::FILE_READ_ERROR;
        }
        return checksum;
    }
}

