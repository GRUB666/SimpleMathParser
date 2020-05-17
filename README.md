# RU
# Простой Математический Парсер (многофункциональный)

Версия 1.4 (Платиновая)
Особенности версии:
- Теперь вы можете добавлять свои собственные функции в отдельные экземпляры парсера
- Все константы и функции инкапсулированы в класс Oper. И измнения констант касаются лишь одного экземлпяра класса.
- Код был переведён на умные указатели (для обеспечения отсутствуия утечек)

Простой в использовании (но мощный внутри) парсер  для обработки математических выражений (С++) 
Достоинства:
- Очень неприхотлив к входной строке (любое количество пробелов, знаки умножения ставить необязательно)
- Достаточно быстрый (при многократном использовании показывает себя куда лучше других парсеров рекурсивного типа. Синтаксический анализ производится лишь один раз при установке нового выражения)
- Легко расширяем (вы без труда можете научить парсер обрабатывать любые ваши (либо стандартные) математические функции)
- Добавление собственных констант
- Поддержка выражений с неизвестным x

Читайте всю информацию о том, как использовать библиотку в разделе Wiki либо же смотрите пример в main.cpp


# EN:
# Simple Math Parser

Version 1.3
Features:
- Added complex functions (like trigonometry). In total, 15 functions are supported by default.
- Now you can add your own functions to the parser very simply and without delving into my code

RU:
Easy-to-use parser for processing mathematical expressions (C ++)
Advantages:
- Very unpretentious to the input line (any number of spaces, multiplication signs are optional)
- Fast enough (with repeated use, it shows itself much better than other parsers of a recursive type. Parsing is performed only once when installing a new expression)
- Easy to expand (you can easily teach the parser to process any of your (or standard) mathematical functions)
- Adding your own constants
- Support for expressions with unknown x

Read all information on how to use the library in the Wiki section.
