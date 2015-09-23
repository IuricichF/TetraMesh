#include <iostream>

#include "source/Mesh.h"
#include "assert.h"

using namespace std;


///Examples for a quick start
/// usage: ./TetraMesh filename.ts
///        ./TetraMesh filename.raw xResolution yResolution zResolution
int main(int argc, char* argv[])
{

    ///How to search and read relations among simplices:
    /// - vertices             = V
    /// - edges                = E
    /// - triangles (faces)    = F
    /// - tetrahedra           = T
    ///
    /// Each function for extracting relations among simplices is named based on the start simplex and the extracted simplcies:
    /// Vertex-Edge relation
    ///     Function: VE();
    ///     Return: set of edges incident into v;
    ///     Similar functions: VV, VF, VT
    ///
    /// Edge-Face relation
    ///     Function: EF(edge)
    ///     Return: set of faces incident to edge
    ///     Similar functions: EE, ET
    ///
    /// ....


    ///How to:
    ///
    FILE* inputFile = fopen(argv[1],"r");
    Mesh mesh = Mesh();

    if(strstr(argv[1],".ts") != NULL){
        ///- read the mesh from an input file (.ts format)
        mesh.loadTS(inputFile);
    }
    else{
        assert(argc == 5);
        ///- read the mesh from an input file (.raw format)
        mesh.loadRAW(inputFile, atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
    }

    ///- initialize the data structure
    mesh.build();

    ///- iterate other the vertices
    for(int i=0; i<mesh.vertexNumber(); i++){

        ///- print out its coordinates and its field value
        printf("%f %f %f %f \n", mesh.getVertex(i).x(), mesh.getVertex(i).y(), mesh.getVertex(i).z(), mesh.getVertex(i).fieldValue());

        ///- extract its adjacent vertices
        vector<int>* vv = mesh.VV(i);
        delete vv;

        ///- extract its incident tetrahedra
        vector<TetraIndex>* vt =mesh.VT(i);
        delete vt;
    }

    ///- iterate other the tetrahedra
    for(int i=0; i<mesh.tetraNumber(); i++){

        ///- print out the indexes of its vertices
        printf("%d %d %d %d \n", mesh.getTetra(i).TV(0), mesh.getTetra(i).TV(1), mesh.getTetra(i).TV(2), mesh.getTetra(i).TV(3));

        ///- extract boundary triangles for each tetrahedron
        for(int f=0; f<4; f++){
            Triangle* face = mesh.getTetra(i).TF(f);
            printf("%d %d %d \n", face->FV(0), face->FV(1), face->FV(2));
        }
    }



////print mesh
//    for(int i=0; i<mesh.vertexNumber(); i++){
//        cout << mesh.getVertex(i) << endl;
//    }

//    for(int i=0; i<mesh.tetraNumber(); i++){
//        cout << mesh.getTetra(i) << endl;
//    }


////check if VT and TT relations are correctly initialized
//    map<VertexIndex, set<TetraIndex> > vt;
//    map<TetraIndex, set<TetraIndex> > tt;

//    for(int i=0; i<mesh.tetraNumber(); i++){
//        for(int v=0; v<4; v++){
//            if(vt.find(mesh.getTetra(i).TV(v)) == vt.end()){
//                vt[mesh.getTetra(i).TV(v)]=set<TetraIndex>();
//            }
//            vt[mesh.getTetra(i).TV(v)].insert(i);
//        }

//        for(int adj=0; adj<4; adj++){
//            int tAdj = mesh.getTetra(i).TT(adj);

//            if(tAdj != noAdjacentTetraIndex){
//                int pos = mesh.getTetra(i).inverseTT(&mesh.getTetra(tAdj));

//                assert(mesh.getTetra(tAdj).TT(pos) == i);
//            }
//        }

//    }

//    for(int i=0; i<mesh.vertexNumber(); i++){
//        vector<TetraIndex>* vthere = mesh.VT(i);
//        set<TetraIndex> vtOrig = vt[i];
//        assert(vthere->size() == vtOrig.size());
//        for(auto t : *vthere){
//            vtOrig.erase(t);
//        }
//        assert(vtOrig.size() == 0);
//    }

}

