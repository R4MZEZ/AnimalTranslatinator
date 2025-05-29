/*!
 * @file
 * @brief Описание основных физических сущностей и признаков окружения
 * @author Степанов Михаил, Казаченко Роман
 * @version 1.0
 */
#pragma once

#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <vector>

namespace pantomime {

enum FacialExpression {
    Happiness = 0,
    Sadness,
    Anger,
    Astonishment,
    MaxFacialExpression,
};

enum BodyPosition {
    Threat = 0,
    Calm,
    Alertness,
    Trust,
    MaxBodyPosition,
};

enum Gesture {
    Shaking = 0,
    Playful,
    Paws,
    Body,
    Earing,
    MaxGestures,
};

struct Pantomime {
    FacialExpression facial;
    BodyPosition body;
    Gesture gestures;
    long size;
};

struct Video {
    std::vector<Pantomime> figures;
};

}  // namespace pantomime

namespace syllable {

enum LarynxSound {
    Purring = 0,
    Hiss,
    Growl,
    Chirp,
    Mooing,
    Bleating,
    MaxLarynxSound,
};

enum ThroatSound {
    None = 0,
    Meow,
    Woof,
    Tweeting,
    Moo,
    Bleat,
    MaxThroatSound,
};

struct Sound {
    LarynxSound larinx;
    ThroatSound throat;
    double frequency;
    int volume;
    double duration;
};

struct Noise {
    std::vector<Sound> noises;
};

}  // namespace syllable

namespace animal {

enum AnimalType {
    Cat = 0,
    Dog,
    Parrot,
    Cow,
    Sheep,
    MaxAnimalType,
};

extern std::map<AnimalType, std::pair<long, long>> animal_sizes;

extern std::map<AnimalType, double> animal_frequency;

extern std::map<AnimalType, std::pair<int, int>> animal_volume;

extern std::map<AnimalType, std::vector<syllable::ThroatSound>> animal_throat;

extern std::map<AnimalType, std::vector<syllable::LarynxSound>> animal_larynx;

struct AnimalCharacteristic {
    pantomime::Pantomime body;
    syllable::Sound sound;
};

struct DecodedAnimalCharacteristic {
    AnimalType animal_type;
    std::string message;
    AnimalCharacteristic animal;
};

struct AnimalDecodingStub {
    std::vector<AnimalType> types;
};

namespace random {

DecodedAnimalCharacteristic generateAnimal();

}  // namespace random

}  // namespace animal
