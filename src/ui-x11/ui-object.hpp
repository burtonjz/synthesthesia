#ifndef UI_OBJECT_HPP_
#define UI_OBJECT_HPP_

#include <array>

class UIObject {
public:
    /**
     * @brief UIObject Constructor
     * 
     * @param x The x-coordinate corresponding with the top-left corner of the object
     * @param y The y-coordinate corresponding with the top-left corner of the object
     * @param index The Object index (e.g., widgets corresponding to Oscillator[0])
     * 
    */
    UIObject(double x, double y, int index);

    /**
     * @brief virtual desctructure for cleanup of derived objects
    */
    virtual ~UIObject() = default;

    /**
    * @brief Virtual function providing the number of widgets the object contains
    * 
    * @return The number of widgets contained by the object.
    */
    virtual int getNumWidgets() const = 0;

    /**
    * @brief Virtual function providing the height of the object
    * 
    * @return double height of object
    */
    virtual double getObjectHeight() const = 0;

    /**
    * @brief Virtual function providing the width of the object
    * 
    * @return double width of object 
    */
    virtual double getObjectWidth() const = 0;

    /**
    * @brief common member function to return size details of object
    * 
    * @return std::array<double,4> x,y,width,height
    */
    std::array<double,4> getObjectDimensions() const;

protected:
    double x_;
    double y_;
    int index_;
};

#endif // UI_OBJECT_HPP_
