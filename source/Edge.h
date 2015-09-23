#ifndef EDGE_H
#define EDGE_H
#include "Vertex.h"


    // Explicit representation of an edge
    class Edge{
    private:
        VertexIndex vertexes[2]; // Indices of the endpoints
    public:

        // Constructor
        inline Edge(){}
        inline Edge(VertexIndex vi1, VertexIndex vi2){
            vertexes[0]=min(vi1,vi2);
            vertexes[1]=max(vi1,vi2);
        }

        // Smaller-index endpoint getter
        inline VertexIndex minindex() const {return vertexes[0];}

        // Bigger-index endpoint getter
        inline VertexIndex maxindex() const {return vertexes[1];}

        // Index of the vertex in position vPos
        inline VertexIndex EV(PositionIndex vPos)
        {
            return vertexes[vPos];
        }

        // Index of verex v inside the edge
        inline short vertexIndex(VertexIndex v){
            if(vertexes[0]==v) return 0;
            if(vertexes[1]==v) return 1;
            return -1;
        }

        // Operators ----------
        inline bool operator==(Edge edge){
            if(minindex()!=edge.minindex()) return false;
            if(maxindex()!=edge.maxindex()) return false;
            return true;
        }

        inline bool operator!=(Edge edge){
            return !((*this)==(edge));
        }

        inline bool operator<(Edge edge) const{
            if(maxindex() < edge.maxindex()) return true;
            else if(maxindex() == edge.maxindex() && minindex() < edge.minindex()) return true;
            else return false;
        }

        inline bool operator>(Edge edge) const{
            if(maxindex() > edge.maxindex()) return true;
            else if(maxindex() == edge.maxindex() && minindex() > edge.minindex()) return true;
            else return false;
        }

        // Standard output operator
        inline friend ostream & operator << (ostream & out, Edge & edge){
            out << "Edge[(" << edge.minindex() << " " << edge.maxindex() << ")]"<<endl;
            return out;
        }

    };


#endif // EDGE_H
