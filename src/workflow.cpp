/*
 * @brief Основной цикл для запуска перевода и инициализации взаимодействия
 * @author Степанов Михаил, Казаченко Роман
 * @version 1.0
 */

#include "reactor.h"
#include "translator.h"
#include <iostream>
#include <thread>

enum Command {
    kTalk,
    kListen,
    kOn,
    kOff,
    kExit
};

std::deque<Command> commandQueue;

bool console    = true;
bool is_working = true;

void listenConsole() {
    while (console) {
        std::string command;
        std::cin >> command;
        if (command == "talk")
            commandQueue.push_back(kTalk);
        else if (command == "listen")
            commandQueue.push_back(kListen);
        else if (command == "on")
            commandQueue.push_back(kOn);
        else if (command == "off")
            commandQueue.push_back(kOff);
        else if (command == "exit") {
            commandQueue.push_back(kExit);
            console = false;
        }
    }
}

int main() {
    std::deque<pantomime::Video> pantomime;
    std::deque<syllable::Noise> sound;
    std::deque<animal::AnimalDecodingStub> types;
    bool reactive_cv_ = false;
    // Создаем внешние реакции в виде "животных"
    reactor::AnimalReactor env(pantomime, sound, types, reactive_cv_);
    // Создаем переводчик
    translator::AnimalTranslatinator translator(pantomime, sound, reactive_cv_);
    std::cout << "Начинаем проверку работоспособностии устройства" << std::endl;
    // Запускаем производство объектов с животными
    std::thread console_thread(&listenConsole);
    while (is_working) {
        if (commandQueue.empty())
            continue;
        Command command = commandQueue.front();
        commandQueue.pop_front();
        switch (command) {
            case kTalk:
                env.startTalking();
                break;
            case kListen:
                translator.startListening(types);
                break;
            case kOn:
                translator.turnOn();
                break;
            case kOff:
                translator.turnOff();
                break;
            case kExit:
                is_working = false;
                break;
        }
    }
    console_thread.join();
    return 0;
}
