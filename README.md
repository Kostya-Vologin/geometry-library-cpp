# geometry-library-cpp
Class library for analytical geometry on a plane.

main.cpp it is only necessary for testing library functions.

To build the library, you need a compiler that supports at least C++20.

No installation or configuration is required. To work, you need to add the geometry.hpp files and 
geometry.cpp in a project that requires calculations in the field of analytical geometry on a plane.

The library supports working with the following concepts: a point, a straight line, a segment, a ray, a set of objects (points, segments, etc.) for polylines and intersection results.

## Library features:

* moving an object &mdash; shifting coordinates by specified values along the X and Y axes;
* object rotation &mdash; displacement by a certain angle relative to the coordinate center;
* intersection search (a set of objects is returned);
* calculation of the distance between objects (minimum of distances up to 2 points of the object).

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

##