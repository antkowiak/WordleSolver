//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
// 2022-01-30
//

#include <iostream>

#include "WordleSolver.h"

void print_help(const char * progPath, const char * msg = nullptr)
{
    std::cout << "Usage: " << progPath << " <options>\n\n";
    std::cout << "    -h or -?      - Display help\n";
    std::cout << "    -n <number>   - Number of results to display (default: 20)\n";
    std::cout << "    -f <filename> - File containing list of words (default: wordlist_all.txt)\n";
    std::cout << "    -z <number>   - Word Size, Number of letters in the words (default: 5)\n";
    std::cout << "    -a <letters>  - Specify letters that should be absent from any solutions\n";
    std::cout << "    -c <guesses>  - Specify an index and letter pair for correctly positioned letters\n";
    std::cout << "    -w <guesses>  - Specify an index and letter pair for wrongly positioned letters\n";

    if (msg)
        std::cout << "\nError: " << msg << "\n";

    std::cout << std::endl;
}

int main(int argc, const char* argv[])
{
    std::size_t numResultsToShow = 20;
    std::string filename = "wordlist_all.txt";
    std::size_t wordSize = 5;
    std::string absentChars = "";
    std::string correctChars = "";
    std::string wrongChars = "";

    for (int index = 1 ; index < argc ; ++index)
    {
        const std::string param(argv[index]);
        if (param == "-h" || param == "-?")
        {
            print_help(argv[0]);
            return EXIT_SUCCESS;
        }
        else if (param == "-f")
        {
            if (index + 1 < argc) { ++index; filename = std::string(argv[index]); }
            else { print_help(argv[0], "No filename specified."); return EXIT_FAILURE; }
        }
        else if (param == "-n")
        {
            if (index + 1 < argc) { ++index; numResultsToShow = std::atol(argv[index]); }
            else { print_help(argv[0], "Number of results not specified."); return EXIT_FAILURE; }
        }
        else if (param == "-z")
        {
            if (index + 1 < argc) { ++index; wordSize = std::atol(argv[index]); }
            else { print_help(argv[0], "Word size not specified."); return EXIT_FAILURE; }
        }
        else if (param == "-a")
        {
            if (index + 1 < argc) { ++index; absentChars = std::string(argv[index]); }
            else { print_help(argv[0], "Absent letters not specified."); return EXIT_FAILURE; }
        }
        else if (param == "-c")
        {
            if (index + 1 < argc) { ++index; correctChars = std::string(argv[index]); }
            else { print_help(argv[0], "Correctly positioned letters not specified."); return EXIT_FAILURE; }
        }
        else if (param == "-w")
        {
            if (index + 1 < argc) { ++index; wrongChars = std::string(argv[index]); }
            else { print_help(argv[0], "Wrongly positioned letters not specified."); return EXIT_FAILURE; }
        }
        else
        {
            print_help(argv[0], "Invalid option.");
            return EXIT_FAILURE;
        }
    }

    WordleSolver ws(wordSize, filename);

    if (!ws.validateAndSanitizeAbsentChars(absentChars)) { print_help(argv[0], "Invalid absent letters."); return EXIT_FAILURE; }
    if (!ws.validateAndSanitizeCorrectPositionedChars(correctChars)) { print_help(argv[0], "Invalid correctly positioned letters."); return EXIT_FAILURE; }
    if (!ws.validateAndSanitizeWrongPositionedChars(wrongChars)) { print_help(argv[0], "Invalid wrongly positioned letters."); return EXIT_FAILURE; }

    ws.addAbsentChars(absentChars);
    ws.addCorrectPositionedChars(correctChars);
    ws.addWrongPositionedChars(wrongChars);

    ws.filterCandidates();
    ws.printCandidates(numResultsToShow);

    return EXIT_SUCCESS;
}
