# geometry-library-cpp
Class library for analytical geometry on a plane.

The library supports working with the following concepts: point, line, segment, ray, set of objects (points, segments, etc.) for broken lines and intersection results.

    main.cpp - testing of library functions.

    To build the library, you need a compiler that supports at least C++20.

    Installation and configuration are not required. To work, you need to add the following files: geometry.hpp and geometry.cpp in a project that requires calculations in the field of analytical geometry on a plane.

## Library functions:

* moving an object &mdash; shifting coordinates by specified values along the X and Y axes;
* object rotation &mdash; displacement by a certain angle relative to the coordinate center;
* intersection search (a set of objects is returned);
* calculation of the distance between objects (minimum of distances up to 2 points of the object).

## User's Guide:
### Creating objects:
To create points, use the constructor with coordinates. Examples:
```cpp
Point p1(0, 0); // точка в начале координат
Point p2(3, 4); // точка с координатами (3, 4)
```
A straight line is created based on two points. You can pass either coordinates of points or objects of the Point class to the constructor. Examples:
```cpp
Line line(Point(0, 0), Point(5, 5)); // прямая через (0, 0) и (5, 5)
Line line(0, 0, 5, 5);
```
A Segment is defined in the same way as a straight line. Examples:
```cpp
Segment segment(Point(2, 0), Point(0, 2)); // отрезок от (2, 0) до (0, 2)
Segment segment(2, 0, 0, 2);
```
A ray is also created based on two points, but it is important here in which order the coordinates of the points or the Point objects are passed to the constructor. The first point is the beginning of the ray. Examples:
```cpp
Ray ray(Point(6, -1), Point(6, 2)); // луч с началом в (6, -1), направленный к (6, 2)
Ray ray(6, -1, 6, 2);
```
A set of geometric objects (ObjectSet) is created empty, objects are added using the add method. You must use make_shared to create an object and manage its memory using a smart pointer (shared_ptr). Examples:
```cpp
ObjectSet set;
set.add(make_shared<Point>(p1)); // добавление точки
set.add(make_shared<Line>(line)); // добавление прямой
```
### Operations for moving and finding the distance between objects:
To move an object, use the move method. Examples:
```cpp
p1.move(1, 1); // перемещение точки на заданные величины по осям X и Y
segment.move(3, 4); // перемещение точек segment на заданные величины
set.move(2, 2); // перемещение всех объектов из набора на заданные величины по осям X и Y
```
To rotate an object relative to the origin, use the rotate method with an angle in radians. Example:
```cpp
p1.rotate(pi / 2); // поворот точки на 90 градусов
```
To calculate the distance to another object, use the distanceTo method. Examples:
```cpp
double distance = p1.distanceTo(p2); // расстояние между двумя точками
double distance2 = set.distanceTo(line2); // расстояние между набором объектов и прямой
```

### Finding intersections of objects:
The intersect method returns an ObjectSet object containing the result of the intersection. If there is no intersection, an empty set is returned. To check whether an ObjectSet contains elements (points, segments, lines, or rays), the isempty() method is used, which returns true if the set is empty, and false otherwise. An object is retrieved from a set using the GetObject method, which returns a smart pointer (shared_ptr) to the object at the specified index. If the specified index is out of bounds, an out_of_range exception is thrown with the message "Index is out of bounds". Example of intersection search:
```cpp
try {
   Line line(Point(0, 0), Point(5, 5));
   Point p1(-1, 1);
   Segment segment(Point(2, 0), Point(0, 2));

   // Ищем пересечение линии и точки
   ObjectSet intersection = line.intersect(p1);
   if (intersection.isempty()) {
      cout << "Пересечения нет" << endl;
   } else {
      auto &intersectionPoint = dynamic_cast<Point &>(*intersection.getObject(0));
      cout << "Пересечение линии и точки: (" << intersectionPoint.getX() << ", " << intersectionPoint.getY() << ")" << endl;
   }

   // Создаём набор объектов
   ObjectSet set;
   set.add(make_shared<Point>(p1));
   set.add(make_shared<Line>(line));
   set.add(make_shared<Segment>(segment));
   Ray ray(6, -1, 6, 2);
   // Ищем пересечение набора объектов и луча
   ObjectSet setIntersection = set.intersect(ray);
   if (setIntersection.isempty()) {
      cout << "Пересечения нет" << endl;
   } else {
      auto &intersectionPoint = dynamic_cast<Point &>(*setIntersection.getObject(0));
      cout << "Пересечение набора объектов и луча: (" << intersectionPoint.getX() << ", " << intersectionPoint.getY() << ")" << endl;
   }

   // Попытка доступа к объекту с некорректным индексом
   auto &intersectionPoint = dynamic_cast<Point &>(*setIntersection.getObject(10)); // Это вызовет исключение
} catch (const out_of_range &e) {
   cerr << "Ошибка: " << e.what() << endl;
}
```

## Class diagram:
![diagram](https://github.com/Kostya-Vologin/geometry-library-cpp/blob/main/pictures/diagram.png)

## Implementation features:
This code implements double dispatching (for the `intersect` and `distanceTo` methods), which is achieved solely through the use of virtual functions. The basic idea is to implement it as two single calls, that is, as two separate virtual function calls: the first defines the dynamic type of the first object, and the second defines the second object.

First, the `intersect` (`distanceTo`) virtual function is called with the `GeometricObject&` parameter. The code of this function in this case is, for example, in the `Point` class:

```cpp
ObjectSet Point::intersect(const GeometricObject &other) const {
    return other.intersect(*this);
}
```

It turns out that another `intersect` (`distanceTo`) is called inside this function. But `other` becomes an object that calls a member function of the class, and the `*this` object becomes a parameter of the function. As you know, compilers determine which of a set of functions should be called based on the static type of arguments passed to the function. In this case, four different `intersect` functions can be called, but one of them is selected based on the static type `*this`. Since in the example we are inside a member function of the `Point` class, the `*this` object must be of type `Point`. Therefore, the `intersect` (`distanceTo`) function is called, which takes the `Point&` parameter, and not `GeometricObject&`.

All `intersect` (`distanceTo`) functions are virtual, therefore, when calling the `Point: : intersect` function, the function call corresponding to the real type of the `other` object is substituted. Inside this implementation of the `intersect` function, the true type of both objects is known, since the object on the left is `*this` (and therefore has the type of the class in which this function is implemented), and the object on the right has the type declared as the type of the parameter.
