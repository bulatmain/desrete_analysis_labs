// #include <kmp_algo.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

using intType = size_t;

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

std::vector<intType> summirize(std::vector<intType> const& v) {
    std::vector<intType> summirized = {0};
    summirized.insert(summirized.end(), v.begin(), v.end());
    for (size_t i = 1; i < summirized.size(); ++i) {
        summirized[i] += summirized[i - 1];
    }
    return summirized;
}

std::tuple<size_t, size_t> getLineAndWord(
    std::vector<intType>::const_iterator const& next,
    std::vector<intType>::const_iterator const& begin,
    std::vector<intType> const& wordsTillLine
    ) {
    size_t line, word;
    auto distance = static_cast<size_t>(std::distance(begin, next));
    auto lineIt = std::upper_bound(wordsTillLine.begin(), wordsTillLine.end(), distance);
    lineIt--;

    line = static_cast<size_t>(std::distance(
        wordsTillLine.cbegin(),
        lineIt
    ));
    word = distance - static_cast<size_t>(wordsTillLine[line]);

    return std::make_tuple(line + 1, word + 1);
}

using NoNextPattern = std::runtime_error;

std::tuple<size_t, size_t> findNextPatternFrom(
    std::vector<intType>::const_iterator& begin,
    std::vector<intType> const& text,
    std::vector<intType> const& pattern,
    std::vector<intType> const& wordsTillLine
    ) {
        auto const next = std::search(begin, text.end(), pattern.begin(), pattern.end());
        if (next == text.end()) {
            begin = text.end();
            throw NoNextPattern("No next pattern found");
        }
        begin = next + 1;
        return getLineAndWord(next, text.cbegin(), wordsTillLine);
}

int main() {   
    std::vector<intType> pattern;

    readPattern(pattern);

    std::vector<intType> text, wordsInLines;

    readText(text, wordsInLines);

    auto wordsTillLine = summirize(wordsInLines);

    auto begin = text.cbegin();

    size_t line, word;
    while (begin != text.end()) {
        try {
            std::tie(line, word) = findNextPatternFrom(begin, text, pattern, wordsTillLine);
            std::cout << line << ", " << word << std::endl;
        } catch (NoNextPattern const& e) {
            break;
        }
    }

    return 0;
}