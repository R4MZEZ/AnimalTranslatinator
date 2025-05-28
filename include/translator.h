#pragma once

#include "animal_types.h"
#include <condition_variable>
#include <deque>

namespace translator {

class Sensor {
public:
    Sensor(std::condition_variable& reactive_cv_) : primary_sensor(reactive_cv_) {}

    void waitAndPackData() { primary_sensor.waitAndPackData(); }

    std::vector<pantomime::Pantomime> splitAndClassifyVideo(pantomime::Video& video) {
        return video_formatter.splitAndClassify(video);
    }

    std::vector<syllable::Sound> splitAndClassifySound(syllable::Noise& noise) {
        return sound_formatter.splitAndClassify(noise);
    }

private:
    class PrimarySensor {
    public:
        PrimarySensor(std::condition_variable& reactive_cv_) : cv_(reactive_cv_) {}

        void waitAndPackData() {
            // Stub that waits for notify from reactor
            std::cout << "blocked" << std::endl;
            std::mutex mu_;
            std::unique_lock lock(mu_);
            cv_.wait(lock);
            std::cout << "unblocked" << std::endl;
        }

    private:
        std::condition_variable& cv_;
    };

    class VideoFormatter {
    public:
        VideoFormatter() {}

        std::vector<pantomime::Pantomime> splitAndClassify(pantomime::Video& video) {
            // In real device we need to build DeepMap. In would helps to mask animals on video and get their real sizes
            buildDeepMap();
            // Split objects on video to different entities and track their movements
            splitObjects();
            // Classify pantomimes of this entities
            classifyObjects();
            return video.figures;
        }

    private:
        void buildDeepMap() {};
        void splitObjects() {};
        void classifyObjects() {};
    };

    class SoundFormatter {
    public:
        SoundFormatter() {}

        std::vector<syllable::Sound> splitAndClassify(syllable::Noise& noise) {
            // In real device we need to build DeepMap. In would helps to mask animals on video and get their real sizes
            splitFrequences();
            // Split objects on video to different entities and track their movements
            splitVolume();
            return noise.noises;
        }

    private:
        void splitFrequences() {};
        void splitVolume() {};
    };

    PrimarySensor primary_sensor;
    VideoFormatter video_formatter;
    SoundFormatter sound_formatter;
};

class Translator {
public:
    Translator() {}

    std::vector<animal::DecodedAnimalCharacteristic> translate(std::vector<pantomime::Pantomime> video,
                                                               std::vector<syllable::Sound> sound,
                                                               std::vector<animal::AnimalType> types) {
        std::vector<animal::DecodedAnimalCharacteristic> decoded;
        for (size_t iter = 0; iter < video.size(); iter++) {
            animal::DecodedAnimalCharacteristic animal;
            animal.animal.body  = predictPantomime(video, iter);
            animal.animal.sound = predictSound(sound, iter);
            animal.animal_type  = predictAnimal(animal.animal.body, animal.animal.sound, types, iter);
            animal.message      = predictMessaage(animal.animal.body, animal.animal.sound, animal.animal_type);
        }
        return decoded;
    }

private:
    pantomime::Pantomime predictPantomime(std::vector<pantomime::Pantomime>& video, size_t iter) { return video[iter]; }

    syllable::Sound predictSound(std::vector<syllable::Sound>& sound, size_t iter) { return sound[iter]; }

    animal::AnimalType predictAnimal(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                     std::vector<animal::AnimalType>& types, size_t iter) {
        return types[iter];
    }

    std::string predictMessaage(pantomime::Pantomime& pantomime, syllable::Sound& sound, animal::AnimalType type) {
        const std::vector<std::string> messages{"Хочу гуляш", "Lorem Ipsum", "Бойся меня, кожаный мешок",
                                                "Давай играть"};
        return messages[std::rand() % messages.size()];
    }

    std::vector<animal::DecodedAnimalCharacteristic> prepareOutput(std::vector<pantomime::Pantomime> video,
                                                                   std::vector<syllable::Sound> sound,
                                                                   std::vector<animal::AnimalType> types) {}
};

class Monitor {
public:
    Monitor() {}

    void display(std::vector<animal::DecodedAnimalCharacteristic>) {}

private:
    std::map<animal::AnimalType, std::string> animal_names;
};

class AnimalTranslatinator {
public:
    AnimalTranslatinator(std::condition_variable& reactive_cv_) : sensor(reactive_cv_) {}

    void turnOn() { power_ = true; }

    void turnOff() { power_ = false; }

    void startListening(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                        std::deque<animal::AnimalDecodingStub>& types) {
        if (power_) {
            // In real device alghoritm should split all input data to logical batches
            // Instead of it we are using deques
            std::cout << "waits until data" << std::endl;
            sensor.waitAndPackData();
            std::cout << "got data" << std::endl;
            if (pantomime.empty()) {
                return;
            }
            std::vector<pantomime::Pantomime> video_property = sensor.splitAndClassifyVideo(pantomime.front());
            pantomime.pop_front();
            std::vector<syllable::Sound> sound_propeperty = sensor.splitAndClassifySound(sound.front());
            sound.pop_front();
            auto messages = translator.translate(video_property, sound_propeperty, types.front().types);
            types.pop_front();
            monitor.display(messages);
            std::cout << "displayed" << std::endl;
        }
    }

private:
    Sensor sensor;
    Translator translator;
    Monitor monitor;
    bool power_ = false;
};

}  // namespace translator
