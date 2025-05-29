#include "reactor.h"

namespace reactor {

AnimalReactor::AnimalReactor(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                             std::deque<animal::AnimalDecodingStub>& types, std::condition_variable& reactive_cv)
    : pantomime(pantomime),
      sound(sound),
      types(types),
      reactive_cv(reactive_cv),
      is_talking(true) {
    animal_names[animal::Cat]    = "Кот";
    animal_names[animal::Dog]    = "Пёс";
    animal_names[animal::Parrot] = "Попугай";
    animal_names[animal::Sheep]  = "Овца";
    animal_names[animal::Cow]    = "Корова";
}

void AnimalReactor::startTalking() {
    std::cout << "Зоопарк открывается!" << std::endl;
    std::srand(std::time({}));
    is_talking = true;
    for (size_t iter = 0; iter < kIterCount; iter++) {
        {
            std::unique_lock lock(mu_);
            double rand    = ((double)std::rand() / (RAND_MAX));
            size_t timeout = std::floor((rand * (kMaxSecond - kMinSecond) + kMinSecond) * kSecondScaller);
            std::cout << "Сейчас животные устали, они подождут " << timeout << " мс прежде чем говорить снова"
                      << std::endl;
            cv_.wait_for(lock, std::chrono::milliseconds(timeout));
        }
        std::cout << "Животные начинают активно разговаривать!" << std::endl;
        size_t animal_count = std::rand() % kMaxAnimal + 1;
        pantomime::Video video;
        syllable::Noise noise;
        animal::AnimalDecodingStub animal_type;
        std::cout << "На беседу пришли:" << std::endl;
        for (size_t iter = 0; iter < animal_count; iter++) {
            animal::DecodedAnimalCharacteristic animal = animal::random::generateAnimal();
            std::cout << "\t" << iter << ". " << animal_names[animal.animal_type] << std::endl;
            video.figures.push_back(animal.animal.body);
            noise.noises.push_back(animal.animal.sound);
            animal_type.types.push_back(animal.animal_type);
        }
        pantomime.push_back(video);
        sound.push_back(noise);
        types.push_back(animal_type);
        std::cout << "Беседа окончена, можно начинать переводить" << std::endl;
        reactive_cv.notify_all();
    }
    is_talking = false;
    std::cout << "Зоопарк закрывается..." << std::endl;
    reactive_cv.notify_all();
}

}  // namespace reactor
