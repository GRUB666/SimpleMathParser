# EN-RU

# EN:
# Simple Math Parser

Version 1.4.3
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

Disadvantage (but with reservation):
Many times slower than top mathematical parsers for C ++. Functionality and extensibility strongly affect the time factor.
I will give an example:
Such an expression (Maxwell’s distribution of speeds) the parser considers for unimaginable 1869 microseconds if you count the total time and 181 one microsecond if you only consider the calculation time:
4p (M / (2pRT)) ^ (3/2) * v ^ 2 * e ^ (- Mv ^ 2 / (2RT))
Therefore, if speed is important to you, and not the ability to dynamically expand the parser, then it is better to look in the direction of other options (there are those that consider the same expressions in less than 400 nanoseconds).
However, there is a caveat: complexity, and with it, runtime increases at approximately the same speed O (n) = sqrt (x), which means you can not be afraid for overly complex expressions, since they will not be able to jump over permissible limits.
conversion

Read all information on how to use the library in the Wiki section.



# RU
# Простой Математический Парсер (многофункциональный)

Версия 1.4.3 (Платиновая)
Особенности версии:
- Теперь вы можете добавлять свои собственные функции в отдельные экземпляры парсера
- Все константы и функции инкапсулированы в класс Oper. И измнения констант касаются лишь одного экземлпяра класса.
- Код был переведён на умные указатели (для обеспечения отсутствия утечек)

Простой в использовании (но мощный внутри) парсер  для обработки математических выражений (С++) 

*Достоинства:
- Очень неприхотлив к входной строке (любое количество пробелов, знаки умножения ставить необязательно)
- Достаточно быстрый (при многократном использовании показывает себя куда лучше других парсеров рекурсивного типа. Синтаксический анализ производится лишь один раз при установке нового выражения)
- Легко расширяем (вы без труда можете научить парсер обрабатывать любые ваши (либо стандартные) математические функции)
- Добавление собственных констант
- Поддержка выражений с неизвестным x

*Недостаток (но с оговоркой):
В разы медленнее топовых математических парсеров для C++. Функциональность и расширяемость сильно сказываются на факторе времени.
Приведу пример: 
Такое выражение (распределение Максвелла по скоростям) парсер считает за невообразимые 1869 микросекунд если считать полное время и 181 одну микросекунду, если считать лишь время вычислений: 
4p(M / (2pRT))^(3/2) * v^2 * e^(-Mv^2/(2RT))
Поэтому если вам важна именно скорость, а не возможность динмаически расширять парсер, то лучше посмотреть в сторону других вариантов (есть те, что считают такие же выражения менее чем за 400 наносекунд).
Однако есть оговорка: сложность, а вместе с ней и время выполнения увеличиваются примерно с такой скоростью O(n) = sqrt(x), значит, вы можете не бояться за слишком сложные выражения, так как они не смогут перескочить через допустимые лимиты.


Читайте всю информацию о том, как использовать библиотку в разделе Wiki либо же смотрите пример в main.cpp

