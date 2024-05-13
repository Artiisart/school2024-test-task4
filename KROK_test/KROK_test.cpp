#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <regex>
struct Commit {// Структура коммита
    std::string username;
    std::string hash;
    std::string datetime;
};
// Функция проверяет, является ли данный символ латинской буквой/цифрой/знаком "_"
bool isLatLetOrNumOrUnderscore(char symbol) {
    if (std::isalpha(symbol))
        return true;

    if (std::isdigit(symbol))
        return true;

    if (symbol == '_')
        return true;

    return false;
}
bool isValidCommitDateTimeFormat(const std::string& date_time) {
    std::regex pattern(R"(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:[+-]?([0-9]*[.])?[0-9]+)");
    if (std::regex_match(date_time, pattern)) {// соответствует ли строка формату? Если да, то проверяем соответствуют ли формату числа в дате
        std::regex rgx_time(R"(\d{2})");
        std::smatch match_mth;// проверка соответствия месяцу, дню, часу и т.д.
        std::smatch match_d;
        std::smatch match_h;
        std::smatch match_s;
        std::smatch match_m;
        std::regex rgx_time_mm(R"([+-]?([0-9]*[.])?[0-9]+)");
        std::smatch match_mm;
        if (std::regex_search(date_time.begin() + 3, date_time.end(), match_mth, rgx_time) 
            && std::regex_search(date_time.begin() + 6, date_time.end(), match_d, rgx_time) 
            && std::regex_search(date_time.begin() + 10, date_time.end(), match_h, rgx_time) 
            && std::regex_search(date_time.begin() + 13, date_time.end(), match_m, rgx_time) 
            && std::regex_search(date_time.begin() + 15, date_time.end(), match_s, rgx_time) 
            && std::regex_search(date_time.begin() + 17, date_time.end(), match_mm, rgx_time_mm))
        {
            bool is_fit =((stoi(match_mth[0]) < 12) && (stoi(match_d[0]) < 31) && (stoi(match_h[0]) < 24)
                && (stoi(match_m[0]) < 60) && (stoi(match_s[0]) < 60) && (stof(match_mm[0]) < 60.0));
            return is_fit;
        }
        else
            return false;
    }
    
    
}

// Функция, которая берёт данные из файла
std::vector<Commit> readCommits(const std::string& filename) {
    std::vector<Commit> commits;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Commit commit;
        std::string username;
        std::string hash;
        std::string datetime;
        std::string test;
        int flag = 0;
        iss >> commit.username >> commit.hash >> commit.datetime;
        //Проверка коммита на соответствие критериям
        if(commit.hash.size() != 7 || !isalpha(commit.username[0]) || !isValidCommitDateTimeFormat(commit.datetime)){
            flag = 1;
        }
        else {
            for (auto a : commit.username) {
                if (!isLatLetOrNumOrUnderscore) {
                    flag = 1;
                }
            }
        }
        //был ли коммит с таким же хешом
        for (auto this_commit : commits) {
            if (commit.hash == this_commit.hash) {
                flag = 1;
            }
        }
        if (flag == 1) {
            continue;
        }
        commits.push_back(commit);
    }

    return commits;
}

// Функция для определения топ-3 контрибьютеров
std::vector<std::string> topContributors(const std::vector<Commit>& commits) {
    std::unordered_map<std::string, int> commitCount;

    // Подсчитываем количество коммитов для каждого пользователя
    for (const auto& commit : commits) {
        commitCount[commit.username]++;
        int a = commitCount[commit.username];
    }
    // Сортируем пользователей по количеству коммитов в убывающем порядке
    std::vector<std::pair<std::string, int>> sortedUsers(commitCount.begin(), commitCount.end());
    std::sort(sortedUsers.begin(), sortedUsers.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });

    // Определяем топ-3 пользователей
    std::vector<std::string> topUsers;
    for (int i = 0; i < std::min(3, static_cast<int>(sortedUsers.size())); ++i) {
        topUsers.push_back(sortedUsers[i].first);
    }

    return topUsers;
}

// Функция для записи результатов в файл
void writeResults(const std::vector<std::string>& topUsers, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& user : topUsers) {
        file << user << " ";
    }
}

int main() {
    // Считываем данные из файла
    std::vector<Commit> commits = readCommits("commits.txt");

    // Определяем топ-3 контрибьютеров
    std::vector<std::string> topUsers = topContributors(commits);

    // Записываем результаты в файл
    writeResults(topUsers, "result.txt");

    std::cout << "Top 3 contributors have been written to result.txt" << std::endl;

    return 0;
}
