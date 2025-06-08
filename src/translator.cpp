#include "translator.h"

namespace translator {

animal::PreparedData Sensor::prepareBatchOfData() {
    // Ожидание пока получим минимальный набор данных для анализа
    animal::PackedData packed_data = primary_sensor.waitAndPackData();
    if (!packed_data.ready)
        return (animal::PreparedData){.ready = false};
    animal::PreparedData prepared_data;
    prepared_data.ready = true;
    // Обрабатываем аудио-данные
    prepared_data.sound = sound_formatter.devideCarrier(packed_data.noise);
    // Обрабатываем видео-данные
    prepared_data.pantomime = video_formatter.splitAndClassify(packed_data.video, packed_data.distance);
    return prepared_data;
}

animal::PackedData Sensor::PrimarySensor::waitAndPackData() {
    if (!cv_)
        return (animal::PackedData){.ready = false};
    std::cout << "Устройство ждет окончания беседы" << std::endl;
    // Вместо реального ожидания поступления минимального кол-ва данных ожидаем оповещения от класса Животного
    // Создаем упакованные первичные данные с датчиков
    animal::PackedData packed_data;
    packed_data.ready = true;
    packed_data.video = pantomime.front();
    pantomime.pop_front();
    packed_data.noise = sound.front();
    sound.pop_front();
    packed_data.distance = (double)std::rand() / RAND_MAX;
    // Передаем упакованные данные дальше
    return packed_data;
}

std::vector<pantomime::Pantomime> Sensor::VideoFormatter::splitAndClassify(pantomime::Video& video, double distance) {
    // Строим карту глубины по изображению
    DeepMap deep_map = buildDeepMap(video, distance);
    // По карте глубины выделяем визуальные признаки различных сущностей на видео
    return getVisualIndication(deep_map);
}

Sensor::VideoFormatter::DeepMap Sensor::VideoFormatter::buildDeepMap(pantomime::Video& video, double distance) {
    /// @todo Строим карту глубины
    /// @todo Делаем поправку на расстояние
    return (DeepMap){.pantomime = video.figures};
}

std::vector<pantomime::Pantomime> Sensor::VideoFormatter::getVisualIndication(
    Sensor::VideoFormatter::DeepMap& deep_map) {
    /// @todo Разделяем объекты на видео
    /// @todo Получаем первичную визуальную информацию об объектах
    return deep_map.pantomime;
}

std::vector<syllable::Sound> Sensor::SoundFormatter::devideCarrier(syllable::Noise& noise) {
    // Разделяем звук на несущие
    Carrier carrier = splitCarrier(noise);
    // Обрабатываем несущие выделяя частоты и громкость
    return buildCarrier(carrier);
}

Sensor::SoundFormatter::Carrier Sensor::SoundFormatter::splitCarrier(syllable::Noise& noise) {
    /// @todo Проводим частотный анализ звука
    /// @todo На основе анализа делим звук на части
    return (Carrier){.sound = noise.noises};
}

std::vector<syllable::Sound> Sensor::SoundFormatter::buildCarrier(Sensor::SoundFormatter::Carrier carrier) {
    /// @todo Определяем среднюю частоту несущей
    /// @todo определяем среднюю громкость звука
    return carrier.sound;
}

std::vector<animal::DecodedAnimalCharacteristic> Translator::translate(animal::PreparedData& prepared_data,
                                                                       std::vector<animal::AnimalType> types) {
    std::vector<animal::DecodedAnimalCharacteristic> decoded;
    std::cout << "Начинаем перевод..." << std::endl;
    // Проходимся по каждому существу в списке и составляем для него перевод
    for (size_t iter = 0; iter < prepared_data.pantomime.size(); iter++) {
        // Выделяем языковыве сигналы
        animal::DecodedAnimalCharacteristic animal =
            predictAnimalCharacteristic(prepared_data.pantomime, prepared_data.sound, types, iter);
        // Пытаемся понять, какое настроение и какие потребности у животного
        Need need = translateNeed(animal);
        // Подготовливаем сообщение перевода
        animal.message = predictMessage(animal, need);
        decoded.push_back(animal);
    }
    std::cout << "Перевод окончен" << std::endl;
    return decoded;
}

animal::DecodedAnimalCharacteristic Translator::predictAnimalCharacteristic(std::vector<pantomime::Pantomime>& video,
                                                                            std::vector<syllable::Sound>& sound,
                                                                            std::vector<animal::AnimalType>& types,
                                                                            size_t iter) {
    animal::DecodedAnimalCharacteristic animal;
    // Классификация животного
    animal.animal_type = predictAnimal(animal.animal.body, animal.animal.sound, types, iter);
    // Определяем пантомимимику
    animal.animal.body = predictPantomime(video, animal.animal_type, iter);
    // Соотносим звук с животным
    animal.animal.sound = predictSound(sound, animal.animal_type, iter);
    return animal;
}

pantomime::Pantomime Translator::predictPantomime(std::vector<pantomime::Pantomime>& video, animal::AnimalType type,
                                                  size_t iter) {
    /// @todo Заглушка. Вместо реального предсказания, просто забираем данные с видео
    return video[iter];
}

syllable::Sound Translator::predictSound(std::vector<syllable::Sound>& sound, animal::AnimalType type, size_t iter) {
    /// @todo Заглушка. Вместо реального предсказания, просто забираем данные с аудио
    return sound[iter];
}

animal::AnimalType Translator::predictAnimal(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                             std::vector<animal::AnimalType>& types, size_t iter) {
    /// @todo Заглушка. Вместо реальной классификации, просто забираем данные с потока
    return types[iter];
}

Translator::Need Translator::translateNeed(animal::DecodedAnimalCharacteristic& animal) {
    // Выделяем настроение
    Mood mood = predictMood(animal);
    // Выделяем потребность
    return predictNeed(animal, mood);
}

Translator::Mood Translator::predictMood(animal::DecodedAnimalCharacteristic& animal) {
    return (Mood){};
}

Translator::Need Translator::predictNeed(animal::DecodedAnimalCharacteristic& animal, Translator::Mood mood) {
    return (Need){};
}

Translator::MessageTemplate Translator::predictMessageTemplate(animal::DecodedAnimalCharacteristic& animal,
                                                               Translator::Need& need) {
    /// @todo Заглушка. Здесь шаблон должен выбираться на основании входных признаков, а не рандомно
    return (MessageTemplate)(std::rand() % kMaxMessageTemplate);
}

std::string Translator::translateMessage(Translator::MessageTemplate message_template) {
    // Выбор необходимой локали. Здесь подумать на кнопкой изменения языка.
    // Стоит ли делать для разных стран разные устройства или добавить кнопку замены языка?
    // В требованиях закрепленного решения нет и оно не срочное, откладываем на попозже.
    return message_locale[RU][message_template];
}

std::string Translator::predictMessage(animal::DecodedAnimalCharacteristic& animal, Need& need) {
    // Подготавливаем шаблон сообщения
    Translator::MessageTemplate message_template = predictMessageTemplate(animal, need);
    // Переводим шаблон на необходимый язык
    return translateMessage(message_template);
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

static double signedCorrection(double value) {
    return (std::rand() % 2) ? value : -value;
}

static double correction() {
    return 1 + signedCorrection((double)std::rand() / RAND_MAX / 10);
}

double AnimalTranslatinator::startListening(std::deque<animal::AnimalDecodingStub>& types) {
    double time_counter = 0;
    if (power_) {
        std::cout << "На устройстве нажата кнопка прослушивания" << std::endl;
        // Подготовка первичных данных
        animal::PreparedData prepared_data = sensor.prepareBatchOfData();

        double video_time_;
        if (hardware_video_) {
            video_time_ = (double)kHardwareVideoStep.second / kHardFreq / 1000000000;
        } else {
            video_time_ = (double)kHardwareVideoStep.first / kSoftFreq / 1000000000;
        }
        video_time_ *= correction();
        std::cout << "Обработка видео заняла " << video_time_ << " секунд" << std::endl;
        time_counter += video_time_;

        double audio_time_;
        if (hardware_audio_) {
            audio_time_ = (double)kHardwareAudioStep.second / kHardFreq / 1000000000;
        } else {
            audio_time_ = (double)kHardwareAudioStep.first / kSoftFreq / 1000000000;
        }
        audio_time_ *= correction();
        std::cout << "Обработка аудио заняла " << audio_time_ << " секунд" << std::endl;
        time_counter += audio_time_;

        if (!prepared_data.ready)
            return 0;
        // Перевод сообщения
        auto messages = translator.translate(prepared_data, types.front().types);

        double classify_time_ =
            kHardwareClassifyStep[hardware_classify] / kHardwareClassifyFreq[hardware_classify] / 1000000000;
        classify_time_ *= correction();
        std::cout << "Классификация животного заняла " << classify_time_ << " секунд" << std::endl;
        time_counter += classify_time_;

        double decoding_time_;
        if (hardware_decoding_) {
            decoding_time_ = (double)kHardwareDecodingStep.second / kHardFreq / 1000000000;
        } else {
            decoding_time_ = (double)kHardwareDecodingStep.first / kSoftFreq / 1000000000;
        }
        decoding_time_ *= correction();
        std::cout << "Определение настроения животного заняло " << decoding_time_ << " секунд" << std::endl;
        time_counter += decoding_time_;

        types.pop_front();
        // Вывод пеервода на экран
        monitor.display(messages);
    } else {
        std::cout << "Кажется, устройство выключено" << std::endl;
    }
    return time_counter;
}

}  // namespace translator
