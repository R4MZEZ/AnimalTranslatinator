/*!
 * @file
 * @brief Описание структуры и алгоритма устройства - транслятора
 * @author Степанов Михаил, Казаченко Роман
 * @version 1.0
 */
#pragma once

#include "animal_types.h"
#include <condition_variable>
#include <deque>
#include <iostream>

namespace translator {

/*!
 * @brief Класс для обработчика входных сигналов
 * Описывает получение информации из окружающего мира и подготовка данных для передачи на переводчик.
 */
class Sensor {
public:
    /*!
     * @brief Создание модуля обработки входных сигнаов
     * @param[out] reactive_cv Триггер на поступление данных.
     */
    Sensor(std::condition_variable& reactive_cv_) : primary_sensor(reactive_cv_) {}

    /*!
     * @brief Ожидание поступления атомарных данных.
     * Ожидание происходит до тех пор пока не будет получено минимально необходимое количество информации для обработки
     * @todo На данный момент реализовано программно через триггер. Должно реагировать на видео и несущие звука.
     */
    void waitAndPackData();

    /*!
     * @brief Передача полученных видео-данных на обработку.
     * Обработчик строит карту глубины и маскирует объекты для разделения животных на картинке и получение их эмоций.
     * @todo Не реализована передача видео. Данные поступает в предзаготовленном виде.
     */
    std::vector<pantomime::Pantomime> splitAndClassifyVideo(pantomime::Video& video);

    /*!
     * @brief Передача полученных аудио-данных на обработку.
     * Обработчик делит данные на несущие и по частоте и громкости относит звукии соответствующим животным.
     * @todo Не реализована передача аудио. Данные поступает в предзаготовленном виде.
     */
    std::vector<syllable::Sound> splitAndClassifySound(syllable::Noise& noise);

private:
    /*!
     * @brief Подсистема первичных датчиков видео и звука.
     * Описывает получение "сырой информации" и выявление достаточности полученной информации.
     */
    class PrimarySensor {
    public:
        /*!
         * @brief Создание модуля первичных датчиков.
         * @param[out] reactive_cv Триггер на поступление данных.
         */
        PrimarySensor(std::condition_variable& reactive_cv_) : cv_(reactive_cv_) {}

        /*!
         * @brief Ожидание поступления атомарных данных.
         * Ожидание происходит до тех пор пока не будет получено минимально необходимое количество информации
         */
        void waitAndPackData();

    private:
        std::condition_variable& cv_;
    };

    /*!
     * @brief Подсистема обработки видео.
     * Описывает получение "сырой информации" и выявление достаточности полученной информации.
     */
    class VideoFormatter {
    public:
        VideoFormatter() {}

        std::vector<pantomime::Pantomime> splitAndClassify(pantomime::Video& video);

    private:
        void buildDeepMap() {};
        void splitObjects() {};
        void classifyObjects() {};
    };

    class SoundFormatter {
    public:
        SoundFormatter() {}

        std::vector<syllable::Sound> splitAndClassify(syllable::Noise& noise);

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
                                                               std::vector<animal::AnimalType> types);

private:
    pantomime::Pantomime predictPantomime(std::vector<pantomime::Pantomime>& video, size_t iter);

    syllable::Sound predictSound(std::vector<syllable::Sound>& sound, size_t iter);

    animal::AnimalType predictAnimal(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                     std::vector<animal::AnimalType>& types, size_t iter);

    std::string predictMessaage(pantomime::Pantomime& pantomime, syllable::Sound& sound, animal::AnimalType type);
};

class Monitor {
public:
    Monitor();

    void display(std::vector<animal::DecodedAnimalCharacteristic> animals);

private:
    std::map<animal::AnimalType, std::string> animal_names;
};

class AnimalTranslatinator {
public:
    AnimalTranslatinator(std::condition_variable& reactive_cv_) : sensor(reactive_cv_) {}

    void turnOn();

    void turnOff();

    void startListening(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                        std::deque<animal::AnimalDecodingStub>& types);

private:
    Sensor sensor;
    Translator translator;
    Monitor monitor;
    bool power_ = false;
};

}  // namespace translator
