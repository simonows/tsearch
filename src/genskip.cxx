/*!
 * \file
 * \brief Realization of skipper of Boyer-Moore algo.
*/

#ifndef CASE_SENSITIVE
#include <ctype.h>
#endif
#include <sys/types.h>


static size_t const ALPHA_SKIP_SIZE = 256;

size_t alpha_skip_lookup[ALPHA_SKIP_SIZE];
size_t *find_skip_lookup;

/*!
 * Generate skip sequence for Boyer-Moore strstr.
 *
 * \param[in] find Search string.
 * \param[in] f_len c-string size.
 * \param[out] skip_lookup Skip sequence.
*/
void generate_alpha_skip(
    char const *find
  , size_t f_len
  , size_t skip_lookup[]
) noexcept {
    size_t i;

    for (i = 0; i < ALPHA_SKIP_SIZE; i++)
    {
        skip_lookup[i] = f_len;
    }

    f_len--;

    for (i = 0; i < f_len; i++)
    {
#ifndef CASE_SENSITIVE
        skip_lookup[static_cast<unsigned char>(tolower(find[i]))] = f_len - i;
        skip_lookup[static_cast<unsigned char>(toupper(find[i]))] = f_len - i;
#else
        skip_lookup[static_cast<unsigned char>(find[i])] = f_len - i;
#endif
    }
}


/*!
 * Prefix check.
 *
 * \param[in] s Search string.
 * \param[in] s_len c-string size.
 * \param[in] pos index.
*/
int is_prefix(
    const char *s
  , const size_t s_len
  , const size_t pos
) noexcept {
    size_t i;

    for (i = 0; pos + i < s_len; i++)
    {
#ifndef CASE_SENSITIVE
        if (tolower(s[i]) != tolower(s[i + pos]))
#else
        if (s[i] != s[i + pos])
#endif
        {
            return 0;
        }
    }

    return 1;
}

/*!
 * Suffix check.
 *
 * \param[in] s Search string.
 * \param[in] s_len c-string size.
 * \param[in] pos index.
*/
size_t suffix_len(
    const char *s
  , const size_t s_len
  , const size_t pos
) noexcept {
    size_t i;

    for (i = 0; i < pos; i++)
    {
#ifndef CASE_SENSITIVE
        if (tolower(s[pos - i]) != tolower(s[s_len - i - 1]))
#else
        if (s[pos - i] != s[s_len - i - 1])
#endif
        {
            break;
        }
    }

    return i;
}

/*!
 * Generate skip sequence for Boyer-Moore strstr base on search string.
 *
 * \param[in] find Search string.
 * \param[in] f_len c-string size.
 * \param[out] skip_lookup Skip sequence.
*/
void generate_find_skip(
    const char *find
  , const size_t f_len
  , size_t *skip_lookup
) noexcept {
    size_t i;
    size_t s_len;
    size_t *sl = skip_lookup;
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

