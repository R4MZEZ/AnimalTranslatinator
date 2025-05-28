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
    std::thread reactor_thread(&reactor::AnimalReactor::startTalking, &env);
    translator.turnOn();
    while (env.is_talking) {
        translator.startListening(pantomime, sound, types);
    }
    translator.turnOff();
    std::cout << "on join" << std::endl;
    reactor_thread.join();
    std::cout << "out join" << std::endl;
    return 0;
}
