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
    kHardVideo,
    kSoftVideo,
    kHardAudio,
    kSoftAudio,
    kGpuClassify,
    kNpuClassify,
    kCpuClassify,
    kHardDecoding,
    kSoftDecoding,
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
        else if (command == "hard video")
            commandQueue.push_back(kHardVideo);
        else if (command == "soft video")
            commandQueue.push_back(kSoftVideo);
        else if (command == "hard audio")
            commandQueue.push_back(kHardAudio);
        else if (command == "soft audio")
            commandQueue.push_back(kSoftAudio);
        else if (command == "gpu")
            commandQueue.push_back(kGpuClassify);
        else if (command == "npu")
            commandQueue.push_back(kNpuClassify);
        else if (command == "cpu")
            commandQueue.push_back(kCpuClassify);
        else if (command == "hard decoding")
            commandQueue.push_back(kHardDecoding);
        else if (command == "soft decoding")
            commandQueue.push_back(kSoftDecoding);
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
            case kListen: {
                double score = translator.startListening(types);
                std::cout << "Общая длительность обработки " << score << " секунд" << std::endl;
            } break;
            case kOn:
                translator.turnOn();
                break;
            case kOff:
                translator.turnOff();
                break;
            case kHardVideo:
                translator.setHardwareVideo(true);
                break;
            case kSoftVideo:
                translator.setHardwareVideo(false);
                break;
            case kHardAudio:
                translator.setHardwareAudio(true);
                break;
            case kSoftAudio:
                translator.setHardwareAudio(false);
                break;
            case kGpuClassify:
                translator.setHardwareClassify(0);
                break;
            case kNpuClassify:
                translator.setHardwareClassify(1);
                break;
            case kCpuClassify:
                translator.setHardwareClassify(2);
                break;
            case kHardDecoding:
                translator.setHardwareDecoding(true);
                break;
            case kSoftDecoding:
                translator.setHardwareDecoding(false);
                break;
            case kExit:
                is_working = false;
                break;
        }
    }
    console_thread.join();
    return 0;
}
