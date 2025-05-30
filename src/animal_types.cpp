#include "animal_types.h"

namespace animal {

std::map<AnimalType, std::pair<long, long>> animal_sizes{
    {AnimalType::Cat, {45, 60}},   {AnimalType::Dog, {22, 140}},   {AnimalType::Parrot, {15, 40}},
    {AnimalType::Cow, {150, 270}}, {AnimalType::Sheep, {60, 110}},
};

std::map<AnimalType, double> animal_frequency{
    {AnimalType::Cat, 20000}, {AnimalType::Dog, 1500},   {AnimalType::Parrot, 20000},
    {AnimalType::Cow, 35000}, {AnimalType::Sheep, 1000},
};

std::map<AnimalType, std::pair<int, int>> animal_volume{
    {AnimalType::Cat, {20, 85}},  {AnimalType::Dog, {60, 120}},   {AnimalType::Parrot, {40, 135}},
    {AnimalType::Cow, {70, 105}}, {AnimalType::Sheep, {70, 110}},
};

std::map<AnimalType, std::vector<syllable::ThroatSound>> animal_throat{
    {AnimalType::Cat, {syllable::ThroatSound::None, syllable::ThroatSound::Meow}},
    {AnimalType::Dog, {syllable::ThroatSound::None, syllable::ThroatSound::Woof}},
    {AnimalType::Parrot, {syllable::ThroatSound::None, syllable::ThroatSound::Tweeting}},
    {AnimalType::Cow, {syllable::ThroatSound::None, syllable::ThroatSound::Moo}},
    {AnimalType::Sheep, {syllable::ThroatSound::None, syllable::ThroatSound::Bleat}},
};

std::map<AnimalType, std::vector<syllable::LarynxSound>> animal_larynx{
    {AnimalType::Cat, {syllable::LarynxSound::Purring, syllable::LarynxSound::Hiss, syllable::LarynxSound::Growl}},
    {AnimalType::Dog, {syllable::LarynxSound::Growl}},
    {AnimalType::Parrot, {syllable::LarynxSound::Chirp}},
    {AnimalType::Cow, {syllable::LarynxSound::Mooing}},
    {AnimalType::Sheep, {syllable::LarynxSound::Bleating}},
};

namespace random {

static constexpr double kMaxSoundDuration = 3.0;

static pantomime::Pantomime generatePantomime(AnimalType animal) {
    pantomime::Pantomime pantomime;
    pantomime.body     = static_cast<pantomime::BodyPosition>(std::rand() % pantomime::MaxBodyPosition);
    pantomime.facial   = static_cast<pantomime::FacialExpression>(std::rand() % pantomime::MaxFacialExpression);
    pantomime.gestures = static_cast<pantomime::Gesture>(std::rand() % pantomime::MaxGestures);
    std::pair<long, long> sizes = animal_sizes[animal];
    pantomime.size              = std::rand() % (sizes.second - sizes.first) + sizes.first;
    return pantomime;
}

static syllable::Sound generateSound(AnimalType animal) {
    syllable::Sound sound;
    sound.larinx                = animal_larynx[animal][std::rand() % animal_larynx[animal].size()];
    sound.throat                = animal_throat[animal][std::rand() % animal_throat[animal].size()];
    sound.duration              = ((double)std::rand()) / RAND_MAX * kMaxSoundDuration;
    sound.frequency             = ((double)std::rand()) / RAND_MAX * animal_frequency[animal];
    std::pair<int, int> volumes = animal_volume[animal];
    sound.volume                = std::rand() % (volumes.second - volumes.first) + volumes.first;
    return sound;
}

DecodedAnimalCharacteristic generateAnimal() {
    const AnimalType animal_type               = static_cast<AnimalType>(std::rand() % MaxAnimalType);
    DecodedAnimalCharacteristic decoded_animal = {.animal_type = animal_type};
    AnimalCharacteristic animal = {.body = generatePantomime(animal_type), .sound = generateSound(animal_type)};
    decoded_animal.animal       = animal;
    return decoded_animal;
}

}  // namespace random

}  // namespace animal
