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
     * @param[in] pantomime Имитация видео-потока
     * @param[in] sound Имитация аудио-потока
     * @param[in] reactive_cv Триггер на поступление данных.
     */
    Sensor(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound, bool& reactive_cv_)
        : primary_sensor(pantomime, sound, reactive_cv_) {}

    /*!
     * @brief Ожидание поступление минимального набора данных и их обработки
     * Ожидание происходит до тех пор пока не будет получено минимально необходимое количество информации для обработки
     * @todo Условность данного метода заключается в том, что мы не генерируем настоящий видео и аудио потоки, а лишь
     * ожидаем тригера поступления данных в очередь обработки
     */
    animal::PreparedData prepareBatchOfData();

private:
    /*!
     * @brief Подсистема первичных датчиков видео и звука.
     * Описывает получение "сырой информации" и выявление достаточности полученной информации.
     */
    class PrimarySensor {
    public:
        /*!
         * @brief Создание модуля первичных датчиков.
         * @param[in] pantomime Имитация видео-потока
         * @param[in] sound Имитация аудио-потока
         * @param[in] reactive_cv Триггер на поступление данных.
         */
        PrimarySensor(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound, bool& reactive_cv_)
            : pantomime(pantomime),
              sound(sound),
              cv_(reactive_cv_) {}

        /*!
         * @brief Ожидание поступления атомарных данных (Первичное чтение).
         * Ожидание происходит до тех пор пока не будет получено минимально необходимое количество информации
         * @return Возвращаемые с датчиков данные. Разделены на аудио-данные и видео-данные. К видео-данным
         * дополнительно прилагается растояние до центрального пикселя на первом кадре
         */
        animal::PackedData waitAndPackData();

    private:
        std::deque<pantomime::Video>& pantomime;
        std::deque<syllable::Noise>& sound;
        bool& cv_;
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
         * @return Разделенные существа на видео
         */
        std::vector<pantomime::Pantomime> splitAndClassify(pantomime::Video& video, double distance);

    private:
        /// @brief Карта глубины с поправкой на расстояние
        struct DeepMap {
            // Пустая структура-заглушка для создание карты глубины
            std::vector<pantomime::Pantomime> pantomime;
        };

        /// @brief Построение карты глубины
        /// @todo Реальный алгоритм основан на использовании нейронных сетей и алгоритмов машинного обучения
        DeepMap buildDeepMap(pantomime::Video& video, double distance);

        /// @brief Выделение визуальных признаков
        /// @todo Реальный алгоритм по карте глубины разделяет объекты на изображении и получает их визуальные признаки
        std::vector<pantomime::Pantomime> getVisualIndication(DeepMap& deep_map);
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
         * @return Разделенные несущие на аудио
         * @todo Не реализована передача аудио. Данные поступает в предзаготовленном виде
         */
        std::vector<syllable::Sound> devideCarrier(syllable::Noise& noise);

    private:
        struct Carrier {
            // Пустая структура-заглушка для создание несущей
            std::vector<syllable::Sound> sound;
        };

        /// @brief Разделение шума на составные части по чаастотным несущим.
        /// @todo Реальный алгоритм основан на частотном анализе звука
        Carrier splitCarrier(syllable::Noise& noise);

        /// @brief Выделение звуковых признаков по несущим.
        /// @todo Реальный алгоритм основан на восстановлении звуковых признаков при помощи нейронных сетей
        std::vector<syllable::Sound> buildCarrier(Carrier);
    };

    /// @brief Первичные датчики (первичное чтение)
    PrimarySensor primary_sensor;
    /// @brief Обработчик видео-данных
    VideoFormatter video_formatter;
    /// @brief Обработчик аудио-данных
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
     * @return Набор языковых признаков и переведенных сообщений от животных
     */
    std::vector<animal::DecodedAnimalCharacteristic> translate(animal::PreparedData& prepared_data,
                                                               std::vector<animal::AnimalType> types);

private:
    /*!
     * @brief Подготовка первичных языковых сигналов
     * @param[in] video Предобработанное видео
     * @param[in] sound Предобработанное аудио
     * @param[in] types Очередь типов животных для заглушки
     * @param[in] iter Порядковый номер животного
     */
    animal::DecodedAnimalCharacteristic predictAnimalCharacteristic(std::vector<pantomime::Pantomime>& video,
                                                                    std::vector<syllable::Sound>& sound,
                                                                    std::vector<animal::AnimalType>& types,
                                                                    size_t iter);

    /*!
     * @brief Определение пантомимики животного
     * @param[in] video Предобработанное видео
     * @param[in] type Тип животного
     * @param[in] iter Порядковый номер животного
     */
    pantomime::Pantomime predictPantomime(std::vector<pantomime::Pantomime>& video, animal::AnimalType type,
                                          size_t iter);

    /*!
     * @brief Соотнесение звука животного
     * @param[in] sound Предобработанное аудио
     * @param[in] type Тип животного
     * @param[in] iter Порядковый номер животного
     */
    syllable::Sound predictSound(std::vector<syllable::Sound>& sound, animal::AnimalType type, size_t iter);

    /*!
     * @brief Классификация животного
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] types Очередь типов животных для заглушки
     * @param[in] iter Порядковый номер животного
     */
    animal::AnimalType predictAnimal(pantomime::Pantomime& pantomime, syllable::Sound& sound,
                                     std::vector<animal::AnimalType>& types, size_t iter);

    /*!
     * @brief Настроение животного
     */
    struct Mood {
        // Пустая структура-заглушка для настроения животного
    };

    /*!
     * @brief Выделение настроения по языковым сигналам
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] type Вид животного
     */
    Mood predictMood(animal::DecodedAnimalCharacteristic& animal);

    /*!
     * @brief Потребности животного
     */
    struct Need {
        // Пустая структура-заглушка для потребности животного
    };

    /*!
     * @brief Построение предсказания о потребности животного
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] type Вид животного
     * @param[in] mood Настроение животного
     */
    Need predictNeed(animal::DecodedAnimalCharacteristic& animal, Mood mood);

    /*!
     * @brief Перевод языка животных в потребности
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] type Вид животного
     */
    Need translateNeed(animal::DecodedAnimalCharacteristic& animal);

    enum MessageTemplate {
        kGoulash = 0,
        kLorem,
        kAfraid,
        kPlay,
        kMaxMessageTemplate,
    };

    enum Locale {
        RU,
        EN,
    };

    std::map<MessageTemplate, std::string> messages_ru{{kGoulash, "Хочу гуляш"},
                                                       {kLorem, "Lorem Ipsum"},
                                                       {kAfraid, "Бойся меня, кожаный мешок"},
                                                       {kPlay, "Давай играть"}};

    std::map<MessageTemplate, std::string> messages_en{{kGoulash, "I want goulash"},
                                                       {kLorem, "Lorem Ipsum"},
                                                       {kAfraid, "Afraid of me, leather bag"},
                                                       {kPlay, "Let's play"}};

    std::map<Locale, std::map<MessageTemplate, std::string>> message_locale{{RU, messages_ru}, {EN, messages_en}};

    /*!
     * @brief Выделение шаблона человеческой языковой конструкции
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] type Вид животного
     * @param[in] need Потребность животного
     */
    MessageTemplate predictMessageTemplate(animal::DecodedAnimalCharacteristic& animal, Need& need);

    /*!
     * @brief Подготовка сообщения переводчика
     * @param[in] message_template шаблон языковой конструкции
     */
    std::string translateMessage(MessageTemplate message_template);

    /*!
     * @brief Составление подходящей фразы на необходимом языке
     * @param[in] pantomime Пантомимика животного
     * @param[in] sound Соотнесенный звук животного
     * @param[in] type Вид животного
     */
    std::string predictMessage(animal::DecodedAnimalCharacteristic& animal, Need& need);
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
    AnimalTranslatinator(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                         bool& reactive_cv_)
        : sensor(pantomime, sound, reactive_cv_) {}

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
    void startListening(std::deque<animal::AnimalDecodingStub>& types);

private:
    /// @brief Обработчик внешних сигналов
    Sensor sensor;
    /// @brief Переводчик сообщения
    Translator translator;
    /// @brief Монитор для вывода полученной информации
    Monitor monitor;
    bool power_ = false;
};

}  // namespace translator
