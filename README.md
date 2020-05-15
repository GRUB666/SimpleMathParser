# SimpleMathParser

1.2

- Теперь буквы могут быть любого регистра
- Необязательно указывать какое-то выражение сразу в конструкторе (оно может быть установлено позже)

RU:
Простой парсер  для обработки мат выражений (С++) 
На данный момент поддерживает базовые математические операции (+ - * /) а так же возведение в произвольную степень.
Пока лишь тестовая версия библиотеки, планируется добавление тригонометрических функций а так же логарифма.

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





EN:

Simple parser for processing mat expressions (C ++)
At the moment, it supports basic mathematical operations (+ - * /) as well as raising to an arbitrary degree.
So far, only a test version of the library, it is planned to add trigonometric functions as well as a logarithm.

How to use:
You must include the header file, inside which will be the namespace smp (Simple Math Parser)

Before you start using it, it is recommended to call the global function InitializeConstants (map <char, double> * consts = nullptr, bool addConstants = true). *

If no arguments are passed to it, then it will add default constants (Pi and e)
The first parameter is a map dictionary with all the constants that you want to add from yourself. A constant can be just one letter, no combinations!
The second parameter is responsible for adding the aforementioned default constants.
| Any characters not defined in this method will be perceived by the parser as constants equal to 1 |!

You can also globally change a character pointing to a variable using setNewXAlias ​​(char symb). By default, unknown in all expressions is considered 'x'

| The functions InitializeConstants and setNewXAlias ​​are global, i.e. will affect the processing of all expressions in the program, in the future it will be encapsulated in the Oper class |

For direct external use, the Expression class is intended, which takes a string with the necessary expression in the constructor (or in the setExpression method). There are no strict conditions for writing, multiplication signs next to letters are optional, spaces are allowed in any quantity. The operation of transferring an expression to an object is relatively slow, since immediately after it the string is analyzed, followed by the recursive creation of the objects. Therefore, it is necessary to use it in not the most critical places of the program.

To get the result, you must call the getValue method (float x = 0), which takes the value of the variable x as an input (it does not have to be in the expression. Then the method is called without arguments). Due to the recursive descent and the subsequent rise in the hierarchy of operations, getValue () works slower than pre-written expressions, on average, it takes 1500 nanoseconds to calculate such a function: 10x + x ^ 2 - 3/2.

Architecture:
Frankly, the parsing code itself is very congested and needs to be improved (although not a spaghetti code), especially the updateSubOpers () methods in the Expression and Multiplication_Oper classes.
The hierarchy is simple:

1.Base abstract class Oper (Operation). It contains a vector of other sub-operations (pointers to them) (sub_opers), signatures of abstract methods:
    virtual double getValue (double x = 0) = 0;
virtual void setExpression (std :: string str) = 0;
virtual void updateSubOpers () = 0;
    
And implementations of the prepareString () and CutWhiteSpaces () methods (From the name you can guess what the latter does).
prepareString (), in addition to calling CutWhiteSpaces (), adds multiplication signs (if there are none) before the letters and between the brackets for the parser to work correctly.

IActionsMap - A small interface containing a char vector. It allows the Expression and Multiplication_Oper classes to know what action to perform with each individual suboperation.

2. Expression: Oper, IActionsMap. The main class mentioned above. After calling setExpression (), he, like almost all other classes from the list, analyzes the resulting string. The expression is divided into separate factors, each of the factors forms a new Multiplication_Oper object, its address is added to sub_opers. Then, when the getValue () method is called on the main instance in an external program, Expression calls the getValue () methods on all sub_opers members, after which it either adds or subtracts individual values ​​in accordance with the ActionsMap. The resulting number is the result of work

3. Multiplication_Oper: Oper, IActionsMap. The largest class in code. Its essence is that it must select separate factors from the line received from Expression (as Expression did with the terms), the difficulty is that the factor can be either a prime number or a new Expression expression, or maybe even a degree. It is necessary to determine exactly where this or that factor begins and where, depending on its type, add an element of the desired type to sub_opers.
(I am aware that, for compact code, it was possible to make certain multipliers recursive one at a time, and not all at once. But for me it is not very productive, as it can make the hierarchy very large and difficult to debug)

4. Number_Oper: Oper. The most basic operation (which is not an operation at all). Does not have sub_opers. It contains only the string value.
When the getValue () method is called, it either converts value to a number and returns it, or, if value is a character, calls the getNumberFromLetter global method (char symb, double x_value), which returns the value corresponding to a specific letter. If this letter is x (or any other variable name), then returns

The advantage of this library is its modularity. In other words, you yourself can write additional aspects of parsing: create a new class, inherit it from Oper and implement its abstract methods, i.e. implement exactly how it will work with the entered string. After that, you need to add the processing of finding your new function in the Multiplication_Oper class. For example, the sequence of characters is "sin" or "cos". After finding it, you need to describe the search for arguments for the function and finally passing all this to the constructor of your new class, followed by adding a pointer to sub_opers.



What should be done:
In addition to formal details, like adding trigonometry, you need to refine the structure so that you can encapsulate InitializeConstants and setNewXAlias ​​in Opers. Also, for a simplified implementation of everything that I described above, you need to modify the prepareString () method (so that it does not make "sin" "s * i * n"), and write an automatic handler for the presence of a sequence of characters (if the handler sees what goes something like "... log (...)", then it should take out further processing a separate function, where you can manually process specific functions.)
Well, of course, you need to restructure the code for greater readability, although I'm not sure how much this can be done.

