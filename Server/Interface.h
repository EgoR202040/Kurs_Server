#pragma once
/** @file
 * @author Егоров Е.А.
 * @version 1.0
 * @brief Заголовочный файл для модуля разбора ПКС
 */
#include "Logger.h"
#include <string>
/** @brief Класс для разбора параметров командной строки
 * @details Разбор ПКС происходит в методе comm_proc
 */
class Interface{
/** @brief Порт работы сервера
*/
    int PORT;
public:
    Interface(){}
/** @brief Функция для разбора ПКС
 * @param [in] argc Количество параметров в командной строке
 * @param [in] argv Массив с параметрами командной строки
 * @throw boost::program_options::error
 */
    int comm_proc(int argc, const char** argv);
};
