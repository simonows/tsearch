/*!
 * \file
 * \brief Implementation of the base procedures.
*/
#ifndef __TSEARCH_H__
#define __TSEARCH_H__

#include <tsearch/base.h>
#include <tsearch/input_data.h>

namespace tsearch
{
    size_t count_words(std::string filename, std::string word);

    unsigned long calc_checksum(std::string filename);


    class MmapReader
    {
    }
}

#endif // __TSEARCH_H__
