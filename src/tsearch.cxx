/*!
 * \file
 * \brief Realization of the base procedures.
*/
#include <ctype.h>
#include <tsearch/tsearch.h>

static size_t const ALPHA_SKIP_SIZE = 256;

static size_t alpha_skip_lookup[ALPHA_SKIP_SIZE];
static size_t *find_skip_lookup;

size_t ag_max(size_t a, size_t b)
{
    if (b > a)
    {
        return b;
    }
    return a;
}


/* Boyer-Moore strstr */
const char *bm_strnstr(
    const char *s
  , const char *find
  , const size_t s_len
  , const size_t f_len
  , const size_t alpha_skip_lookup[]
  , const size_t *find_skip_lookup
){
    ssize_t i;
    size_t pos = f_len - 1;

    while (pos < s_len)
    {
        for (
            i = f_len - 1;
            i >= 0 && tolower(s[pos]) == find[i];
            pos--, i--
        );
        if (i < 0) {
            return s + pos + 1;
        }
        pos += ag_max(alpha_skip_lookup[(unsigned char)s[pos]], find_skip_lookup[i]);
    }

    return nullptr;
}


void generate_alpha_skip(char const *find, size_t f_len, size_t skip_lookup[])
{
    size_t i;

    for (i = 0; i < ALPHA_SKIP_SIZE; i++)
    {
        skip_lookup[i] = f_len;
    }

    f_len--;

    for (i = 0; i < f_len; i++)
    {
        skip_lookup[(unsigned char)find[i]] = f_len - i;
    }
}


int is_prefix(const char *s, const size_t s_len, const size_t pos)
{
    size_t i;

    for (i = 0; pos + i < s_len; i++)
    {
        if (s[i] != s[i + pos])
        {
            return 0;
        }
    }

    return 1;
}


size_t suffix_len(const char *s, const size_t s_len, const size_t pos)
{
    size_t i;

    for (i = 0; i < pos; i++)
    {
        if (s[pos - i] != s[s_len - i - 1])
        {
            break;
        }
    }

    return i;
}


void generate_find_skip(const char *find, const size_t f_len, size_t **skip_lookup)
{
    size_t i;
    size_t s_len;
    size_t *sl = ag_malloc(f_len * sizeof(size_t));
    *skip_lookup = sl;
    size_t last_prefix = f_len;

    for (i = last_prefix; i > 0; i--)
    {
        if (is_prefix(find, f_len, i))
        {
            last_prefix = i;
        }
        sl[i - 1] = last_prefix + (f_len - i);
    }

    for (i = 0; i < f_len; i++)
    {
        s_len = suffix_len(find, f_len, i);
        if (find[i - s_len] != find[f_len - 1 - s_len])
        {
            sl[f_len - 1 - s_len] = f_len - 1 - i + s_len;
        }
    }
}



namespace tsearch
{
    size_t count_words(std::string filename, std::string word)
    {
        char *buf = nullptr;
        size_t count = 0;

        MmapReader file(filename, word);
        std::string trans;

        if (word.size() == 0)
        {
            return count;
        }
        if (!file.is_exist())
        {
            throw TgError::FILE_NOT_FOUND;
        }

        generate_alpha_skip(word.c_str(), word.size(), alpha_skip_lookup);
        find_skip_lookup = nullptr;
        generate_find_skip(word.c_str(), word.size(), &find_skip_lookup);

        do
        {
            auto len = file.nextpart(&buf);
            char *match = nullptr;

            if (file.betweenbuf(trans))
            {
                match = bm_strnstr(
                    trans.c_str()
                  , word.c_str()
                  , trans.size()
                  , word.size()
                  , alpha_skip_lookup
                  , find_skip_lookuph
                );
                if (match != nullptr)
                {
                    count++;
                }
            }

            match = buf;
            while (buf - match < len)
            {
                match = bm_strnstr(
                    buf
                  , word.c_str()
                  , len
                  , word.size()
                  , alpha_skip_lookup
                  , find_skip_lookuph
                );
                if (match == nullptr)
                {
                    break;
                }
                count++;
            }
        } while (buf != nullptr);

        return count;
    }

    unsigned long calc_checksum(std::string filename)
    {
        return 0;
    }
}

