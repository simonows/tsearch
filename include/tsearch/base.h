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
        FILE_NAME_MISSING,
        MODE_MISSING,
        FILE_IS_MANDATORY,
    } TgError;

}

#endif // __TSEARCH_BASE_H__
