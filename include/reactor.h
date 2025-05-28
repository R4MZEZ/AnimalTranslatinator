#pragma once

#include "animal_types.h"
#include "condition_variable"
#include "mutex"
#include <cmath>
#include <deque>
#include <iostream>

namespace reactor {

class AnimalReactor {
public:
    AnimalReactor(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                  std::deque<animal::AnimalDecodingStub>& types, std::condition_variable& reactive_cv)
        : pantomime(pantomime),
          sound(sound),
          reactive_cv(reactive_cv),
          is_talking(true) {}

    void startTalking() {
        std::cout << "start talking" << std::endl;
        is_talking = true;
        for (size_t iter = 0; iter < kIterCount; iter++) {
            {
                std::unique_lock lock(mu_);
                double rand    = ((double)std::rand() / (RAND_MAX));
                size_t timeout = std::floor((rand * (kMaxSecond - kMinSecond) + kMinSecond) * kSecondScaller);
                std::cout << "wait for " << timeout << std::endl;
                cv_.wait_for(lock, std::chrono::milliseconds(timeout));
            }
            size_t animal_count = std::rand() % kMaxAnimal + 1;
            pantomime::Video video;
            syllable::Noise noise;
            for (size_t iter = 0; iter < animal_count; iter++) {
                animal::DecodedAnimalCharacteristic animal = animal::random::generateAnimal();
                video.figures.push_back(animal.animal.body);
                noise.noises.push_back(animal.animal.sound);
            }
            pantomime.push_back(video);
            sound.push_back(noise);
            std::cout << "send data" << std::endl;
            reactive_cv.notify_all();
        }
        is_talking = false;
        std::cout << "going out" << std::endl;
        reactive_cv.notify_all();
    }

    volatile bool is_talking;

private:
    std::mutex mu_;
    std::condition_variable cv_;
    std::deque<pantomime::Video>& pantomime;
    std::deque<syllable::Noise>& sound;
    std::condition_variable& reactive_cv;

    static constexpr double kMaxSecond     = 0.2;
    static constexpr double kMinSecond     = 0.1;
    static constexpr size_t kSecondScaller = 1000;
    static constexpr size_t kMaxAnimal     = 3;
    static constexpr size_t kIterCount     = 5;
};

}  // namespace reactor
