/*!
 * \file
 * \brief Main source.
 *
 * Тестовое задание:
 * Напишите (консольную) программу, принимающую на вход имя файла и набор
 * параметров. В зависимости от параметров программа должна работать в трёх
 * режимах:
 *
 *   1. test -f Test.tst -m words -v mother
 *     — печатает количество слов «mother» в файле «Test.tst»
 *
 *   2. test -f Test.tst -m checksum
 *     — печатает 32-битную чексумму, рассчитанную по алгоритму
 *       checksum = word1 + word2 + ... + wordN
 *       (word1..wordN – 32-хбитные слова, представляющие содержимое файла)
 *
 *   3. test -h — печатает информацию о программе и описание параметров.
 *
 * При написании тестового задания мы ожидаем увидеть пример качественного
 * production-кода на С++ (а не «чистом» С, несмотря на некоторую избыточность
 * С++ для данной задачи),  в частности:
 *   - соблюдение единого стиля кодирования
 *   - разумное расходование памяти
 *   - расчёт на производительность
 *   - расчёт на расширяемость
 *   - следование принципам ООП там, где это уместно
 *   - использование modern C++ best practices
*/

#include <iostream>
#include <exception>

#include <tsearch/tsearch.h>

//! Help' printable string.
static std::string const HELP =
    "Usage: ./test -f FILE [OPTION...]"
    "\n    -f, --file FILE_NAME"
    "\n        input file (mandatory)"
    "\n    -m, --mode [word | checksum ]"
    "\n        work mode (mandatory)."
    "\n    -v, WORD"
    "\n        word for search (mandatory)."
    "\n    -h, --help"
    "\n        prints help."
;

/*
 * NAME
 *     test, testsearcher - The Test Searcher. Like ag, but simply.
 *
 * SYNOPSIS
 *     ./test -f FILE [OPTION...]
 *
 * OPTIONS
 *     -f, --file FILE_NAME
 *         input file (mandatory).
 *
 *     -m, --mode [word | checksum ]
 *         work mode (mandatory).
 *
 *     -v WORD
 *         word for search (mandatory).
 *
 *     -h, --help
 *         prints help.
*/
int main(int const argc, char const **argv)
{
    tsearch::InputData tg_args;

    try
    {
        tg_args.parse(argc, argv);

        if (not tg_args.validate())
        {
            throw tsearch::TgError::BAD_ARGS;
        }

        switch (tg_args.get_mode())
        {
            case tsearch::TgMode::HELP:
            {
                std::cout << HELP << std::endl;
                exit(0);
            }
            case tsearch::TgMode::WORD:
            {
                auto count = tsearch::count_words(
                    tg_args.get_filename()
                  , tg_args.get_word()
                );
                std::cout << count << std::endl;
                break;
            }
            case tsearch::TgMode::CHECKSUM:
            {
                auto checksum = tsearch::calc_checksum(tg_args.get_filename());
                std::cout << std::hex << checksum << std::endl;
                break;
            }
            default:
                break;
        }
    }
    catch (tsearch::TgError &error)
    {
        std::error_code ec = error;
        std::cerr << "Error: " << ec.message() << std::endl;
        exit(ec.value());
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        exit(1);
    }

    return 0;
}

