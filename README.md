# Arguments parser library

Простейшая библиотека на C++ для обработки аргументов командной строки.

---

## ⭐ Особенности

- Поддерживает позиционные и именованные аргументы
- Поддерживает типы `int`, `string`, `bool`
- Поддерживает хранение мульти значений под одним аргументом
- Поддерживает значения по умолчанию
- Поддерживает парсинг напрямую в нужные переменные
- Присутствует автоматическая генерация сообщения-помощи для текущего парсера
- Отсутствие шаблонов (templates-free)

---

## 📚 API Reference

### Регистрация аргументов

```cpp
ArgParser::AddIntArgument(char short_name, const std::string& full_name, std::string description="");  // Добавление целочисленного аргумента
```

```cpp
ArgParser::AddStringArgument(char short_name, const std::string& full_name, std::string description="");  // Добавление строкового аргумента
```

```cpp
ArgParser::AddFlag(char short_name, const std::string& full_name, std::string description="");  // Добавление аргумента-флага
```

```cpp
ArgParser::AddHelp(char short_name, const std::string& full_name, std::string description="");  // Добавление аргумента - помощь
```

### Параметры аргументов (на примере Int)

```cpp
.StoreValue(int& value_link);  // Ссылка на переменную, куда класть спаршенное значение
```

```cpp
.StoreValues(std::vector<int>& vector_value_link);  // Ссылка на вектор, куда записывать множество значений
```

```cpp
.Default(int value);  // Значение по умолчанию
```

```cpp
.MultiValue(size_t minimal_count=0);  // Сколько минимум должно быть значений для аргумента
```

```cpp
.Positional();  // Сделать аргумент позиционным
```

### Получение значений из парсера

```cpp
ArgParser::HelpDescription() const;  // Получение сгенерированного сообщения - помощь
```

```cpp
ArgParser::GetIntValue(const std::string& full_name, int index=0);  // Получение значения целочисленного аргумента
```

```cpp
ArgParser::GetStringValue(const std::string& full_name, int index=0);  // Получение значения строкового аргумента
```

```cpp
ArgParser::GetFlag(const std::string& full_name, int index=0);  // Получение значения аргумента-флага
```

---

## 🚀 Быстрый старт

Ниже будет приведен пример использования библиотеки.

### Задача

Найти сумму или произведение заданных чисел.

### Возможная реализация

```cpp
#include <functional>

#include <iostream>
#include <numeric>

#include "lib/ArgParser.h"

struct Options {
    bool sum = false;
    bool mult = false;
};

int main(int argc, char** argv) {
    Options opt;
    std::vector<int> values;

    ArgumentParser::ArgParser parser("Program");
    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
    parser.AddFlag("sum", "add args").StoreValue(opt.sum);
    parser.AddFlag("mult", "multiply args").StoreValue(opt.mult);
    parser.AddHelp('h', "help", "Program accumulate arguments");

    if(!parser.Parse(argc, argv)) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if(parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    if(opt.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if(opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        std::cout << parser.HelpDescription();
        return 1;
    }

    return 0;

}
```
