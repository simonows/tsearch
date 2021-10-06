/*!
 * \file
 * \brief Implementation of the base procedures.
*/
#ifndef __TSEARCH_H__
#define __TSEARCH_H__

#include <tsearch/base.h>
#include <tsearch/input_data.h>

#define CASE_SENSITIVE

namespace tsearch
{
    size_t count_words(std::string filename, std::string word);

    unsigned long calc_checksum(std::string filename);


    class MmapReader
    {
        std::string file_name;
        std::string word;
        std::string btw_word;
        char *buf;
        size_t buf_len;
        size_t file_len;
        size_t offset;
        int fd;
        size_t part_size;

        MmapReader(const MmapReader&);
        void operator=(const MmapReader&);

    public:
        MmapReader(std::string const file_name, std::string const word);
        ~MmapReader();
        bool is_exist() { return fd != -1; }
        bool betweenbuf(std::string &);
        size_t nextpart(char **);
        void set_part_size(size_t size) { part_size = size; }
    };
}

#endif // __TSEARCH_H__
