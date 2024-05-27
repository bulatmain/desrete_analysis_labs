// #include <kmp_algo.hpp>

#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

#include <kmp.hpp>

using intType = uint32_t;

template <class T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
    for (auto const& e : v) {
        os << e << ' ';
    }
    os << std::endl;
    return os;
}

using UnableToKeepReadingException = std::runtime_error;

std::istringstream readLineInISS() {
    std::string line;
    std::getline(std::cin, line);
    if (!std::cin) {
        throw UnableToKeepReadingException("Error: can't read next line");
    }
    return std::istringstream(line);
}

std::vector<intType> readWordsInISS(std::istringstream& iss) {
    std::vector<intType> wordsInLine;
    intType next;
    while (iss >> next) {
        wordsInLine.push_back(next);
    }
    return wordsInLine;
}

std::vector<intType> readWordsInLine() {
    auto iss = readLineInISS();
    return readWordsInISS(iss);
}

void readPattern(std::vector<intType>& pattern) {
    try {
        pattern = readWordsInLine();
    } catch (UnableToKeepReadingException const& e) {
        return;
    }
}

void readText(std::vector<intType>& text, std::vector<intType>& wordsInLines) {
    while (true) {
        try {
            auto wordsInNewline = readWordsInLine();
            wordsInLines.push_back(
                static_cast<intType>(wordsInNewline.size())
            );
            text.insert(
                text.end(), 
                wordsInNewline.begin(), 
                wordsInNewline.end()
            );
        } catch (UnableToKeepReadingException const& e) {
            break;
        }
    }
}

std::vector<intType> summarize(std::vector<intType> const& v) {
    std::vector<intType> summarized = {0};
    summarized.insert(summarized.end(), v.begin(), v.end());
    for (size_t i = 1; i < summarized.size(); ++i) {
        summarized[i] += summarized[i - 1];
    }
    return summarized;
}

std::tuple<size_t, size_t> getLineAndWordFromOccurence(size_t occurence, std::vector<intType> const& wordsTillLine) {
    size_t line, word;
    auto lineIt = std::upper_bound(wordsTillLine.begin(), wordsTillLine.end(), occurence);
    lineIt--;

    line = static_cast<size_t>(std::distance(
        wordsTillLine.cbegin(),
        lineIt
    ));
    word = occurence - static_cast<size_t>(wordsTillLine[line]);

    return std::make_tuple(line + 1, word + 1);
}

int main() {   
    std::vector<intType> pattern;

    readPattern(pattern);

    std::vector<intType> text;
    std::vector<intType> wordsInLines;

    readText(text, wordsInLines);

    auto wordsTillLine = summarize(wordsInLines);

    cust::KnuthMorrisPrattAlgorithm<intType> kmp(pattern, text);

    auto occurences = kmp.findAllOccurences();

    size_t line, word;
    for (auto const& occurence : occurences) {
        // line = 1, word = occurence + 1;
        std::tie(line, word) = getLineAndWordFromOccurence(occurence, wordsTillLine);
        std::cout << line << ", " << word << std::endl;
    }

    return 0;
}