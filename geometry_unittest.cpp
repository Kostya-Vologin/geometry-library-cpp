/*
К сожаления, некоторые тесты не удалось запустить, так как "compile.flags=-fno-rtti" отключило умные указатели.
Но без этой записи в файле локальных настроек не работает  Google test.
Решение, чтобы работало всё, я не нашёл.
Если закомментировать все тесты, где нет dynamic_cast, то удастся "выполнить все тесты для модуля".
*/

#include <gtest/gtest.h>

#include <iostream>
#include "geometry.hpp"

using namespace std;

// Тесты для класса Point
TEST(PointTest, MovePoint) {
   Point p(0, 0);
   p.move(1, 1);
   EXPECT_DOUBLE_EQ(p.getX(), 1.0);
   EXPECT_DOUBLE_EQ(p.getY(), 1.0);
}

TEST(PointTest, RotatePoint) {
   Point p(1, 0);
   p.rotate(pi/2);
   EXPECT_NEAR(p.getX(), 0.0, 1e-9);
   EXPECT_NEAR(p.getY(), 1.0, 1e-9);
}

TEST(PointTest, IntersectPoint) {
   Point p1(10, 10);
   Point p2(10, 10);
   Point p3(20, 20);

   ObjectSet result = p1.intersect(p2);
   ASSERT_FALSE(result.isempty());
   auto &intersection = dynamic_cast<Point &>(*result.getObject(0));
   EXPECT_DOUBLE_EQ(intersection.getX(), 10);
   EXPECT_DOUBLE_EQ(intersection.getY(), 10);

   ObjectSet result2 = p2.intersect(p3);
   EXPECT_TRUE(result2.isempty());
}

TEST(PointTest, DistanceToPoint) {
   Point p1(0, 0);
   Point p2(3, 4);
   EXPECT_DOUBLE_EQ(p1.distanceTo(p2), 5.0);
}

// Тесты для класса LineObject
TEST(LineObjectTest, OnLine) {
   Line line(Point(0, 0), Point(5, 5));
   Point p1(3, 3);
   Point p2(3, 4);
   EXPECT_TRUE(line.onLine(p1));
   EXPECT_FALSE(line.onLine(p2));
   EXPECT_DOUBLE_EQ(p2.distanceTo(line), sqrt(2)/2);
}

TEST(LineObjectTest, IntersectLineObject) {
   Line line(Point(0, 0), Point(5, 5));
   Point p(5, 5);
   ObjectSet intersection = line.intersect(p);
   EXPECT_FALSE(intersection.isempty());
   auto result = dynamic_cast<Point &>(*intersection.getObject(0));
   EXPECT_DOUBLE_EQ(result.getX(), 5.0);
   EXPECT_DOUBLE_EQ(result.getY(), 5.0);

   Segment segment(Point(2, 0), Point(0, 2));
   ObjectSet intersection2 = line.intersect(segment);
   EXPECT_FALSE(intersection2.isempty());
   auto result2 = dynamic_cast<Point &>(*intersection2.getObject(0));
   EXPECT_DOUBLE_EQ(result2.getX(), 1.0);
   EXPECT_DOUBLE_EQ(result2.getY(), 1.0);

   Ray ray(Point(6, -1), Point(6, 2));
   ObjectSet intersection3 = line.intersect(ray);
   EXPECT_FALSE(intersection2.isempty());
   auto result3 = dynamic_cast<Point &>(*intersection3.getObject(0));
   EXPECT_DOUBLE_EQ(result3.getX(), 6.0);
   EXPECT_DOUBLE_EQ(result3.getY(), 6.0);
}

// Тесты для класса ObjectSet
TEST(ObjectSetTest, DistanceToObjectSet) {
   ObjectSet set;
   Line line(Point(0, 0), Point(5, 5));
   Segment segment(Point(2, 0), Point(0, 2));
   set.add(make_shared<Line>(line));
   set.add(make_shared<Segment>(segment));
   ASSERT_FALSE(set.isempty());

   Point p(3, 0);
   double distance = set.distanceTo(p);
   EXPECT_DOUBLE_EQ(distance, 1.0);

   Line line2(Point(2, -2), Point(4, 0));
   double distance2 = set.distanceTo(line2);
   EXPECT_DOUBLE_EQ(distance2, sqrt(2));
}

TEST(ObjectSetTest, IntersectObjectSet) {
   ObjectSet set;
   Line line(Point(0, 0), Point(5, 5));
   Segment segment(Point(2, 0), Point(0, 2));
   set.add(make_shared<Line>(line));
   set.add(make_shared<Segment>(segment));
   ASSERT_FALSE(set.isempty());

   Ray ray(Point(2, 5), Point(2, 3));
   ObjectSet intersection1 = set.intersect(ray);
   EXPECT_FALSE(intersection1.isempty());
   auto result1 = dynamic_cast<Point &>(*intersection1.getObject(0));
   auto result2 = dynamic_cast<Point &>(*intersection1.getObject(1));
   EXPECT_DOUBLE_EQ(result1.getX(), 2.0);
   EXPECT_DOUBLE_EQ(result1.getY(), 2.0);
   EXPECT_DOUBLE_EQ(result2.getX(), 2.0);
   EXPECT_DOUBLE_EQ(result2.getY(), 0.0);

   Point ch(4, 4);
   ObjectSet intersection2 = set.intersect(ch);
   EXPECT_FALSE(intersection2.isempty());
   auto result3 = dynamic_cast<Point &>(*intersection2.getObject(0));
   EXPECT_DOUBLE_EQ(result3.getX(), 4.0);
   EXPECT_DOUBLE_EQ(result3.getY(), 4.0);

   ObjectSet set2;
   Line lineS(Point(0, 5), Point(3, 5));
   Ray rayS(Point(7, 5), Point(2, 0));
   set2.add(make_shared<Line>(lineS));
   set2.add(make_shared<Ray>(rayS));

   ObjectSet intersection3 = set.intersect(set2);
   EXPECT_FALSE(intersection3.isempty());
   auto result4 = dynamic_cast<Point &>(*intersection3.getObject(0));
   auto result5 = dynamic_cast<Point &>(*intersection3.getObject(1));
   EXPECT_DOUBLE_EQ(result4.getX(), 5.0);
   EXPECT_DOUBLE_EQ(result4.getY(), 5.0);
   EXPECT_DOUBLE_EQ(result5.getX(), 2.0);
   EXPECT_DOUBLE_EQ(result5.getY(), 0.0);
}

TEST(ObjectSetTest, MoveObjects) {
   ObjectSet set;
   Point p1(0, 0);
   Line line(Point(0, 0), Point(5, 5));
   set.add(make_shared<Point>(p1));
   set.add(make_shared<Line>(line));
   set.move(2, 2);
   auto &pointmove = dynamic_cast<Point &>(*set.getObject(0));
   auto &linemove = dynamic_cast<Line &>(*set.getObject(1));
   EXPECT_DOUBLE_EQ(pointmove.getX(), 2.0);
   EXPECT_DOUBLE_EQ(pointmove.getY(), 2.0);

   EXPECT_DOUBLE_EQ(linemove.getStart().getX(), 2.0);
   EXPECT_DOUBLE_EQ(linemove.getStart().getY(), 2.0);
   EXPECT_DOUBLE_EQ(linemove.getEnd().getX(), 7.0);
   EXPECT_DOUBLE_EQ(linemove.getEnd().getY(), 7.0);
}
