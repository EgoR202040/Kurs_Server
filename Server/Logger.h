#pragma once
/** @file
 * @author Егоров Е.А.
 * @version 1.0
 * @brief Заголовочный файл для модуля записи логов
 */
#include <string>
/** @brief Класс для записи логов
 * @details Запись логов происходит в методе writelog.
 * Для установки пути записи логов используется метод set_path
 */
class Logger
{
/** @brief Вспомогательная функция для получения текущего времени
 * @param [in] s Строка-ключ,принимающая значения: now и date
 * @warning Данная функция используется в методе writelog
 * @return Строка с временем
 */
    static std::string getCurrentDateTime(std::string s);
/** @brief Атрибут класса,хранящий путь к файлу для записи логов
*/
    std::string path_to_logfile;
public:
/** @brief Функция для записи логов
 * @param [in] s Строка для записи в лог файл
 * @throw crit_err,если путь к файлу не существует
 */
    int writelog(std::string s);
/** @brief Функция для установки пути записи логов
 * @param [in] path_file Строка,хранящая путь к файлу для записи логов
 * @throw crit_err,если путь к файлу не существует
 */
    int set_path(std::string path_file);
    Logger(){path_to_logfile = " ";};
    Logger(std::string s){path_to_logfile = s;};
};
