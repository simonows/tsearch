/*!
 * \file
 * \brief Realization of specific error.
*/

#include <tsearch/base.h>

namespace tsearch
{
    const char *TgErrCategory::name() const noexcept
    {
        return "test";
    }

    std::string TgErrCategory::message(int ev) const
    {
        switch (static_cast<TgError>(ev))
        {
            case TgError::OK:
                return "ok";

            case TgError::BAD_ARGS:
                return "wrong arguments";

            case TgError::FILE_NAME_MISSING:
                return "a file name missing";

            case TgError::MODE_MISSING:
                return "a mode argument is missing";

            case TgError::FILE_IS_MANDATORY:
                return "file option is mandatory";

            case TgError::FILE_NOT_FOUND:
                return "file not found";

            case TgError::FSTAT_FAILED:
                return "fstat failed";

            case TgError::OPEN_FAILED:
                return "open failed";

            case TgError::MODE_WRONG:
                return "mode is wrong";

            case TgError::FILE_READ_ERROR:
                return "Reading the file failed with an error. "
                       "The result is incomplete";

            default:
                return "(unrecognized error)";
        }
    }

    const tsearch::TgErrCategory theTgErrCategory {};

    std::error_code make_error_code(tsearch::TgError e)
    {
        return {static_cast<int>(e), theTgErrCategory};
    }

}



