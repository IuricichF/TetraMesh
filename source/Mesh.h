#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>
#include <queue>
#include <set>
#include "Tetra.h"


// Class storing the tetrahedral mesh
class Mesh{
protected:
    //bool printingForTesting;
    std::vector<Vertex> vertices; // std::vector storing vertices
    std::vector<Tetra> tetras; // std::vector storing tetrahedra

public:
    // Constructor
    inline Mesh(){
    }

    // Vertex reference getter
    inline Vertex& getVertex(VertexIndex index){return vertices[index];}

    // Tetrahedron reference getter
    inline Tetra& getTetra(TetraIndex index){return tetras[index];}

    // Number of stored vertices
    inline VertexIndex vertexNumber(){return vertices.size();}

    // Number of stored tetrrahedra
    inline TetraIndex tetraNumber(){return tetras.size();}



    // Functions retrieving the star of a vertex (simplexes incident to the vertex)
    std::vector<TetraIndex>* VT(VertexIndex center);
    //
    std::vector<TetraIndex> VT(VertexIndex center,bool* foundBorder);
    //
    std::vector<Triangle> VF(VertexIndex center);
    //
    std::vector<Edge>* VE(VertexIndex center);


    // Functions retrieving the star of an edge (simplexes incident to an edge and bigger than it)
    std::vector<TetraIndex>* ET(Edge* edge);
    //
    std::vector<Triangle*>* EF(Edge* edge);


    // Functions retrieving the star of a face (simplexes incident to a triangle and bigger than it)
    TetraIndex FTstar(Triangle face);
    //
    std::vector<TetraIndex>* FT(Triangle* face);



    // Functions retrieving the adjacents of a simplex
    std::vector<VertexIndex>* VV(VertexIndex center);
    //
    std::vector<Edge> EE(Edge edge);
    //
    std::vector<Triangle*> FF(Triangle face);
    //
    // std::vector<Triangle*> TT is in the class Tetra


    // return true if the vertex is on the boundary of the mesh
    bool isBoundary(VertexIndex center);

    // Mesh building function
    void build();

    ///IO functions---------
    // Loader for TS tetrahedral mesh files
    void loadTS(FILE* TSfile);

    // Loader for RAW tetrahedral mesh files
    void loadRAW(FILE* RAWfile,int x, int y, int z);

    // Loader for RAW tetrahedral mesh files
    void writeTS(char* file_name);

    //write a new mesh in .ts format with the original scalar field switched with fieldvalues
    void writeTS(char* file_name, vector<float> fieldvalues);

    //write a file with a column of values taken from fieldvalues (first value is the number of values)
    void writeField(char* file_name, vector<float> fieldvalues);

    //write a new mesh in .vtk format with the original scalar field switched with fieldvalues (for paraview)
    void writeVTK(char* file_name, vector<VertexIndex>* segmentation1);

    vector<float> readField(char* file_name);/// read a field from file


protected:
    //Functions

    // Index sorting function
    void sortIndexes(PositionIndex *indexes, int numberOfIndexes, Tetra const& tetra);

    // Adding a vertex to the mesh
    inline void pushVertex(Vertex* vertex){
        vertices.push_back(*vertex);
    }

    // Adding a tetrahedron the to mesh
    inline void pushTetra(Tetra const& tetra){
        tetras.push_back(tetra);
    }

    // Sorts lexicographically triples of indices
    inline void sortFaces(vector<pair<Triangle,TetraIndex>* >* faces){
        sortFaces(faces,0,faces->size());
    }

    // Sorts lexicographically subsets of a list of triples of indices
    void sortFaces(vector<pair<Triangle,TetraIndex>* >* faces, int left, int right);

    // Merges sorted sublists of triples of indices
    void merge(vector<pair<Triangle,TetraIndex>* >* faces, int left, int center, int right);


    //Utility Function
    template<class C> bool isIntoVector(C c, vector<C> &c_vect);


    // Mesh coherence test
    void testCoherence();

};

#endif // MESH_H
