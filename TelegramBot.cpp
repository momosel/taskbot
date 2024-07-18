#include <tgbot/tgbot.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include <cstdlib> // Для replit

using namespace std;
using namespace TgBot;

map<int64_t, vector<string>> tasksMap;
map<int64_t, bool> addTaskMode;
map<int64_t, bool> deleteTaskMode;

// Функція для збереження завдань у файл
void saveTasksToFile() {
    ofstream outFile("tasks.txt");
    for (const auto& pair : tasksMap) {
        outFile << pair.first << ":";
        for (const auto& task : pair.second) {
            outFile << task << ",";
        }
        outFile << endl;
    }
}

// Функція для завантаження завдань із файлу
void loadTasksFromFile() {
    ifstream inFile("tasks.txt");
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        int64_t chatId;
        char colon;
        iss >> chatId >> colon;
        
        string task;
        vector<string> tasks;
        while (getline(iss, task, ',')) {
            if (!task.empty()) {
                tasks.push_back(task);
            }
        }
        tasksMap[chatId] = tasks;
    }
}

// Функція для відображення завдань
void showTasks(Bot& bot, const Message::Ptr& message) {
    int64_t chatId = message->chat->id;
    if (tasksMap[chatId].empty()) {
        bot.getApi().sendMessage(chatId, "Список справ порожній.");
        return;
    }

    ostringstream oss;
    oss << "Ваші справи:\n";
    for (size_t i = 0; i < tasksMap[chatId].size(); ++i) {
        oss << i + 1 << ". " << tasksMap[chatId][i] << "\n";
    }
    bot.getApi().sendMessage(chatId, oss.str());
}

int main() {

    TgBot::Bot bot("7225066297:AAF-2ifDCQRdqdZW07tOwHpEiMiakxninK4");
    
    //const char* token = getenv("7225066297:AAF-2ifDCQRdqdZW07tOwHpEiMiakxninK4");
    //if (!token) {
    //    cerr << "Помилка: BOT_TOKEN не встановлений." << endl;
    //    return 1;
    //}

    // Завантаження завдань із файлу при старті бота
    loadTasksFromFile();

    //TgBot::Bot bot(token);

    // Обробка команди /start
    bot.getEvents().onCommand("start", [&bot](const Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id, "Давайте заплануємо ваш день.\n"
                                                    "/addcase щоб додати справу.\n"
                                                    "/showcases щоб показати список справ.\n"
                                                    "/deletecase щоб видалити справу.");
        addTaskMode[message->chat->id] = false;
    });

    // Обробка команди /addcase
    bot.getEvents().onCommand("addcase", [&bot](const Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id, "Напишіть одну або декілька ваших справ. Введіть їх в одному повідомленні через кому");
        addTaskMode[message->chat->id] = true;
    });

    // Обробка команди /showcases
    bot.getEvents().onCommand("showcases", [&bot](const Message::Ptr& message) {
        showTasks(bot, message);
        addTaskMode[message->chat->id] = false;
    });

    // Обробка команди /deletecase
    bot.getEvents().onCommand("deletecase", [&bot](const Message::Ptr& message) {
        showTasks(bot, message);
        bot.getApi().sendMessage(message->chat->id, "Напишіть номери справ, які ви хочете видалити, через кому");
        addTaskMode[message->chat->id] = false;
        deleteTaskMode[message->chat->id] = true;
    });

    bot.getEvents().onAnyMessage([&bot](const Message::Ptr& message) {
        int64_t chatId = message->chat->id;

        if (addTaskMode[chatId]) 
        {
            istringstream iss(message->text);
            string task;
            while (getline(iss, task, ',')) 
            {
                tasksMap[chatId].push_back(task);
            }

            saveTasksToFile();
            bot.getApi().sendMessage(chatId, "Справу(и) додано");
            addTaskMode[chatId] = false;

        } 
        else if (deleteTaskMode[chatId]) 
        {
            vector<int> taskNums;
            istringstream iss(message->text);
            string num;
            while (getline(iss, num, ',')) 
            {
                try {
                    taskNums.push_back(stoi(num) - 1);
                } catch (const invalid_argument&) {
                    bot.getApi().sendMessage(chatId, "Введіть коректні номери справ через кому");
                    return;
                }
            }

            // Видалення завдань
            sort(taskNums.begin(), taskNums.end(), greater<int>());
            for (int taskNum : taskNums) 
            {
                if (taskNum < 0 || taskNum >= tasksMap[chatId].size()) 
                {
                    bot.getApi().sendMessage(chatId, "Некоректний номер справи");
                    return;
                } 
                else 
                {
                    tasksMap[chatId].erase(tasksMap[chatId].begin() + taskNum);
                }
            }

            saveTasksToFile();
            bot.getApi().sendMessage(chatId, "Справу(и) видалено");
            showTasks(bot, message);
            deleteTaskMode[chatId] = false;
        }
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
