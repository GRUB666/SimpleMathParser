# SimpleMathParser

1.3
Особенности:
- Добавлены сложные функции (вроде тригонометрии). Всего по умолчанию поддерживается 15 функций
- Теперь можно очень просто и не вникая в мой код добавить собственные функции в парсер

RU:
Простой в использовании парсер  для обработки математических выражений (С++) 
Достоинства:
- Очень неприхотлив к входной строке (любое количество пробелов, знаки умножения ставить необязательно)
- Достаточно быстрый (при многократном использовании показывает себя куда лучше других парсеров рекурсивного типа. Синтаксический анализ производится лишь один раз при установке нового выражения)
- Легко расширяем (вы без труда можете научить парсер обрабатывать любые ваши (либо стандартные) математические функции)
- Добавление собственных констант
- Поддержка выражений с неизвестным x

Как использовать:
Необходимо подключить заголовочный файл, внутри которого будет пространство имён smp (Simple Math Parser)

Перед началом использования рекомендуется вызввать глобальную функцию InitializeConstants(map<char, double> *consts = nullptr, bool addConstants = true).*

Если не передавать в него никакие аргументы, то он добавит константы по умолчанию (Пи и е)
Первый параметр словарь map со всеми константами, которые вы хотите добавить от себя. Константа может быть лишь одной буквой, никаких сочетаний!
Второй параметр отвечает за добавление вышеупомянутых констант по умолчанию.
|Любые не определённые в этом методе символы будут восприниматься парсером как константы равные 1|!

Так же вы можете глобально изменить символ, указывающий на перменную с помощью setNewXAlias(char symb). По умолчанию неизвестным во всех выражениях считается 'x'

|Фцнкции InitializeConstants и setNewXAlias - глобальные, т.е. повлияют на обработку всех выражений в программе, в будущем это будет инкапсулировано в класс Oper|

Для непосредственного внешнего использования предназначен класс Expression, который принимает в конструкторе (или в методе setExpression) строку с необходимым выражением. Никаких строгих условий для записи нет, знаки умножения рядом с буквами ставятся по желанию, пробелы допускаются в любом количестве. Операция передачи выражения в объект относительно медленная, так как непосредственно после неё происходит анализ строки с последующим рекурсивным созданием объектов. Поэтому использовать надо в не самых критичных местах программы.

Для получения результата необходимо вызвать метод getResult(float x = 0), который принимает на вход значение переменной х (при этом она необязательно должна быть в выражении. Тогда метод вызывается без аргументов).  За счёт рекурсивного спуска и последующего подъёма по иерархии операций, getResult() работает медленней чем заранее записанные выражения, в среднем, для расчёта такой функции уходит 1500 наносекунд: 10x + x^2 - 3/2.

Архитектура:
Скажу честно: сам код парсинга очень нагромождён и требует доработок (хотя и не спаггети-код), особенно методы updateSubOpers() в классах Expression и Multiplication_Oper.
Иерархия же простая: 



Представленная архитектура устарела и указана для версии 1.1! Некоторые методы были переименованы и добавлены


1.Базовый абстрактный класс Oper (Operation). Он содержит в себе vector других под операций (указателей на них)(sub_opers), сигнатуры абстрактных методов: 
    virtual double getValue(double x = 0) = 0;
		virtual void setExpression(std::string str) = 0;
		virtual void updateSubOpers() = 0;
    
И реализации методов prepareString() и CutWhiteSpaces() (Из названия можно догадаться, что делает последний). 
prepareString() помимо вызова CutWhiteSpaces() добавляет перед буквами и между скобками знаки умножения (если их нет) для корректной работы парсера.

IActionsMap - Маленький интерфейс, содержащий в себе вектор char. Он позволяет классам Expression и Multiplication_Oper знать, какое действие нужно выполнить с каждой отдельной подоперацией

2. Expression : Oper, IActionsMap. Основной класс, о котором говорилось выше. После вызова setExpression() он, как и почти все остальные классы из списка, анализирует полученную строку. Выражение разбивается на отдельные множители, каждый из множителей образует новый объект Multiplication_Oper, его адрес добавляется в sub_opers. После чего при вызове метода getValue() у основного экземпляра во внешней программе Expression вызывает методы getValue() у всех членов sub_opers, после чего либо складывает либо вычитает отдельные значения в соотвествии с ActionsMap. Полученное число и есть результат работы

3. Multiplication_Oper : Oper, IActionsMap. Самый большой по коду класс. Его суть в том, что из полученной от Expression строки он должен выделить отдельные множители (как это делал Expression со слагаемыми), сложность в том, что множителем может быть как простое число, так и новое выражение Expression, а может даже быть и степенью. Необходимо точно определить, где начинается, а где заканчивается тот или иной множитель и, в зависимости от его типа, добавить в sub_opers элемент нужного типа.
(Я в курсе, что можно было для компактности кода сделать опеределние множителей рекурсивным по одному, а не все сразу. Но это как по мне не очень производительно, так как может сделать иерархию очень большой и трудно отлаживаемой)

4. Number_Oper : Oper. Самая базовая операция (котороая и не операция вовсе). Не имеет sub_opers. Содержит в себе лишь строку value.
При вызове метода getValue() либо преобразует value в число и отдаёт его, либо, если value - символ, вызывает глобальный метод getNumberFromLetter(char symb, double x_value), который возращает значение, соотвествующее конкретной букве. Если эта буква x (или любое другое наименование переменной), то возвращается переданный метод x_value (т.е. заместо неизвестного ставится число переданное пользователем изначально в getValue())

Есть ещё класс для возведения в степень, но он не имеет концептуальных отличий от уже описанных классов.

Вот, собственно, и всё описание. 





1.3
Features:
- Added complex functions (like trigonometry). Found 15 functions by default
- Add function code to parser

RU:
Easy to use parser for processing mathematical expressions (C ++)
Advantages:
- Any number of spaces, multiplication signs are optional)
- Fast enough (when used repeatedly, it shows that it is better than other parsers of a recursive type.
- Easily expand math functions
- Adding your own constants
- Support for expressions with unknown x

How to use:
You must include the header file, inside which will be the namespace smp (Simple Math Parser)

InitializeConstants (map <char, double> * consts = nullptr, bool addConstants = true). *

If he does not pass any arguments to him, then he will add default constants (Pi and e)
The first parameter is a map dictionary with all the constants that you want to add from yourself. A constant can be just one letter, no combinations!
Default.
Any characters not defined in this method will be perceived by the parser as constants equal to 1 |!

SetNewXAlias ​​(char symb). By default, unknown in all expressions is considered 'x'

| The functions InitializeConstants and setNewXAlias ​​are global, i.e. it will be encapsulated in the Oper class in the future |

An expression that takes in the constructor (or in the setExpression method) results in an expressed expression. There are no strict conditions for writing, multiplication signs next to letters are optional, spaces are allowed in any quantity. Expression transfer operations in objects are relatively slow. Programs.

To get the result, you must call the getResult method (with floating point x = 0), which must be expressed. GetResult () is slower than pre-written expressions, on average, it takes 1500 nanoseconds to calculate such functions: 10x + x ^ 2 - 3/2.

Architecture:
Honestly, the parsing code itself is very demanding and requires detailed codes (although not a spaghetti code), especially the updateSubOpers () methods in the Expression and Multiplication_Oper classes.
The hierarchy is simple:



The architecture presented is deprecated and is listed for version 1.1! Some methods have been renamed and added.


1.Base abstract class Oper (Operation). It contains a vector of other operations (pointers), signatures of abstract methods:
virtual double getValue (double x = 0) = 0;
virtual void setExpression (std :: string str) = 0;
virtual void updateSubOpers () = 0;

And implementations of the prepareString () and CutWhiteSpaces () methods (From the name you can guess what the latter does).
prepareString (), in addition to calling CutWhiteSpaces (), indicates that for the parser to work correctly, you need to specify a letter between the brackets.

IActionsMap - A small interface containing a vector symbol. It allows the Expression and Multiplication_Oper classes to know what action to perform with each individual suboperation.

2. Expression: Oper, IActionsMap. The main class mentioned above. After calling setExpression (), it analyzes the resulting version. The expression is divided into separate factors, each of which receives a new Multiplication_Oper object, its address is added to sub_opers. The expression uses the getValue () methods of all members of sub_opers, after which it either adds or subtracts individual values ​​in relation to the ActionMap. The resulting number is the result of work

3. Multiplication_Oper: Oper, IActionsMap. The largest class in code. This is an expression that must be obtained from the expression, and it must be both a prime and a new expression. The expression can be a power. Add an element of the required type to sub_opers.
(I am aware that it was possible for compactness
