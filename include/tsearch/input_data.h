#ifndef __INPUT_DATA_H__
#define __INPUT_DATA_H__

#include <string>
#include <map>

#include <tsearch/base.h>

namespace tsearch
{
    class InputData
    {
        enum TgMode mode;
        std::string file_name;
        std::string word;

    public:
        InputData();
        void parse(int const argc, char const **argv);
        enum TgMode get_mode(){ return mode; }
        std::string get_filename(){ return file_name; }

        typedef void (*HandlerFuncPtr)(
            InputData &
          , int const &
          , char const **
          , int &
        );

    private:
        static const std::map<std::string, HandlerFuncPtr> handler;

        friend void file_handler(InputData &, int const &, char const **, int &);
        friend void mode_handler(InputData &, int const &, char const **, int &);
        friend void word_handler(InputData &, int const &, char const **, int &);
        friend void help_handler(InputData &, int const &, char const **, int &);
    };
}

#endif // __INPUT_DATA_H__
