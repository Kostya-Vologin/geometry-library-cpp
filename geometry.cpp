#include "geometry.hpp"

// ------------------ Point ------------------
void Point::move(double dx, double dy) {
   x += dx;
   y += dy;
}

void Point::rotate(double radians) {
   double new_x = x * cos(radians) - y * sin(radians);
   double new_y = x * sin(radians) + y * cos(radians);
   x = new_x;
   y = new_y;
}

ObjectSet Point::intersect(const GeometricObject &other) const {
   return other.intersect(*this);
}
ObjectSet Point::intersect(const Point &other) const {
   ObjectSet result;
   if (x == other.getX() && y == other.getY()) {
      result.add(make_shared<Point>(*this));
   }
   return result;
}
ObjectSet Point::intersect(const LineObject &other) const {
   ObjectSet result;
   if (other.onLine(*this) && other.checkT(other.where(*this))) result.add(make_shared<Point>(*this));
   return result;
}
ObjectSet Point::intersect(const ObjectSet &other) const {
   return other.intersect(*this);
}
double Point::distanceTo(const GeometricObject &other) const {
   return other.distanceTo(*this);
}
double Point::distanceTo(const Point &other) const {
   double x2 = other.getX();
   double y2 = other.getY();
   return sqrt(pow(x2 - x, 2) + pow(y2 - y, 2));
}
double Point::distanceTo(const LineObject &other) const {
   Point a = other.getStart();
   Point b = other.getEnd();
   double A = b.getY() - a.getY(); //первый вектор
   double B = b.getX() - a.getX();
   double dx = x - a.getX(); //второй вектор
   double dy = y - a.getY();

   double t = (dx*B + dy*A) / (B*B + A*A);
   if (other.checkT(t)) {
      //Point projection(a.getX() + t * B, a.getY() + t * A);
      Point projection = other.makePoint(t);
      return projection.distanceTo(*this);
   }
   return min(a.distanceTo(Point(x, y)), b.distanceTo(Point(x, y)));
}
double Point::distanceTo(const ObjectSet &other) const {
   return other.distanceTo(*this);
}

// ------------------ LineObject ------------------
void LineObject::move(double dx, double dy) {
   start.move(dx, dy);
   end.move(dx, dy);
}

void LineObject::rotate(double radians) {
   start.rotate(radians);
   end.rotate(radians);
}

ObjectSet LineObject::intersect(const GeometricObject &other) const {
   return other.intersect(*this);
}

ObjectSet LineObject::intersect(const Point &other) const {
   return other.intersect(*this);
}

ObjectSet LineObject::intersect(const LineObject &other) const {
   ObjectSet result;
   Point p1 = start;
   Point p2 = end;
   Point q1 = other.getStart();
   Point q2 = other.getEnd();
   double dx1 = p2.getX() - p1.getX();
   double dy1 = p2.getY() - p1.getY();
   double dx2 = q2.getX() - q1.getX();
   double dy2 = q2.getY() - q1.getY();
   double o1 = p1.getX() - q1.getX();
   double o2 = p1.getY() - q1.getY();
   // решаем систему уравнений методом Крамера:
   double denominator = dx1 * dy2 - dy1 * dx2;
   if (denominator == 0) {
      if (this->onLine(q1)) { //лежат на одной прямой
         double ta = this->where(q1);
         double tb = this->where(q2);
         double left, right;
         double tas, tbs; // ta` и tb`
         bool flag = false; // флаг на пересечение

         if (ta<0) {
            if (this->checkT(ta)) {
               left = ta;
               flag = true;
            } else {
               tas = other.where(p1);
               if (other.checkT(tas)) {
                  left = 0;
                  flag = true;
               }
            }
         } else {
            tas = other.where(p1);
            if (other.checkT(tas)) {
               left = 0;
               flag = true;
            } else if (this->checkT(ta)) {
               left = ta;
               flag = true;
            }
         }

         if (tb<1) {
            tbs = other.where(p2);
            if (other.checkT(tbs)) {
               right = 1;
            } else if (this->checkT(tb)) {
               right = tb;
            }
         } else {
            if (this->checkT(tb)) {
               right = tb;
            } else {
               tbs = other.where(p2);
               if (other.checkT(tbs)) {
                  right = 1;
               }
            }
         }

         if (flag) {
            Point rp1 = makePoint(left);
            Point rp2 = makePoint(right);

            if (this->checkT(left-1) && other.checkT(other.where(makePoint(left-1)))) { //  left-Ok
               if (this->checkT(right+1) && other.checkT(other.where(makePoint(right+1)))) { //right-Ok
                  result.add(make_shared<Line>(rp1, rp2));
               } else {
                  result.add(make_shared<Ray>(rp2, rp1));
               }
            } else { //  left-NoOk
               if (this->checkT(right+1) && other.checkT(other.where(makePoint(right+1)))) { //right-Ok
                  result.add(make_shared<Ray>(rp1, rp2));
               } else {
                  result.add(make_shared<Segment>(rp1, rp2));
               }
            }
         }
         return result;
      } else {
         return result; // параллельны, возвращается пустой ObjectSet
      }
   }
   double denominator1 = o2 * dx1 - o1 * dy1;
   double denominator2 = dx2 * o2 - dy2 * o1;
   double t_prime = denominator1 / denominator;
   double t = denominator2 / denominator;

   if (other.checkT(t_prime) && this->checkT(t)) {
      Point intersection = this->makePoint(t);
      result.add(make_shared<Point>(intersection));
   }
   return result;
}

ObjectSet LineObject::intersect(const ObjectSet &other) const {
   return other.intersect(*this);
}
double LineObject::distanceTo(const GeometricObject &other) const {
   return other.distanceTo(*this);
}
double LineObject::distanceTo(const Point &other) const {
   return other.distanceTo(*this);
}
double LineObject::distanceTo(const LineObject &other) const {
   ObjectSet result;
   Point a = other.getStart();
   Point b = other.getEnd();
   result = this->intersect(other);
   if (result.isempty()) {
      double r1 = a.distanceTo(*this);
      double r2 = b.distanceTo(*this);
      double r3 = start.distanceTo(other);
      double r4 = end.distanceTo(other);
      return min({r1, r2, r3, r4});
   } else {
      return 0;
   }
}
double LineObject::distanceTo(const ObjectSet &other) const {
   return other.distanceTo(*this);
}

bool LineObject::onLine(const Point &other) const {
   double x = other.getX();
   double y = other.getY();
   double x1 = start.getX();
   double y1 = start.getY();
   double x2 = end.getX();
   double y2 = end.getY();
   return fabs((x-x1)*(y2-y1)-(y-y1)*(x2-x1))<0.01;
}

double LineObject::where(const Point &other) const {
   double x = other.getX();
   double y = other.getY();
   double x1 = start.getX();
   double y1 = start.getY();
   double x2 = end.getX();
   double y2 = end.getY();
   double t;
   if (fabs(x2-x1)>0.01) {
      t = (x-x1)/(x2-x1);
   } else {
      t = (y-y1)/(y2-y1);
   }
   return t;
}

Point LineObject::makePoint(double t) const {
   Point p1 = start;
   Point p2 = end;
   double dx1 = p2.getX() - p1.getX();
   double dy1 = p2.getY() - p1.getY();
   return Point(p1.getX() + dx1 * t, p1.getY() + dy1 * t);
}

// ------------------ ObjectSet ------------------
void ObjectSet::move(double dx, double dy) {
   for (size_t i=0; i<objects.size(); i++) {
      objects[i]->move(dx, dy);
   }
}

void ObjectSet::rotate(double radians) {
   for (size_t i=0; i<objects.size(); i++) {
      objects[i]->rotate(radians);
   }
}

ObjectSet ObjectSet::intersect(const GeometricObject &other) const {
   return other.intersect(*this);
}
ObjectSet ObjectSet::intersect(const Point &other) const {
   ObjectSet result;
   for (size_t i = 0; i < objects.size(); i++) {
      ObjectSet temp = objects[i]->intersect(other);
      for (size_t j = 0; j < temp.objects.size(); j++) {
         result.add(temp.objects[j]);
      }
   }
   return result;
}
ObjectSet ObjectSet::intersect(const LineObject &other) const {
   ObjectSet result;
   for (size_t i = 0; i < objects.size(); i++) {
      ObjectSet temp = objects[i]->intersect(other);
      for (size_t j = 0; j < temp.objects.size(); j++) {
         result.add(temp.objects[j]);
      }
   }
   return result;
}
ObjectSet ObjectSet::intersect(const ObjectSet &other) const {
   ObjectSet result;
   for (size_t i = 0; i < objects.size(); i++) {
      for (size_t j = 0; j < other.objects.size(); j++) {
         ObjectSet temp = objects[i]->intersect(*other.objects[j]);
         for (size_t k = 0; k < temp.objects.size(); k++) {
            result.add(temp.objects[k]);
         }
      }
   }
   return result;
}

double ObjectSet::distanceTo(const GeometricObject &other) const {
   return other.distanceTo(*this);
}
double ObjectSet::distanceTo(const Point &other) const {
   double min_r = other.distanceTo(*this->objects[0]);
   for (size_t i = 1; i < objects.size(); i++) {
      double r = other.distanceTo(*this->objects[i]);
      if (r < min_r) min_r = r;
      if (min_r == 0) return 0;
   }
   return min_r;
}
double ObjectSet::distanceTo(const LineObject &other) const {
   double min_r = other.distanceTo(*this->objects[0]);
   for (size_t i = 1; i < objects.size(); i++) {
      double r = other.distanceTo(*this->objects[i]);
      if (r < min_r) min_r = r;
      if (min_r == 0) return 0;
   }
   return min_r;
}
double ObjectSet::distanceTo(const ObjectSet &other) const {
   double min_r = this->objects[0]->distanceTo(*other.objects[0]);
   for (size_t i = 0; i < objects.size(); i++) {
      for (size_t j = 0; j < other.objects.size(); j++) {
         double r = this->objects[i]->distanceTo(*other.objects[j]);
         if (r < min_r) min_r = r;
         if (min_r == 0) return 0;
      }
   }
   return min_r;
}

shared_ptr<GeometricObject> ObjectSet::getObject(size_t index) const {
   if (index >= objects.size() || index < 0) {
      throw out_of_range("Index выходит за пределы");
   }
   return objects[index];
}