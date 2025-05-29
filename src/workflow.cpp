/*
 * @brief Основной цикл для запуска перевода и инициализации взаимодействия
 * @author Степанов Михаил, Казаченко Роман
 * @version 1.0
 */

#include "reactor.h"
#include "translator.h"
#include <iostream>
#include <thread>

int main() {
    std::deque<pantomime::Video> pantomime;
    std::deque<syllable::Noise> sound;
    std::deque<animal::AnimalDecodingStub> types;
    std::condition_variable reactive_cv_;
    reactor::AnimalReactor env(pantomime, sound, types, reactive_cv_);
    translator::AnimalTranslatinator translator(reactive_cv_);
    std::cout << "Начинаем проверку работоспособностии устройства" << std::endl;
    std::thread reactor_thread(&reactor::AnimalReactor::startTalking, &env);
    translator.turnOn();
    while (env.is_talking) {
        translator.startListening(pantomime, sound, types);
    }
    translator.turnOff();
    reactor_thread.join();
    return 0;
}
