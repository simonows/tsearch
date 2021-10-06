/*!
 * \file
 * \brief Base data types.
*/
#ifndef __TSEARCH_BASE_H__
#define __TSEARCH_BASE_H__

namespace tsearch
{
    /// Problem solving mode
    typedef enum class TgMode
    {
        NONE = 0,   ///< Complete solution
        WORD,       ///< Count words
        CHECKSUM,   ///< Calculate checksum
        HELP,       ///< Print help
    } TgMode;

    /// Specific errors
    typedef enum class TgError
    {
        OK = 0,
        BAD_ARGS,
        FILE_NAME_MISSING,
        MODE_MISSING,
        FILE_IS_MANDATORY,
        FILE_NOT_FOUND,
        FSTAT_FAILED,
        OPEN_FAILED,
    } TgError;

}

#endif // __TSEARCH_BASE_H__
