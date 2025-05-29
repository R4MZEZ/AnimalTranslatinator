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
     * @param[in] reactive_cv Триггер на поступление данных.
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
         * @param[in] reactive_cv Триггер на поступление данных.
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
     * Описывает получение информации о животных на видео.
     */
    class VideoFormatter {
    public:
        /*!
         * @brief Создание модуля обработки видео.
         */
        VideoFormatter() {}

        /*!
         * @brief Передача полученных видео-данных на обработку.
         * Обработчик строит карту глубины и маскирует объекты для разделения животных и получение их эмоций.
         */
        std::vector<pantomime::Pantomime> splitAndClassify(pantomime::Video& video);

    private:
        /// @brief Построение карты глубины
        void buildDeepMap() {};
        /// @brief Маскирование отдельных сущностей на видео
        void splitObjects() {};
        /// @brief Разделение объектов на живые и не живые
        void classifyObjects() {};
    };

    /*!
     * @brief Подсистема обработки звука
     * Описывает получение информации о звуках животных.
     */
    class SoundFormatter {
    public:
        /*!
         * @brief Создание модуля обработки звука
         */
        SoundFormatter() {}

        /*!
         * @brief Передача полученных аудио-данных на обработку.
         * Обработчик делит данные на несущие и по частоте и громкости относит звукии соответствующим животным.
         * @todo Не реализована передача аудио. Данные поступает в предзаготовленном виде.
         */
        std::vector<syllable::Sound> splitAndClassify(syllable::Noise& noise);

    private:
        /// @brief Разделение частотных составляющих звука
        void splitFrequences() {};
        /// @brief Разделение амплитудных составляющих звука
        void splitVolume() {};
    };

    /// @brief Первичные датчики
    PrimarySensor primary_sensor;
    /// @brief Обработчик видео
    VideoFormatter video_formatter;
    /// @brief Обработчик звука
    SoundFormatter sound_formatter;
};

/*!
 * @brief Класс переводчика
 * Описывает перевод с животного человеческий на основании обрааботанных данных
 */
class Translator {
public:
    /*!
     * @brief Создание модуля переводчика
     */
    Translator() {}

    /*!
     * @brief Перевод с животного на человеческий
     * @param[in] video Предобработанное видео
     * @param[in] sound Предобработанный звук
     * @param[in] types Заглушка в виде типов животных
     */
    std::vector<animal::DecodedAnimalCharacteristic> translate(std::vector<pantomime::Pantomime> video,
                                                               std::vector<syllable::Sound> sound,
                                                               std::vector<animal::AnimalType> types);

private:
    /*!
     * @brief Определение пантомимики животного
     * @param[in] video Предобработанное видео
     * @param[in] iter Порядковый номер животного
     * @todo
     */
    pantomime::Pantomime predictPantomime(std::vector<pantomime::Pantomime>& video, size_t iter);

    /*!
     * @brief Соотнесение звука животного
     * @param[in] sound Предобработанное аудио
     * @param[in] iter Порядковый номер животного
     * @todo
     */
    syllable::Sound predictSound(std::vector<syllable::Sound>& sound, size_t iter);

    /*!
     * @brief Классификация животного
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] types Очередь типов животных для заглушки
     * @param[in] iter Порядковый номер животного
     * @todo
     */
    animal::AnimalType predictAnimal(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                     std::vector<animal::AnimalType>& types, size_t iter);

    /*!
     * @brief Составление подходящей фразы для перевода
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] type Вид животного
     * @todo
     */
    std::string predictMessage(pantomime::Pantomime& pantomime, syllable::Sound& sound, animal::AnimalType type);
};

/*!
 * @brief Класс монитора
 * Предназначен для имитации вывода на экран
 */
class Monitor {
public:
    /*!
     * @brief Создание модуля монитора
     */
    Monitor();

    /*!
     * @brief Вывод полученной информации на экран
     * @param[in] Массив переведнных сообщений с определенными признаками животных
     */
    void display(std::vector<animal::DecodedAnimalCharacteristic> animals);

private:
    std::map<animal::AnimalType, std::string> animal_names;
};

/*!
 * @brief Устройство переводчика
 */
class AnimalTranslatinator {
public:
    /*!
     * @brief Создание модели переводчика
     * @param[in] reactive_cv Триггер на поступление данных.
     */
    AnimalTranslatinator(std::condition_variable& reactive_cv_) : sensor(reactive_cv_) {}

    /*!
     * @brief Нажатие на кнопку включения
     */
    void turnOn();

    /*!
     * @brief Нажатие на кнопку выключения
     */
    void turnOff();

    /*!
     * @brief Нажатие на кнопку прослушивания
     * @param[in] pantomime Входной поток видео
     * @param[in] sound Входной поток аудио
     * @param[in] types Входной поток типов животных (для заглушки декодирования)
     */
    void startListening(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                        std::deque<animal::AnimalDecodingStub>& types);

private:
    /// @brief Обработчик внешних сигналов
    Sensor sensor;
    /// @brief Переводчик сообщения
    Translator translator;
    /// @brief Монитор для вывода на экран
    Monitor monitor;
    bool power_ = false;
};

}  // namespace translator
