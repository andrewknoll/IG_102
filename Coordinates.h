#ifndef COORDINATES
#define COORDINATES

class Coordinates{
    protected:
        float c[4];

    public:
        Coordinates();
        Coordinates(float x, float y, float z, float w);

        float getX() const;
        float getY() const;
        float getZ() const;
        float getW() const;

        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void setW(float w);

        void setAll(float x, float y, float z);

        Coordinates baseChange(Coordinates o, Coordinates u, Coordinates v, Coordinates w);
};

#endif