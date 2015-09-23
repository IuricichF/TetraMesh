#ifndef TETRA_H
#define TETRA_H

#include "Triangle.h"

#define coutTInd(C,T) if(T==noAdjacentTetraIndex)C<<"None";else C<<T;
#define coutPosInd(C,P) if(P==unknown)C<<"Unknown";else C<<P;

// Class storing complete topological information for tetrahedra (top simplices) with reference to geometrical information of their vertices
class Tetra{
private:
    VertexIndex tetraTV[4]; // Indices of geometrical information of the vertices
    TetraIndex tetraTT[4]; // Adjacency relations

public:

    // Constructor
    inline Tetra(){
        tetraTV[0]=-1;
        tetraTV[1]=-1;
        tetraTV[2]=-1;
        tetraTV[3]=-1;
        setTT(noAdjacentTetraIndex,noAdjacentTetraIndex,noAdjacentTetraIndex,noAdjacentTetraIndex);
    }

    inline Tetra(VertexIndex a, VertexIndex b, VertexIndex c, VertexIndex d){
        tetraTV[0]=a;
        tetraTV[1]=b;
        tetraTV[2]=c;
        tetraTV[3]=d;
        setTT(noAdjacentTetraIndex,noAdjacentTetraIndex,noAdjacentTetraIndex,noAdjacentTetraIndex);
    }

    // Vertex replacement constructor
    inline Tetra(VertexIndex Avertex, PositionIndex vertexAFinalPosition, Tetra* oldTetra){
        for(PositionIndex ind=0; ind<=3; ind++){
            tetraTV[ind]=oldTetra->TV(ind);
        }
        tetraTV[vertexAFinalPosition]=Avertex;
        setTT(noAdjacentTetraIndex,noAdjacentTetraIndex,noAdjacentTetraIndex,noAdjacentTetraIndex);
        setTT(vertexAFinalPosition,oldTetra->TT(vertexAFinalPosition));
    }

    // Single vertex index setter
    inline void setTV(PositionIndex vertexPosition, VertexIndex vertex){
        tetraTV[vertexPosition]=vertex;
    }

    // Complete adjacency relations setter
    inline void setTT(TetraIndex a, TetraIndex b, TetraIndex c, TetraIndex d){
        tetraTT[0]=a;
        tetraTT[1]=b;
        tetraTT[2]=c;
        tetraTT[3]=d;
    }

    // Single adjacency relation setter
    inline void setTT(PositionIndex oppositeVertexPosition, TetraIndex tetra){
        tetraTT[oppositeVertexPosition]=tetra;
    }

    // Vertices getter
    inline VertexIndex TV(PositionIndex vertexPosition) const{
        return tetraTV[vertexPosition];
    }

    // Adjacent tetrahedra getter
    inline TetraIndex TT(PositionIndex TT) const{
        return tetraTT[TT];
    }

    // Inverse adjacency relations getter
    inline PositionIndex inverseTT(Tetra* t1) const{
        for(PositionIndex i=0; i<=3; i++)
            if(!this->contains(t1->TV(i)))
                return i;

        return 4;
    }

    // true if the given vertex is on the boundary of the tetrahedron
    inline bool contains(VertexIndex vertex) const{
        for(PositionIndex pindex=0; pindex<=3; pindex++){
            if(TV(pindex)==vertex) return true;
        }
        return false;
    }

    // Internal Index of the given vertex inside the tetrahedron
    inline short int vertexIndex(VertexIndex vertex) const{
        for(PositionIndex pindex=0; pindex<=3; pindex++){
            if(TV(pindex)==vertex) return pindex;
        }
        return -1;
    }

    // Internal Index of the given face (triangle) inside the tetrahedron
    inline short int faceIndex(VertexIndex v1, VertexIndex v2, VertexIndex v3) const{
        for(PositionIndex pindex=0; pindex<=3; pindex++){
            if(tetraTV[pindex]!= v1 && tetraTV[pindex]!= v2 && tetraTV[pindex]!= v3) return pindex;
        }
        return -1;
    }

    // true if the given triangle is on the boundary of the tetrahedron
    inline bool contains(Triangle face){
        for(PositionIndex pindex=0; pindex<=3; pindex++){
            if(face.contains(TV(pindex))) return true;
        }
        return false;
    }

    // Faces extraction
    inline Triangle* TF(PositionIndex faceIndex) const{
        PositionIndex indexes[3];
        int filled=0;
        for(PositionIndex index=0; index<=3; index++){
            if(index!=faceIndex) indexes[filled++]=index;
        }
        return new Triangle(TV(indexes[0]),TV(indexes[1]),TV(indexes[2]),faceIndex);
    }

    // Standard output operator
    inline friend ostream & operator << (ostream & out, Tetra const& tetra){
        out << "Tetra[(" << tetra.TV(0) << " " << tetra.TV(1) << " " << tetra.TV(2) << " " << tetra.TV(3) << ")"<<endl;
        out << "adj-(" << tetra.TT(0) << " " << tetra.TT(1) << " " << tetra.TT(2) << " " << tetra.TT(3) << ")"<<endl;
        out << "]";
        return out;
    }

};


#endif // TETRA_H
