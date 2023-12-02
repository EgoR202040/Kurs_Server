#pragma once
/** @file
 * @author Егоров Е.А.
 * @version 1.0
 * @brief Заголовочный файл для модуля ошибок
 */
#include <system_error>
#include <stdexcept>
#include <string>
/** @brief Класс критических ошибок
 * @details В конструкторе указывается строка с текстом ошибки
 */
class crit_err:public std::runtime_error
{
public:
 /**
 * @brief Конструктор ошибки
 * @param [in] s Текст ошибки
 */
	crit_err(const std::string& s):std::runtime_error(s){}
};
/** @brief Класс не критических ошибок
 * @details В конструкторе указывается строка с текстом ошибки
 */
class no_crit_err:public std::runtime_error
{
public:
 /**
 * @brief Конструктор ошибки
 * @param [in] s Текст ошибки
 */
	no_crit_err(const std::string s): std::runtime_error(s){}
};
