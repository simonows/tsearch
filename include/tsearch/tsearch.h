/*!
 * \file
 * \brief Implementation of the base procedures.
*/

#ifndef __TSEARCH_H__
#define __TSEARCH_H__

#include <tsearch/base.h>
#include <tsearch/input_data.h>

//#define CASE_SENSITIVE

namespace tsearch
{
    size_t count_words(std::string filename, std::string word);

    unsigned int calc_checksum(std::string filename);

    /*!
     * \brief Process file in memory
    */
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
        MmapReader(std::string const _file_name);
        MmapReader(std::string const _file_name, std::string const _word)
          : MmapReader(_file_name)
        {}
        ~MmapReader();
        bool is_exist() { return fd != -1; }
        bool eof() { return offset == file_len; }
        bool betweenbuf(std::string &);
        size_t nextpart(char **);
        size_t size() { return file_len; }
        void set_part_size(size_t size) { part_size = size; }
    };
}

#endif // __TSEARCH_H__
