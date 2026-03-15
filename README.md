# 📦 Лабораторная работа 2 — Sequences

> Реализация обобщённых структур данных и операций над ними на C++17 с использованием шаблонов.

---

## 👤 Автор Копанев Кирилл Артемович. Б25-507

## 🗂 Структуры данных

| Структура | Описание |
|---|---|
| `DynamicArray<T>` | Динамический массив с изменяемым размером |
| `LinkedList<T>` | Связный список |
| `MutableArraySequence<T>` | Изменяемая последовательность на массиве |
| `ImmutableArraySequence<T>` | Неизменяемая последовательность на массиве |
| `MutableListSequence<T>` | Изменяемая последовательность на списке |
| `ImmutableListSequence<T>` | Неизменяемая последовательность на списке |
| `AdaptiveSequence<T>` | Адаптивная последовательность с переключением представления |
| `BitSequence` | Битовая последовательность с побитовыми операциями |

---

## ⚙️ Возможности

### Базовые операции
- `Append`, `Prepend`, `InsertAt`
- `Get`, `GetFirst`, `GetLast`
- `GetSubsequence`
- `Concat`
- `Builder`
- `IEnumerator`

### Функции высшего порядка
- `Map`
- `Where`
- `Reduce`
- `Zip` / `Unzip`

### Алгоритмы
- `GetMinMaxAvg`
- `GetMedian`
- `CountInversions`
- `GetPrefixes`
- `GetSuffixes`
- `GetMovingAverage`
- `GetMirrorSum`

---

## 🧪 Тестирование

Для проекта подготовлены отдельные тесты в файле `tests.cpp`.

При успешном запуске выводится сообщение вида:

```text
Тест номер 1 пройден
Тест номер 2 пройден
...
Успешно! Всего тестов: N
```

---

## 🚀 Сборка и запуск

### Запуск основной программы

```cmd
mingw32-make
.\Lab-2.exe
```

### Запуск тестов

```cmd
mingw32-make tests
.\tests.exe
```

---

## 🛠 Требования

- `g++` с поддержкой C++17
- `mingw32-make`
- Windows PowerShell / Command Prompt

---

## 📁 Структура проекта

```text
Lab-2/
├── main.cpp
├── tests.cpp
├── Makefile
├── Dynamic_Array.h
├── Linked_List.h
├── ArraySequence.h
├── ListSequence.h
├── AdaptiveSequence.h
├── BitSequence.h
├── MapReduce.h
├── Pair.h
├── Sequence.h
└── ienumerator.h
```

---

## ❗ Обработка ошибок

В проекте используются стандартные исключения:
- `std::out_of_range` — выход за границы
- `std::invalid_argument` — некорректные аргументы

---



