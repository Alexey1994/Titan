Заметил странную активность на данном проекте, появились звёздочки.
Если кого-то интересует тема компиляторов, то предлагаю посмотреть <a href="https://github.com/Alexey1994/slang">сюда</a> (код на С) или <a href="https://github.com/Alexey1994/Browser-inerpreter">сюда<a> (код на JS, ещё снял <a href="https://www.youtube.com/watch?v=BzJ-ReZGp0s&t=21s">видео написания кода</a>). Но эти проекты я тоже не собираюсь поддерживать, занимаюсь написанием среды разработки визуального программирования(похоже на Blueprint в CryEngine, но писать можно вообще любые программы).<br>
Для интересующихся операционными системами на x86 есть <a href="https://github.com/Alexey1994/Rabbit">этот проект</a>.<br>
100% будет дополняться и исправляться <a href="https://github.com/Alexey1994/C-libraries">этот проект</a>.<br>
Прошу не смотреть сюда - проект мёртв. Он оказался слишком сырым поскольку я оценивал интерес людей к данной теме.<br>
Если у кого-то появились вопросы, предложения, пожелания, пишите на alexeyborisenko20@gmail.com<br>

Весь процесс включает три этапа:

1)Разбор программы с последующей трансляцией в байткод<br>
2)Разбор байткода в дерево, подлежащее исполнению<br>
3)Интерпретация, компиляция либо трансляция полученного дерева<br>

На первом этапе функция String *lexer(char *name) производит трансляцию файла с именем name в байткод<br>
Для этого файла определены следующие правила:<br>

Существуют 3 типа переменных:<br>
Z-целое число, объявляется следующим образом:<br>
Z number=90<br>
<br>
Элемент произвольной длины:<br>
Element e=new 12 - e имеет размер 12 байт<br>
<br>
Массив указателей на всё:<br>
Z x=0, y=1<br>
Pointers p={x, y} - берёт переменный из внешней области видимости и ссылается на них 0-м и 1-м индексами<br>
<br>
2 типа констант:<br>
строка, число<br>
Константное число можно объявить так:<br>
Const var=1<br>
Строка объявляется при инициализации элемента:<br>
Element t="string"<br>
<br>
Также есть операторы if и while:

    Z x=0, i=1
    if x
      print x
    end
    
    while i
      i=i-1
      print i
    end

Вызов функций в доработке:

    function f(Z a)
      print a
      
      function g(Z a)
        print a
      end
      
      function h()
        print a
      end
    end
    f()

На втором этапе функция Tree *parse(String *s) производит разбор байткода, составленного из следующих конструкций:

    PTRS_INIT - объявляет массив указателей
    ARRAY_INIT - устарела
    ELEMENT_INIT - объявляет элемент
    INT_INIT - объявляет целое число
    CONST_INIT - объявляет констаннтое число
    CONST_STRING_INIT - объявляет констаннтую строку
    FUNCTION  - объявляет функцию
    STREAM 0x07 - на будущее
    
    LOOP - бесконечный цикл
    IF - условие
    
    ASSIGNMENT - присваивание одной переменной другой
    
    INC - добавить единицу к переменной
    DEC - вычесть единицу
    
    MUL - умножить
    DIV - разделить
    MOD - не реализована, подразумевается остаток от деления
    ADD - сложить
    SUB - вычесть
    
    SHR - побитовый сдвиг вправо
    SHL - побитовый сдвиг влево
    
    AND - побитовое и
    OR - побитовое или
    NOT - отрицание
    XOR - сумма по модулю 2
    
    CALL - вызов функции
    CALL_PTR_FUNCTION - не реализована
    
    END - конец блока
    
    BREAK - прервать цикл
    CONTINUE - перейти в начало цикла
    
    ALLOC - выделить память на куче
    
    EQ ==
    NEQ !=
    GT >
    LT <
    GE >=
    LE <=
    
    PUTC - распечатать что душе угодно

И наконец на третьем этапе происходить выполнение дерева, полученного при парсинге. Это дерево имеет следующую структуру:

      function_root
        
        function1
          var1
          var2
          ...
          
          arg1
          arg2
          ...
          
          body
          
          function11
            ...
          function12
            ...
          ...
        
        function2
          var1
          var2
          ...
          
          arg1
          arg2
          ...
      
          body
          
          function21
            ...
          function22
            ...
          ...
