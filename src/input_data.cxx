/*!
 * \file
 * \brief Realization of the command parser.
*/

#include <tsearch/tsearch.h>

static std::string const WORDS_PATTERN = "word";
static std::string const CHECKSUM_PATTERN = "checksum";

namespace tsearch
{
    void file_handler(InputData &, int const &, char const **, int &);
    void mode_handler(InputData &, int const &, char const **, int &);
    void word_handler(InputData &, int const &, char const **, int &);
    void help_handler(InputData &, int const &, char const **, int &);

    /// Handler map
    std::map<std::string, InputData::HandlerFuncPtr> const InputData::handler = {
        { "-f",     file_handler }
      , { "--file", file_handler }
      , { "-m",     mode_handler }
      , { "--mode", mode_handler }
      , { "-v",     word_handler }
      , { "-h",     help_handler }
      , { "--help", help_handler }
    };

    /*!
     * Parse command line arguments.
     *
     * \param[in] argc Command-line argument's count.
     * \param[in] argv Command-line arguments.
    */
    void InputData::parse(int const argc, char const **argv)
    {
        for (int i = 0; i < argc; i++)
        {
            auto it = handler.find(argv[i]);
            if (it != handler.end())
            {
                (*it).second(*this, argc, argv, i);
            }
            else
            {
                // TODO: make a warning
            }
        }
    }

    /*!
     * Validate command line arguments.
     *
     * \return validate report.
    */
    bool InputData::validate()
    {
        if (file_name.empty() || mode == TgMode::NONE)
        {
            return false;
        }
        if (mode == TgMode::WORD)
        {
            return !word.empty();
        }
        return true;
    }

    /*!
     * Processing of the file argument.
     *
     * \param[in] obj InputData object.
     * \param[in] argc Command-line argument's count.
     * \param[in] argv Command-line arguments.
     * \param[in] i iterator.
     *
     * \throw TgError::FILE_NAME_MISSING when a argv doen't contain a file name
    */
    void file_handler(
        InputData &obj
      , int const &argc
      , char const **argv
      , int &i
    ){
        if (argc > i + 1)
        {
            obj.file_name = argv[i + 1];
            i++;
        }
        else
        {
            throw TgError::FILE_NAME_MISSING;
        }
    }

    /*!
     * Processing of the help argument.
     *
     * \param[in] obj InputData object.
     * \param[in] argc Command-line argument's count.
     * \param[in] argv Command-line arguments.
     * \param[in] i iterator.
    */
    void help_handler(
        InputData &obj
      , int const &argc
      , char const **argv
      , int &i
    ){
        obj.mode = TgMode::HELP;
    }

    /*!
     * Processing of the word argument.
     *
     * \param[in] obj InputData object.
     * \param[in] argc Command-line argument's count.
     * \param[in] argv Command-line arguments.
     * \param[in] i iterator.
    */
    void word_handler(
        InputData &obj
      , int const &argc
      , char const **argv
      , int &i
    ){
        if (argc > i + 1)
        {
            obj.word = argv[i + 1];
            i++;
        }
        else
        {
            throw TgError::MODE_MISSING;
        }
    }

    /*!
     * Processing of the work-mode argument.
     *
     * \param[in] obj InputData object.
     * \param[in] argc Command-line argument's count.
     * \param[in] argv Command-line arguments.
     * \param[in] i iterator.
     *
     * \throw TgError::MODE_MISSING when a argv doen't contain a mode type
    */
    void mode_handler(
        InputData &obj
      , int const &argc
      , char const **argv
      , int &i
    ){
        if (argc <= i + 1)
        {
            throw TgError::MODE_MISSING;
        }
        if (WORDS_PATTERN == argv[i + 1])
        {
            obj.mode = TgMode::WORD;
            i++;
        }
        else if (CHECKSUM_PATTERN == argv[i + 1])
        {
            obj.mode = TgMode::CHECKSUM;
            i++;
        }
        else
        {
            throw TgError::MODE_WRONG;
        }
    }


    InputData::InputData() :
        mode(TgMode::NONE)
      , file_name("")
      , word("")
    {
    }

}
