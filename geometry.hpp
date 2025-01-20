/*
MIT License

Copyright (c) 2025 Kostya-Vologin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <algorithm>

using std::numbers::pi;
using namespace std;

class ObjectSet;
class Point;
class LineObject;

// Базовый класс для всех геометрических объектов
class GeometricObject {
public:
   virtual ~GeometricObject() = default; // деструктор
   virtual void move(double dx, double dy) = 0; // перемещение объекта на заданные величины по осям X и Y
   virtual void rotate(double angle) = 0; // поворот относительно центра координат
   // поиск пересечений:
   virtual ObjectSet intersect(const GeometricObject &other) const = 0; 
   virtual ObjectSet intersect(const Point &other) const = 0;
   virtual ObjectSet intersect(const LineObject &other) const = 0;
   virtual ObjectSet intersect(const ObjectSet &other) const = 0;
   // поиск растояний между объектами:
   virtual double distanceTo(const GeometricObject &other) const = 0;
   virtual double distanceTo(const Point &other) const = 0;
   virtual double distanceTo(const LineObject &other) const = 0;
   virtual double distanceTo(const ObjectSet &other) const = 0;
};

// Класс для точки
class Point : public GeometricObject {
private:
   double x, y; // координаты X и Y точки
public:
   Point(double x = 0, double y = 0) : x(x), y(y) {} // конструктор, задающий координаты точки
   double getX() const { return x; } // метод для получения координаты X
   double getY() const { return y; } // метод для получения координаты Y
   void move(double dx, double dy) override; // перемещение точки на заданные величины по осям X и Y
   void rotate(double radians) override; // поворот точки относительно центра координат
   // поиск пересечения точки с другом объектом:
   virtual ObjectSet intersect(const GeometricObject &other) const;
   virtual ObjectSet intersect(const Point &other) const;
   virtual ObjectSet intersect(const LineObject &other) const;
   virtual ObjectSet intersect(const ObjectSet &other) const;
   // поиск растояния от точки до другого объекта:
   virtual double distanceTo(const GeometricObject &other) const;
   virtual double distanceTo(const Point &other) const;
   virtual double distanceTo(const LineObject &other) const;
   virtual double distanceTo(const ObjectSet &other) const;
};

// Класс для линейных объектов (прямая, отрезок, луч)
class LineObject : public GeometricObject {
private:
   Point start, end; // точки, с помощью которых задаются линейные объекты
public:
   // методы для получения точек линейных объектов:
   Point getStart() const { return start; }
   Point getEnd() const { return end; }
   // Конструкторы, позволяющие задать точки линейного объекта: 
   LineObject(double x1, double y1, double x2, double y2) : start(x1, y1), end(x2, y2) {}
   LineObject(const Point &start, const Point &end) : start(start), end(end) {}
   void move(double dx, double dy) override; // перемещение линейного объекта на заданные величины по осям X и Y
   void rotate(double radians) override; // поворот линейного объекта относительно центра координат
   // поиск пересечения линейного объекта с другом объектом:
   virtual ObjectSet intersect(const GeometricObject &other) const;
   virtual ObjectSet intersect(const Point &other) const;
   virtual ObjectSet intersect(const LineObject &other) const;
   virtual ObjectSet intersect(const ObjectSet &other) const;
   // поиск растояния от линейного объекта до другого объекта:
   virtual double distanceTo(const GeometricObject &other) const;
   virtual double distanceTo(const Point &other) const;
   virtual double distanceTo(const LineObject &other) const;
   virtual double distanceTo(const ObjectSet &other) const;

   virtual bool checkT(double t) const { return true; } // проверка, удовлетворяет ли параметр t типу линейного объекта (для прямой всегда true)
   bool onLine(const Point &other) const; // проверка, лежит ли точка на линии
   double where(const Point &other) const; // вычисление параметра t линейного объекта, для конкретной точки
   Point makePoint(double t) const; // создание точки на линейном объекте используя параметр t
};

// Класс для прямой
class Line : public LineObject {
public:
   // конструкторы, позволяющие задать прямую:
   Line(double x1, double y1, double x2, double y2) : LineObject(x1, y1, x2, y2) {}
   Line(const Point &start, const Point &end) : LineObject(start, end) {}
};

// Класс для отрезка
class Segment : public LineObject {
public:
   // конструкторы, позволяющие задать отрезок:
   Segment(double x1, double y1, double x2, double y2) : LineObject(x1, y1, x2, y2) {}
   Segment(const Point &start, const Point &end) : LineObject(start, end) {}
   bool checkT(double t) const override { return t >= 0 && t <= 1; } // проверка, удовлетворяет ли параметр t для отрезка
};

// Класс для луча
class Ray : public LineObject {
public:
   // конструкторы, позволяющие задать луч:
   Ray(double x1, double y1, double x2, double y2) : LineObject(x1, y1, x2, y2) {}
   Ray(const Point &start, const Point &end) : LineObject(start, end) {}
   bool checkT(double t) const override { return t >= 0; }// проверка, удовлетворяет ли параметр t для луча
};

// Класс для набора геометрических объектов
class ObjectSet : public GeometricObject {
private:
   vector <shared_ptr<GeometricObject>> objects; // набор объектов (точки, прямые, отрезки, лучи)
public:
   void move(double dx, double dy) override; // перемещение всех объектов из набора на заданные величины по осям X и Y
   void rotate(double radians) override; // поворот всех объектов из набора относительно центра координат
   // поиск всех пересечений набора объектов с другом объектом:
   virtual ObjectSet intersect(const GeometricObject &other) const;
   virtual ObjectSet intersect(const Point &other) const;
   virtual ObjectSet intersect(const LineObject &other) const;
   virtual ObjectSet intersect(const ObjectSet &other) const;
   // поиск растояния от набора объектов до другого объекта:
   virtual double distanceTo(const GeometricObject &other) const;
   virtual double distanceTo(const Point &other) const;
   virtual double distanceTo(const LineObject &other) const;
   virtual double distanceTo(const ObjectSet &other) const;

   void add(shared_ptr<GeometricObject> other) { objects.push_back(other); }; // добавления объекта в набор
   bool isempty() const { return objects.empty(); } // проверка на пустоту набора
   shared_ptr<GeometricObject> getObject(size_t index) const; // получение объекта из набора по индексу
};

#endif // GEOMETRY_HPP