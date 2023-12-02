#pragma once
#include <string>
#include <vector>
#include <map>
#include "Logger.h"
#include "Errors.h"
/** @file
 * @author Егоров Е.А.
 * @version 1.0
 * @brief Заголовочный файл для модуля сетевого взаимодействия
 */
/** @brief Класс для сетевого взаимодействия
 * @details Сетевое взаимодействие происходит в методе connection
 * Вспомогательные методы для сетевого взаимодействия: md5 и generate_salt
 */
using namespace std;
class Client_Communicate
{
    public:
 /**
 * @brief Функция обработки соединения с клиентом
 * @param [in] port Порт для работы сервера
 * @param [in] database Массив map с базой данных, где ключ - логин
 * @param [in] l1 Ссылка на класс Logger для записи логов
 */
    int connection(int port,std::map<std::string,std::string> database,Logger* l1);
 /**
 * @brief Функция хеширования.Алгоритм md5 
 * @param [in] input_str Строка для хеширования
 * @return Зашифрованная строка
 */
    static std::string md5(std::string input_str);
 /**
 * @brief Функция генерации случайного числа SALT
 * @return Случайное число SALT
 */
    static std::string generate_salt();
};
