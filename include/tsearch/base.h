/*!
 * \file
 * \brief Base data types.
*/

#ifndef __TSEARCH_BASE_H__
#define __TSEARCH_BASE_H__

#include <system_error>

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
        MODE_WRONG,
        FILE_READ_ERROR,
    } TgError;


    /// Specific error override
    struct TgErrCategory: std::error_category
    {
        const char * name() const noexcept override;
        std::string message(int ev) const override;
    };

    /// override
    std::error_code make_error_code(tsearch::TgError);
}

namespace std
{
    template<>
    struct is_error_code_enum<tsearch::TgError>:
        true_type
    {};
}


#endif // __TSEARCH_BASE_H__

