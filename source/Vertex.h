#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>

#include "Define.h"

using namespace std;

// Class storing geometrical information of vertices
class Vertex {
    private:
        float vertexCoordinates[3]; // Coordinates
        float vertexFieldValue; // Scalar value
        TetraIndex vertexVTstar; // VT* relationship


    public:

        // Constructor
        inline Vertex(){
            vertexCoordinates[0]=0;
            vertexCoordinates[1]=0;
            vertexCoordinates[2]=0;
            setFieldValue(0);
            setVTstar(uninitializedTetraIndex);
        }

        inline Vertex(float x, float y, float z,float value){
            vertexCoordinates[0]=x;
            vertexCoordinates[1]=y;
            vertexCoordinates[2]=z;
            setFieldValue(value);
            setVTstar(uninitializedTetraIndex);
        }

        // Field value setter
        inline void setFieldValue(float value){
            vertexFieldValue=value;
        }

        // VT* setter
        inline void setVTstar(TetraIndex VTstar){
            vertexVTstar=VTstar;
        }

        // First float getter
        inline float x() const{
            return vertexCoordinates[0];
        }

        // Second float getter
        inline float y() const{
            return vertexCoordinates[1];
        }

        // Third float getter
        inline float z() const{
            return vertexCoordinates[2];
        }

        // Field value getter
        inline float fieldValue() const{
            return vertexFieldValue;
        }

        // VT* getter
        inline TetraIndex VTstar() const{
            return vertexVTstar;
        }


        // Operators -----------
        inline bool operator==(Vertex vertex){
            return x()==vertex.x() && y()==vertex.y() && z()==vertex.z();
        }

        // Standard output operator
        inline friend ostream & operator << (ostream & out, Vertex const& vertex){
            out << "Vertex[(" << vertex.x() << " " << vertex.y() << " " << vertex.z() << " " << vertex.fieldValue() << ")"<<endl;
            out << "VT*:" << vertex.VTstar() <<endl;
            out << "]";
            return out;
        }

        // Difference operator
        inline Vertex* operator-(Vertex other){
            return new Vertex(x()-other.x(),y()-other.y(),z()-other.z(),fieldValue()-other.fieldValue());
        }
};


#endif // VERTEX_H
