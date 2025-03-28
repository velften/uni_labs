# uni_labs
Все лабораторные покрыты unit-тестами с помощью библиотеки gtest.

## Аналоги умных указателей из STL
В этом проекте реализованы аналоги умных указателей из STL: UnqPtr, WeakPtr и ShrdPtr. Эти классы позволяют управлять памятью и избегать утечек памяти.
WeakPtr и ShrdPtr реализованы максимально приближённо к STL т.е через владение структурой ControlBlock, что позволяет решать проблему циклических ссылок.

## Реализация LRUcache с использованием хэш-таблицы и двусвязного списка.

Создана хэш-таблица с методом решения коллизий open addressing (Quadratic probing) и DoubleLinkedList. Реализован простой GUI для визуализации работы кэша. В качестве примера для ускорения поиска данных был выбран JSON-файл с данными о персонах (имя, фамилия, возраст).
![image](https://github.com/user-attachments/assets/a944619d-494b-45df-ad24-6e928608614c)



 ## Реализация ориентиированного и неориентиированного графа с визуализацией

Созданы ориентиированный и неориентиированный взвешенные графы, базовые операции добавления и удаления ребер и вершин, алгоритмы BFS, DFS, Дейкстры.
Доступна отрисовка самого графа, выделение кратчайшего расстояния между вершинами, раскраска графа в минимальное кол-во цветов с помощью алгоритма Ершова.
Добавлена возможность создания своего графа, загрузки графа из JSON и сохранение в него же.
![graph](https://github.com/user-attachments/assets/00ab5a53-1845-4974-a8b4-3afa18be3729)
