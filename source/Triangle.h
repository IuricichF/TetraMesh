#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Edge.h"

    // Explicit representation of a triangle
    class Triangle{
    private:
        VertexIndex vertexes[3]; // Vertex indices
        PositionIndex fIndex; // Position index of the face in the original tetrahedron (if appliable)

    public:

        // Constructor
        inline Triangle(VertexIndex vi1, VertexIndex vi2, VertexIndex vi3, PositionIndex index=4){
            vertexes[0]=min(min(vi1,vi2),vi3);
            vertexes[2]=max(max(vi1,vi2),vi3);
            if(vi1>minindex()&&vi1<maxindex()) vertexes[1]=vi1;
            else if(vi2>minindex()&&vi2<maxindex()) vertexes[1]=vi2;
            else vertexes[1]=vi3;
            fIndex=index;
        }

        // Null constructor
        inline Triangle(){
            vertexes[0]=vertexes[1]=vertexes[2]=0;
            fIndex=4;
        }

        // Swallow copy
        inline Triangle* clone(){
            return new Triangle(minindex(),middleindex(),maxindex(),faceIndex());
        }

        // Smaller-index vertex getter
        inline VertexIndex minindex() const {return vertexes[0];}

        // Middle-index vertex getter
        inline VertexIndex middleindex() const {return vertexes[1];}

        // Bigger-index vertex getter
        inline VertexIndex maxindex() const {return vertexes[2];}

        // PositionIndex getter
        inline PositionIndex faceIndex(){return fIndex;}

        // Vertices getter
        inline VertexIndex FV(PositionIndex index){return vertexes[index%3];}

        // Bounding edges getter
        inline Edge* FE(PositionIndex index){return new Edge(vertexes[(index+1)%3],vertexes[(index+2)%3]);}

        // true if the triangle contains the given vertex
        inline bool contains(VertexIndex vertex){
            for(PositionIndex pindex=0; pindex<3; pindex++){
                if(FV(pindex)==vertex) return true;
            }
            return false;
        }

        // Operators-----------
        inline bool operator==(Triangle triangle){
            if(minindex()!=triangle.minindex()) return false;
            if(middleindex()!=triangle.middleindex()) return false;
            if(maxindex()!=triangle.maxindex()) return false;
            return true;
        }

        inline bool operator!=(Triangle triangle){
            return !((*this) == triangle);
        }

        inline bool operator<(Triangle tri) const{
            if(maxindex() < tri.maxindex()) return true;
            else if(maxindex() == tri.maxindex() && middleindex() < tri.middleindex()) return true;
            else if(maxindex() == tri.maxindex() && middleindex() == tri.middleindex() && minindex() < tri.minindex()) return true;
            else return false;
        }

        inline bool operator>(Triangle tri) const{
            if(maxindex() > tri.maxindex()) return true;
            else if(maxindex() == tri.maxindex() && middleindex() > tri.middleindex()) return true;
            else if(maxindex() == tri.maxindex() && middleindex() == tri.middleindex() && minindex() > tri.minindex()) return true;
            else return false;
        }

        // Standard output operator
        inline friend ostream & operator << (ostream & out, Triangle & triangle){
            out << "Triangle[(" << triangle.minindex() << " " << triangle.middleindex() << " " << triangle.maxindex() << ")]";
            return out;
        }

    };


#endif // TRIANGLE_H
