#pragma once
#include <vector>
#include <iostream>
/** @file
 * @author Егоров Е.А.
 * @version 1.0
 * @brief Заголовочный файл для модуля вычислений
 */
/** @brief Класс для операции умножение вектора
 * @details Вектор указывается в параметрах конструктора.
 * Для получения результат вычислений используется метод send_res.
 */
class Calculator
{
double results;
public:
/**
 * @brief Вычисление
 * @param [in] input_data Вектор данных. Не должен быть пустой.
 * Тип данных double
 * Исключения не возбуждаются
 * @warning При переполнении будет возвращено inf или -inf
 */
    Calculator(std::vector<double>input_data);
/**
 * @brief Получение результата вычислений
 */
   	double send_res();
};
