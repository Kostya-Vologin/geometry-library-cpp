#include <iostream>
#include "geometry.hpp"

using namespace std;

int main() {
   //Тестируем Point:
   Point p1(0, 0);
   Point p2(3, 4);
   cout << "Растояние между точками p1(0, 0) и p2(3, 4): " << p1.distanceTo(p2) << endl;
   p1.move(1, 1);
   cout << "p1 после перемещения: (" << p1.getX() << ", " << p1.getY() << ")" << endl;
   p1.rotate(pi/2);
   cout << "p1 после поворота на 90 градусов: (" << p1.getX() << ", " << p1.getY() << ")" << endl;

   Point p10(10, 10);
   Point p20(10, 10);
   Point p30(20, 20);
   ObjectSet result1 = p10.intersect(p20);
   auto &point15 = dynamic_cast<Point &>(*result1.getObject(0));
   cout << "Пересечение p10(10, 10) c p20(10, 10): (" << point15.getX() << ", " << point15.getY() << ")" << endl;
   ObjectSet result20 = p20.intersect(p30);
   if (result20.isempty()) cout << "p20(10, 10) и p30(20, 20): Пересечения нет" << endl;

   // Тестируем LineObject:
   Line line(Point(0, 0), Point(5, 5));
   cout << "Проверим p2 на onLine: " << (line.onLine(p2) ? "p2 лежит на прямой" : "p2 не лежит на прямой") << endl;
   Point p3(5, 5);
   cout << "Проверим p3 на onLine: " << (line.onLine(p3) ? "p3 лежит на прямой" : "p3 не лежит на прямой") << endl;
   ObjectSet intersection = line.intersect(p3);
   cout << "Пересечение line c точкой p3: " << (intersection.isempty() ? "пересечения нет" : "пересечение есть") << endl;

   Segment segment(Point(2, 0), Point(0, 2));
   ObjectSet intersection2 = line.intersect(segment);
   cout << "Пересечение line c segment: " << (intersection2.isempty() ? "пересечения нет" : "пересечение есть") << endl;
   shared_ptr<GeometricObject> obj = intersection2.getObject(0);
   auto &point = dynamic_cast<Point &>(*obj);
   cout << "Пересечение line c segment: (" << point.getX() << ", " << point.getY() << ")" << endl;

   Ray ray(Point(6, -1), Point(6, 2));
   ObjectSet intersection3 = line.intersect(ray);
   cout << "Пересечение line c ray: " << (intersection3.isempty() ? "пересечения нет" : "пересечение есть") << endl;
   auto result3 = dynamic_cast<Point &>(*intersection3.getObject(0));
   cout << "Пересечение line c ray: (" << result3.getX() << ", " << result3.getY() << ")" << endl;

   // Тестируем ObjectSet
   ObjectSet set;
   set.add(make_shared<Point>(p1));
   set.add(make_shared<Point>(p2));
   set.add(make_shared<Line>(line));
   set.add(make_shared<Segment>(segment));

   cout << "ObjectSet пустой: " << (set.isempty() ? "да" : "нет")<< endl;

   Point p4(3, 0);
   double distance = set.distanceTo(p4);
   cout << "Расстояние от точки p4(3, 0) до ObjectSet: " << distance << endl;
   Line line2(Point(2, -2), Point(4, 0));
   double distance2 = set.distanceTo(line2);
   cout << "Расстояние от line2(Point(2, -2), Point(4, 0)) до ObjectSet: " << distance2 << endl;

   Ray ray20(Point(2, 5), Point(2, 3));
   ObjectSet intersection10 = set.intersect(ray20);
   if (intersection10.isempty()) cout << "Как-то пусто" << endl;
   auto result110 = dynamic_cast<Point &>(*intersection10.getObject(0));
   auto result210 = dynamic_cast<Point &>(*intersection10.getObject(1));
   cout << "Первая точка пересечения: (" << result110.getX() << ", " << result110.getY() << ")" << endl;
   cout << "Вторая точка пересечения: (" << result210.getX() << ", " << result210.getY() << ")" << endl;

   Point ch(4, 4);
   ObjectSet inter = set.intersect(ch);
   if (inter.isempty()) cout << "Как-то пусто" << endl;
   auto re = dynamic_cast<Point &>(*inter.getObject(0));
   cout << "Точка пересечения ch(4, 4) и ObjectSet: (" << re.getX() << ", " << re.getY() << ")" << endl;

   ObjectSet set2;
   Line lineS(Point(0, 5), Point(3, 5));
   Ray rayS(Point(7, 5), Point(2, 0));
   set2.add(make_shared<Line>(lineS));
   set2.add(make_shared<Ray>(rayS));
   ObjectSet interse = set.intersect(set2);
   if (interse.isempty()) cout << "Как-то пусто" << endl;
   auto resul1 = dynamic_cast<Point &>(*interse.getObject(0));
   auto resul2 = dynamic_cast<Point &>(*interse.getObject(1));
   cout << "Первая точка set и set2 пересечения: (" << resul1.getX() << ", " << resul1.getY() << ")" << endl;
   cout << "Вторая точка set и set2 пересечения: (" << resul2.getX() << ", " << resul2.getY() << ")" << endl;

   set.move(2, 2);
   try {
      auto &p1move = dynamic_cast<Point &>(*set.getObject(0));
      cout << "p1 в set после перемещения: (" << p1move.getX() << ", " << p1move.getY() << ")" << endl;

      auto &linemove = dynamic_cast<Line &>(*set.getObject(2));
      cout << "line в set после перемещения: ("
           << "Start: (" << linemove.getStart().getX() << ", " << linemove.getStart().getY() << "), "
           << "End: (" << linemove.getEnd().getX() << ", " << linemove.getEnd().getY() << "))" << endl;
   } catch (const out_of_range &e) {
      cerr << e.what() << endl;
   }
   return 0;
}