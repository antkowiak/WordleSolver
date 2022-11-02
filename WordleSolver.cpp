//
// Written by Ryan Antkowiak 
// 2022-01-30
//

#include <algorithm>
#include <fstream>
#include <iostream>

#include "WordleSolver.h"

namespace
{
    constexpr const std::size_t DEFAULT_WORD_SIZE = 5;
    constexpr const double DUPLICATE_CHARACTER_PENALTY = 0.2;
    constexpr const char* WORDLIST_FILE = R"(wordlist_all.txt)";
}

WordleSolver::WordleSolver()
    : WordleSolver(DEFAULT_WORD_SIZE, WORDLIST_FILE)
{
}

WordleSolver::WordleSolver(std::size_t wordSize, const std::string& wordlistFile)
    : m_wordSize(wordSize)
{
    loadWordsFromInputFile(wordlistFile);
}

bool WordleSolver::validateAndSanitizeAbsentChars(std::string& absentChars) const
{
    std::string s;

    for (char c : absentChars)
    {
        c = std::toupper(c);
        if (c >= 'A' && c <= 'Z')
            s += c;
        else
            return false;
    }

    absentChars = s;
    return true;
}

bool WordleSolver::validateAndSanitizeCorrectPositionedChars(std::string& chars) const
{
    if (chars.size() % 2 != 0)
        return false;

    std::string s;

    for (std::size_t index = 0 ; index < chars.size() ; ++index)
    {
        char pos = chars[index];
        ++index;
        char letter = std::toupper(chars[index]);
        if (pos >= '0' && pos <= '9' && letter >= 'A' && letter <= 'Z' && (pos - '0' < m_wordSize))
        {
            s += pos;
            s += letter;
        }
        else
            return false;
    }

    chars = s;
    return true;
}

bool WordleSolver::validateAndSanitizeWrongPositionedChars(std::string& chars) const
{
    if (chars.size() % 2 != 0)
        return false;

    std::string s;

    for (std::size_t index = 0 ; index < chars.size() ; ++index)
    {
        char pos = chars[index];
        ++index;
        char letter = std::toupper(chars[index]);
        if (pos >= '0' && pos <= '9' && letter >= 'A' && letter <= 'Z' && (pos - '0' < m_wordSize))
        {
            s += pos;
            s += letter;
        }
        else
            return false;
    }

    chars = s;
    return true;
}

void WordleSolver::addAbsentChar(char absentChar)
{
    if (std::find(m_absentChars.begin(), m_absentChars.end(), absentChar) == m_absentChars.end())
        m_absentChars.push_back(absentChar);
}

void WordleSolver::addAbsentChars(const std::vector<char>& absentChars)
{
    for (const auto& absentChar : absentChars)
        addAbsentChar(absentChar);
}

void WordleSolver::addAbsentChars(const std::string& absentChars)
{
    for (char c : absentChars)
        addAbsentChar(c);
}

void WordleSolver::addCorrectPositionedChar(std::size_t position, char c)
{
    std::pair<std::size_t, char> p{ position, c };
    if (std::find(m_correctPositionChars.begin(), m_correctPositionChars.end(), p) == m_correctPositionChars.end())
        m_correctPositionChars.push_back(p);
}

void WordleSolver::addCorrectPositionedChars(const std::vector<std::pair<std::size_t, char>>& chars)
{
    for (const auto& cpc : chars)
        addCorrectPositionedChar(cpc.first, cpc.second);
}

void WordleSolver::addCorrectPositionedChars(const std::string& chars)
{
    if (chars.size() % 2 != 0)
        return;

    for (std::size_t index = 0 ; index < chars.size() ; ++index)
    {
        char pos = chars[index];
        ++index;
        char letter = chars[index];
        addCorrectPositionedChar(pos - '0', letter);
    }
}

void WordleSolver::addWrongPositionedChar(std::size_t wrongPosition, char c)
{
    std::pair<std::size_t, char> p{ wrongPosition, c };
    if (std::find(m_wrongPositionChars.begin(), m_wrongPositionChars.end(), p) == m_wrongPositionChars.end())
        m_wrongPositionChars.push_back(p);
}

void WordleSolver::addWrongPositionedChars(const std::vector<std::pair<std::size_t, char>>& chars)
{
    for (const auto& wpc : chars)
        addWrongPositionedChar(wpc.first, wpc.second);
}

void WordleSolver::addWrongPositionedChars(const std::string& chars)
{
    if (chars.size() % 2 != 0)
        return;

    for (std::size_t index = 0 ; index < chars.size() ; ++index)
    {
        char pos = chars[index];
        ++index;
        char letter = chars[index];
        addWrongPositionedChar(pos - '0', letter);
    }
}

void WordleSolver::filterCandidates()
{
    std::vector<std::string> filteredWords;

    for (const auto& word : m_candidateWords)
    {
        if (!wordSatisfiesConstraintAbsentChars(word))
            continue;
        if (!wordSatisfiesConstraintCorrectPositionChars(word))
            continue;
        if (!wordSatisfiesConstraintWrongPositionChars(word))
            continue;

        filteredWords.push_back(word);
    }

    m_candidateWords = filteredWords;

    // Re-build the score table
    buildScoreTable();
    scoreWords();
}

void WordleSolver::printCandidates(std::size_t n) const
{
    const int maxScore = calculateMaxScore();
    const double fmaxScore = static_cast<double>(maxScore);

    std::size_t count{ 0 };

    for (const auto& candidate : m_scoredWords)
    {
        ++count;

        if (n != 0 && count > n)
            break;

        const double fscore = (fmaxScore > 0.0) ? (static_cast<double>(candidate.first) / fmaxScore) : (0.0);
        std::cout << candidate.second << ", " << std::to_string(fscore) 
            << " [" << std::to_string(candidate.first) << "/" << maxScore << "]\n";
    }

    std::cout.flush();
}

bool WordleSolver::wordSatisfiesConstraintAbsentChars(const std::string& word) const
{
    for (std::size_t index = 0; index < m_wordSize && index < word.size(); ++index)
        if (std::find(m_absentChars.begin(), m_absentChars.end(), word[index]) != m_absentChars.end())
            return false;
    return true;
}

bool WordleSolver::wordSatisfiesConstraintCorrectPositionChars(const std::string& word) const
{
    for (const auto& cpc : m_correctPositionChars)
    {
        if (cpc.first > m_wordSize || cpc.first > word.size())
            return false;
        if (word[cpc.first] != cpc.second)
            return false;
    }
    return true;
}

bool WordleSolver::wordSatisfiesConstraintWrongPositionChars(const std::string& word) const
{
    for (const auto& wpc : m_wrongPositionChars)
    {
        if (wpc.first > m_wordSize || wpc.first > word.size())
            return false;
        if (word[wpc.first] == wpc.second)
            return false;
        if (word.find(wpc.second) == std::string::npos)
            return false;
    }
    return true;
}

void WordleSolver::loadWordsFromInputFile(const std::string& wordlistFile)
{
    m_candidateWords.clear();
    try
    {
        std::ifstream ifs;
        ifs.open(wordlistFile);
        while (ifs.good() && !ifs.eof())
        {
            std::string line;
            std::getline(ifs, line);
            const std::string word = sanitizeInput(line);
            if (word.size() == m_wordSize)
                m_candidateWords.push_back(word);
        }
        ifs.close();
    }
    catch (...) {}
}

std::string WordleSolver::sanitizeInput(const std::string& line) const
{
    std::string word;

    for (char c : line)
    {
        c = std::toupper(c);
        if (c >= 'A' && c <= 'Z')
        word += c;
    }

    return word;
}

void WordleSolver::buildScoreTable()
{
    m_scoreTable.clear();
    m_scoreTable.resize(m_wordSize);

    for (const auto& word : m_candidateWords)
        for (std::size_t index = 0; index < m_wordSize && index < word.size(); ++index)
            m_scoreTable[index][word[index]]++;
}

int WordleSolver::scoreWord(const std::string& word) const
{
    int score{ 0 };
    std::map<char, std::size_t> charFreqCount;

    for (std::size_t index = 0; index < m_wordSize && index < word.size(); ++index)
    {
        score += m_scoreTable[index].at(word[index]);
        charFreqCount[word[index]]++;
    }

    // Check for duplicated character, and if found, subtract a penalty
    if (charFreqCount.size() < word.size())
    {
        const std::size_t charDelta = word.size() - charFreqCount.size();
        double penaltyMultiplier = 1.0 - (  static_cast<double>(charDelta) * DUPLICATE_CHARACTER_PENALTY);
        score = static_cast<int>(penaltyMultiplier * static_cast<double>(score));
    }

    return score;
}

void WordleSolver::scoreWords()
{
    m_scoredWords.clear();

    for (const auto& candidate : m_candidateWords)
        m_scoredWords.push_back({scoreWord(candidate), candidate});

    std::sort(m_scoredWords.begin(), m_scoredWords.end(),
        [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b)
        {
            if (a.first == b.first)
                return a.second < b.second;
            return a.first > b.first;
        });
}

int WordleSolver::calculateMaxScore() const
{
    int maxScore{ 0 };

    for (const auto& place : m_scoreTable)
    {
        int maxAtPlace = 0;
        for (const auto& letter : place)
        {
            maxAtPlace = std::max(maxAtPlace, letter.second);
        }
        maxScore += maxAtPlace;
    }

    return maxScore;
}

