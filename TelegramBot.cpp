#include <tgbot/tgbot.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using namespace TgBot;

int main() {
    TgBot::Bot bot("7225066297:AAF-2ifDCQRdqdZW07tOwHpEiMiakxninK4");

    vector<string> cases;
    bool canAddCases = false;
    bool awaitingDeletion = false;

    //start
    bot.getEvents().onCommand("start", [&bot, &canAddCases, &awaitingDeletion](const Message::Ptr& message) {
        canAddCases = false;  
        awaitingDeletion = false;
        string welcomeMessage = "Давайте заплануємо ваш день\n";
        welcomeMessage += "Команди:\n";
        welcomeMessage += "/addcase - Додати справу\n";
        welcomeMessage += "/viewcases - Переглянути список справ\n";
        welcomeMessage += "/deletecase - Видалити справу за номером";
        bot.getApi().sendMessage(message->chat->id, welcomeMessage);
    });

    //addcase
    bot.getEvents().onCommand("addcase", [&bot, &canAddCases](const Message::Ptr& message) {
        canAddCases = true;
        bot.getApi().sendMessage(message->chat->id, "Напишіть одну або кілька ваших справ");
    });

    //viewcases
    bot.getEvents().onCommand("viewcases", [&bot, &cases](const Message::Ptr& message) {
        if (cases.empty()) {
            bot.getApi().sendMessage(message->chat->id, "Список справ порожній");
        } else {
            string caseList = "Ваш список справ:\n";
            for (size_t i = 0; i < cases.size(); ++i) {
                caseList += to_string(i + 1) + ". " + cases[i] + "\n";
            }
            bot.getApi().sendMessage(message->chat->id, caseList);
        }
    });

    //deletecase
    bot.getEvents().onCommand("deletecase", [&bot, &cases, &awaitingDeletion](const Message::Ptr& message) {
        if (cases.empty()) {
            bot.getApi().sendMessage(message->chat->id, "Список справ порожній");
            return;
        }
        awaitingDeletion = true;
        string caseList = "Ваш список справ:\n";
        for (size_t i = 0; i < cases.size(); ++i) {
            caseList += to_string(i + 1) + ". " + cases[i] + "\n";
        }
        bot.getApi().sendMessage(message->chat->id, caseList + "\nВведіть номер справи, яку ви хочете видалити");
    });

    //все соо
    bot.getEvents().onAnyMessage([&bot, &cases, &canAddCases, &awaitingDeletion](const Message::Ptr& message) {
        if (StringTools::startsWith(message->text, "/")) {
            return;
        }

        //удаление дела
        if (awaitingDeletion) {
            istringstream iss(message->text);
            int caseNumber;
            if (!(iss >> caseNumber) || caseNumber <= 0 || static_cast<size_t>(caseNumber) > cases.size()) {
                bot.getApi().sendMessage(message->chat->id, "Невірний номер справи. Будь ласка, введіть правильний номер.");
            } else {
                cases.erase(cases.begin() + caseNumber - 1);
                bot.getApi().sendMessage(message->chat->id, "Справу видалено.");
            }
            awaitingDeletion = false;
            return;
        }

        if (canAddCases) {
            try {
                cases.push_back(message->text);
                bot.getApi().sendMessage(message->chat->id, "Справу додано: " + message->text);
            } catch (const std::exception& e) {
                bot.getApi().sendMessage(message->chat->id, "Помилка при додаванні справи: " + string(e.what()));
            }
        } else {
            bot.getApi().sendMessage(message->chat->id, "Спочатку використовуйте команду /addcase для додавання справ");
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
    } catch (const std::exception& e) {
        printf("Standard exception: %s\n", e.what());
    } catch (...) {
        printf("Unknown error occurred\n");
    }

    return 0;
}
