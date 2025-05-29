#include "translator.h"

namespace translator {

void Sensor::waitAndPackData() {
    primary_sensor.waitAndPackData();
}

std::vector<pantomime::Pantomime> Sensor::splitAndClassifyVideo(pantomime::Video& video) {
    std::cout << "Производим обработку видео" << std::endl;
    return video_formatter.splitAndClassify(video);
}

std::vector<syllable::Sound> Sensor::splitAndClassifySound(syllable::Noise& noise) {
    std::cout << "Производим обработку звука" << std::endl;
    return sound_formatter.splitAndClassify(noise);
}

void Sensor::PrimarySensor::waitAndPackData() {
    // Stub that waits for notify from reactor
    std::mutex mu_;
    std::unique_lock lock(mu_);
    std::cout << "Устройство ждет окончания беседы" << std::endl;
    cv_.wait(lock);
}

std::vector<pantomime::Pantomime> Sensor::VideoFormatter::splitAndClassify(pantomime::Video& video) {
    // In real device we need to build DeepMap. In would helps to mask animals on video and get their real sizes
    buildDeepMap();
    // Split objects on video to different entities and track their movements
    splitObjects();
    // Classify pantomimes of this entities
    classifyObjects();
    return video.figures;
}

std::vector<syllable::Sound> Sensor::SoundFormatter::splitAndClassify(syllable::Noise& noise) {
    // In real device we need to build DeepMap. In would helps to mask animals on video and get their real sizes
    splitFrequences();
    // Split objects on video to different entities and track their movements
    splitVolume();
    return noise.noises;
}

std::vector<animal::DecodedAnimalCharacteristic> Translator::translate(std::vector<pantomime::Pantomime> video,
                                                                       std::vector<syllable::Sound> sound,
                                                                       std::vector<animal::AnimalType> types) {
    std::vector<animal::DecodedAnimalCharacteristic> decoded;
    std::cout << "Начинаем перевод..." << std::endl;
    for (size_t iter = 0; iter < video.size(); iter++) {
        animal::DecodedAnimalCharacteristic animal;
        animal.animal.body  = predictPantomime(video, iter);
        animal.animal.sound = predictSound(sound, iter);
        animal.animal_type  = predictAnimal(animal.animal.body, animal.animal.sound, types, iter);
        animal.message      = predictMessaage(animal.animal.body, animal.animal.sound, animal.animal_type);
        decoded.push_back(animal);
    }
    std::cout << "Перевод окончен" << std::endl;
    return decoded;
}

pantomime::Pantomime Translator::predictPantomime(std::vector<pantomime::Pantomime>& video, size_t iter) {
    return video[iter];
}

syllable::Sound Translator::predictSound(std::vector<syllable::Sound>& sound, size_t iter) {
    return sound[iter];
}

animal::AnimalType Translator::predictAnimal(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                             std::vector<animal::AnimalType>& types, size_t iter) {
    return types[iter];
}

std::string Translator::predictMessaage(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                        animal::AnimalType type) {
    const std::vector<std::string> messages{"Хочу гуляш", "Lorem Ipsum", "Бойся меня, кожаный мешок", "Давай играть"};
    return messages[std::rand() % messages.size()];
}

Monitor::Monitor() {
    animal_names[animal::Cat]    = "котика";
    animal_names[animal::Dog]    = "собачку";
    animal_names[animal::Parrot] = "попугайчика";
    animal_names[animal::Sheep]  = "овечку";
    animal_names[animal::Cow]    = "корову";
}

void Monitor::display(std::vector<animal::DecodedAnimalCharacteristic> animals) {
    std::cout << "Происходит вывод на экран:" << std::endl;
    std::cout << "------------------------------" << std::endl;
    std::cout << "На изображении найдено " << animals.size() << " животных" << std::endl << std::endl;
    for (auto& animal : animals) {
        std::cout << "Информация о животном:" << std::endl;
        std::cout << "Мы видим здесь " << animal_names[animal.animal_type] << std::endl;
        std::cout << "Кажется, животное вам хочет сказать следующее:" << std::endl;
        std::cout << animal.message << std::endl << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}

void AnimalTranslatinator::turnOn() {
    std::cout << "На устройстве нажата кнопка включения" << std::endl;
    power_ = true;
}

void AnimalTranslatinator::turnOff() {
    std::cout << "На устройстве нажата кнопка выключения" << std::endl;
    power_ = false;
}

void AnimalTranslatinator::startListening(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                                          std::deque<animal::AnimalDecodingStub>& types) {
    if (power_) {
        std::cout << "На устройстве нажата кнопка прослушивания" << std::endl;
        // In real device alghoritm should split all input data to logical batches
        // Instead of it we are using deques
        sensor.waitAndPackData();
        if (pantomime.empty()) {
            std::cout << "Кнопка прослушивания отпущена, кажется, здесь не разговаривают животные" << std::endl;
            return;
        }
        std::vector<pantomime::Pantomime> video_property = sensor.splitAndClassifyVideo(pantomime.front());
        pantomime.pop_front();
        std::vector<syllable::Sound> sound_propeperty = sensor.splitAndClassifySound(sound.front());
        sound.pop_front();
        auto messages = translator.translate(video_property, sound_propeperty, types.front().types);
        types.pop_front();
        monitor.display(messages);
    }
}

}  // namespace translator
