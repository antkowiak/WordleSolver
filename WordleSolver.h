//
// Written by Ryan Antkowiak 
// 2022-01-30
//

#pragma once

#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

class WordleSolver
{
public:
    WordleSolver();
    WordleSolver(std::size_t wordSize, const std::string& wordlistFile);

    bool validateAndSanitizeAbsentChars(std::string& absentChars) const;
    bool validateAndSanitizeCorrectPositionedChars(std::string& chars) const;
    bool validateAndSanitizeWrongPositionedChars(std::string& chars) const;

    void addAbsentChar(char absentChar);
    void addAbsentChars(const std::vector<char>& absentChars);
    void addAbsentChars(const std::string& absentChars);

    void addCorrectPositionedChar(std::size_t position, char c);
    void addCorrectPositionedChars(const std::vector<std::pair<std::size_t, char>>& chars);
    void addCorrectPositionedChars(const std::string& chars);
    
    void addWrongPositionedChar(std::size_t wrongPosition, char c);
    void addWrongPositionedChars(const std::vector<std::pair<std::size_t, char>>& chars);
    void addWrongPositionedChars(const std::string& chars);

    void filterCandidates();
    void printCandidates(std::size_t n = 20) const;

private:
    bool wordSatisfiesConstraintAbsentChars(const std::string& word) const;
    bool wordSatisfiesConstraintCorrectPositionChars(const std::string& word) const;
    bool wordSatisfiesConstraintWrongPositionChars(const std::string& word) const;

    void loadWordsFromInputFile(const std::string& wordlistFile);
    std::string sanitizeInput(const std::string& line) const;
    void buildScoreTable();
    int scoreWord(const std::string& word) const;
    void scoreWords();
    int calculateMaxScore() const;

private:
    std::size_t m_wordSize{};
    std::vector<std::map<char, int>> m_scoreTable{};
    std::vector<std::string> m_candidateWords{};
    std::vector<std::pair<int, std::string>> m_scoredWords;

    std::vector<char> m_absentChars{};
    std::vector<std::pair<std::size_t, char>> m_correctPositionChars{};
    std::vector<std::pair<std::size_t, char>> m_wrongPositionChars{};
};
