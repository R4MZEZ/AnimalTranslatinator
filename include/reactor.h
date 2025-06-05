/*!
 * @file
 * @brief Механизм генерации данных для активации датчиков
 * @author Степанов Михаил, Казаченко Роман
 * @version 1.0
 */
#pragma once

#include "animal_types.h"
#include "condition_variable"
#include "mutex"
#include <cmath>
#include <deque>
#include <iostream>

namespace reactor {

/*!
 * @brief Класс для генерации данных
 * Описывает окружение с "говорящими животными", каждое из которых обладает собственными физическими признаками.
 */
class AnimalReactor {
public:
    /*!
     * @brief Создание генератора данных
     * @param[out] pantomime Поток генерируемого видео
     * @param[out] sound Поток генерируемого звука
     * @param[out] types Поток генерируемых типов животных
     * @param[out] reactive_cv Триггер на генерируемые данные.
     */
    AnimalReactor(std::deque<pantomime::Video>& pantomime, std::deque<syllable::Noise>& sound,
                  std::deque<animal::AnimalDecodingStub>& types, bool& reactive_cv);

    ~AnimalReactor() {
        std::cout << "Зоопарк закрывается..." << std::endl;
        is_talking  = false;
        reactive_cv = false;
    }

    /*!
     * @brief Запуск механизма генерации данных
     * Механизм генерирует случайные данные с задержкой в заданном диапазоне,
     * диапазон задержки задается через статические параметры класса.
     */
    void startTalking();

    /// @brief Переменная опроса. Имитирует состояние окружения - есть ли рядом животные.
    volatile bool is_talking;

private:
    std::mutex mu_;
    std::condition_variable cv_;
    std::deque<syllable::Noise>& sound;
    bool& reactive_cv;
    std::deque<pantomime::Video>& pantomime;
    std::deque<animal::AnimalDecodingStub>& types;
    std::map<animal::AnimalType, std::string> animal_names;

    static constexpr double kMaxSecond     = 2;     ///< Максимальное значение диапазона задержки
    static constexpr double kMinSecond     = 1;     ///< Минимальное значение диапазона задержки
    static constexpr size_t kSecondScaller = 1000;  ///< Делитель значение (1000 для деления мс и получения с)
    static constexpr size_t kMaxAnimal     = 3;     ///< Максимальное количество животных в одном считыванини данных
    static constexpr size_t kIterCount     = 5;     ///< Количество итераций по генерации данных
};

}  // namespace reactor
