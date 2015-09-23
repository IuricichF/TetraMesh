#include "Mesh.h"


void Mesh :: sortIndexes(PositionIndex *indexes, int numberOfIndexes, Tetra const& tetra){

    for(int i=0; i<numberOfIndexes; i++){
        int minimum=i;
        for(int j=i+1; j<numberOfIndexes; j++){
            if(tetra.TV(indexes[j])<tetra.TV(indexes[minimum])) minimum=j;
        }

        PositionIndex tmp=indexes[i];
        indexes[i]=indexes[minimum];
        indexes[minimum]=tmp;
    }

}

void Mesh :: build(){
    unsigned int VTstarSet=0;
    unsigned int TTandInverseTTSet=0;
    std::vector<pair<Triangle,TetraIndex>* > faces;
    faces.reserve(this->tetraNumber()*4);


    Triangle* tri;
    Tetra tetra;


    for(TetraIndex index=0; index<tetraNumber(); index=index+1){
        tetra=getTetra(index);

        for(PositionIndex vp=0; vp<=3; vp++){

            Vertex& vertex=getVertex(tetra.TV(vp));

            if(vertex.VTstar()==uninitializedTetraIndex){
                VTstarSet++;
                vertex.setVTstar(index);
            }

              tri = tetra.TF(vp);
              faces.push_back(new pair<Triangle,TetraIndex>(*(tri),index));
              delete tri;

        }
    }


    sortFaces(&faces);

    Triangle firstFace;
    Triangle secondFace;
    TetraIndex firstIndex, secondIndex;



    for(unsigned int i=0; i<faces.size()-1; i++){
        firstFace=faces[i]->first;
        secondFace=faces[i+1]->first;

        if((firstFace)==(secondFace)){
            firstIndex=faces[i]->second;
            secondIndex=faces[i+1]->second;

            Tetra& firstTetra=getTetra(firstIndex);
            Tetra& secondTetra=getTetra(secondIndex);
            firstTetra.setTT(firstFace.faceIndex(),secondIndex);
            secondTetra.setTT(secondFace.faceIndex(),firstIndex);
            TTandInverseTTSet+=2;

            delete faces[i];
            delete faces[i+1];
            i++;

        }
        else{
            delete faces[i];
        }
    }

    faces.clear();
}



void Mesh :: sortFaces(vector<pair<Triangle,TetraIndex>* >* faces, int left, int right){

    if (right-left>2) {
        int center = (left+right)/2;
        sortFaces(faces, left, center);
        sortFaces(faces, center, right);
        merge(faces, left, center, right);
    }
    else{
        //int compare=minusTriangle(faces->at(left)->first,faces->at(right-1)->first);
        if(faces->at(left)->first>faces->at(right-1)->first){
            pair<Triangle,TetraIndex>* tmp=faces->at(right-1);
            faces->at(right-1)=faces->at(left);
            faces->at(left)=tmp;
        }
    }
}


void Mesh :: loadTS(FILE* TSfile){
    int vNumber;
    int tNumber;
    float x;
    float y;
    float z;
    float field;
    VertexIndex a,b,c,d;

    fscanf(TSfile,"%d %d ",&vNumber,&tNumber);
    vertices=std::vector<Vertex >(vNumber);
    tetras=std::vector<Tetra>(tNumber);
    for(VertexIndex i=0; i<vertexNumber(); i=i+1){
        fscanf(TSfile,"%f %f %f %f ",&x,&y,&z,&field);
        vertices[i]= Vertex(x,y,z,field);
    }
    for(TetraIndex i=0; i<tetraNumber(); i++){
        fscanf(TSfile,"%d %d %d %d ",&a,&b,&c,&d);
        tetras[i]= Tetra(a,b,c,d);
    }
    fclose(TSfile);

}


void Mesh :: loadRAW(FILE* TSfile, int x, int y, int z){

    unsigned char *fileBuf;	// Pointer to our buffered data

    // Open the file in binary mode using the "rb" format string
    // This also checks if the file exists and/or can be opened for reading correctly
    if (TSfile == NULL){
        cout << "File is not open" << endl;
        return;
    }

    // Get the size of the file in bytes
    long fileSize = x*y*z;//getFileSize(file);

    // Allocate space in the buffer for the whole file
    fileBuf = new unsigned char[fileSize];

    // Read the file in to the buffer
    fread(fileBuf, fileSize, 1, TSfile);


    vertices=vector<Vertex>(x*y*z);
    tetras=vector<Tetra>((x*y*z - ((x*y)+(y*(z-1))+((z-1)*(x-1))))*6);

    int tot = 0;
    for(int i=0; i<z; i++){
        for(int j=0; j<y; j++){
            for(int k=0; k<x; k++){
                vertices[tot]= Vertex(k,j,i,fileBuf[tot]);
                tot++;
            }
        }
    }

    tot=0;
    for(int h=0; h<z-1; h++){
        for(int j=0; j<y-1; j++){
            for(int k=0; k<x-1; k++){
                int i = k + (j*x) + (x*y)*h;

                    tetras[tot++]= Tetra(i+1,i,x+i,x*y +i);
                    tetras[tot++]= Tetra(i+x+1,i+1,x+i,x*y +i);
                    tetras[tot++]= Tetra(i+x+1,x*y +i+1,i+1,x*y +i);
                    tetras[tot++]= Tetra(i+x+1,x*y+x+i,x*y +i+1,x*y+i);
                    tetras[tot++]= Tetra(i+x+1,x+i,x*y+x+i,x*y +i);
                    tetras[tot++]= Tetra(i+x+1,x*y+x+i+1,x*y+i+1,x*y+x+i);

            }
        }
    }

    fclose(TSfile);
}


std::vector<Triangle*> Mesh :: FF(Triangle face)
{
    vector<Triangle*> faces;
    vector<Triangle*>* tetra0 = this->EF(face.FE(0));
    vector<Triangle*>* tetra1 = this->EF(face.FE(1));
    vector<Triangle*>* tetra2 = this->EF(face.FE(2));
    for(unsigned int i=0;i<tetra0->size();i++)
    {
        if(face != *tetra0->at(i))
        {
            faces.push_back(tetra0->at(i));
        }
    }
    for(unsigned int i=0;i<tetra1->size();i++)
    {
        if(face != *tetra1->at(i))
        {
            faces.push_back(tetra1->at(i));
        }
    }
    for(unsigned int i=0;i<tetra2->size();i++)
    {
        if(face != *tetra2->at(i))
        {
            faces.push_back(tetra2->at(i));
        }
    }
    return faces;
}


std::vector<TetraIndex>* Mesh :: FT(Triangle* face)
{
    TetraIndex t = FTstar(*face);
    vector<TetraIndex>* ret = new vector<TetraIndex>();
    ret->push_back(t);
    TetraIndex t1 = getTetra(t).TT(getTetra(t).faceIndex(face->FV(0),face->FV(1),face->FV(2)));
    if(t1 != noAdjacentTetraIndex)
        ret->push_back(t1);

    return ret;
}


TetraIndex Mesh :: FTstar(Triangle face)
{
    set<TetraIndex> tetras;
    TetraIndex initial = getVertex(face.FV(0)).VTstar();

    queue<TetraIndex> coda;

    tetras.insert(initial);

    PositionIndex k;
    TetraIndex attuale;

    if(getTetra(initial).contains(face.FV(0)) && getTetra(initial).contains(face.FV(1)) && getTetra(initial).contains(face.FV(2))){
        return initial;
    }
    else{
        coda.push(initial);
    }

    while(!coda.empty()){
        attuale = coda.front();

        k = getTetra(attuale).vertexIndex(face.FV(0));

        if(getTetra(attuale).contains(face.FV(0)) && getTetra(attuale).contains(face.FV(1)) && getTetra(attuale).contains(face.FV(2))){
            return attuale;
        }

        for(PositionIndex pos = 1; pos <= 3; pos++)
        {
            TetraIndex adj = this->getTetra(attuale).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && tetras.find(adj) == tetras.end())
            {
                coda.push(adj);
                tetras.insert(adj);
            }
        }
        coda.pop();
    }

    return -1;
}


std::vector<Edge> Mesh :: EE(Edge edge)
{
    vector<Edge> edges;
    vector<Edge>* edge0 = this->VE(edge.EV(0));
    vector<Edge>* edge1 = this->VE(edge.EV(1));
    for(unsigned int i=0;i<edge0->size();i++)
    {
        if(edge != edge0->at(i))
            edges.push_back(edge0->at(i));
    }
    for(unsigned int i=0;i<edge1->size();i++)
    {
        if(edge != edge1->at(i))
            edges.push_back(edge1->at(i));
    }
    return edges;
}


std::vector<Triangle*>* Mesh :: EF(Edge* edge)
{

    vector<Triangle*>* faces = new vector<Triangle*>();
    TetraIndex first = getVertex(edge->EV(0)).VTstar();
    TetraIndex adj;
    if(!getTetra(first).contains(edge->EV(1))){

        set<TetraIndex>* tetras = new set<TetraIndex>();
        queue<TetraIndex>* coda = new queue<TetraIndex>();
        coda->push(first);
        tetras->insert(first);

        PositionIndex k;
        TetraIndex attuale;

        while(!coda->empty()){
            attuale = coda->front();
            if(getTetra(attuale).contains(edge->EV(1))){
                first = attuale;
                break;
            }
            k = getTetra(attuale).vertexIndex(edge->EV(0));
            for(PositionIndex pos = 1; pos <= 3; pos++)
            {
                adj = this->getTetra(attuale).TT((PositionIndex)((k+pos)%4));
                if( adj != noAdjacentTetraIndex && tetras->find(adj) == tetras->end())
                {
                    coda->push(adj);
                    tetras->insert(adj);
                }
            }
            coda->pop();
        }

        delete tetras;
        delete coda;
    }


    vector<short int> directions;
    for(int i=0;i<4;i++){
        if(getTetra(first).TV(i) != (edge->EV(0)) &&
           getTetra(first).TV(i) != (edge->EV(1))){

           directions.push_back(i);
        }
    }

    short int tmp_direction;
    TetraIndex next = first;
    short int next_direction = directions[0];
    while(next != noAdjacentTetraIndex){

        faces->push_back(getTetra(next).TF(next_direction));

        if(getTetra(next).TT(next_direction) != noAdjacentTetraIndex){
            Tetra t = getTetra(getTetra(next).TT(next_direction));
            tmp_direction = getTetra(next).inverseTT(&t);
        }

        next = getTetra(next).TT(next_direction);
        if(next == noAdjacentTetraIndex) continue;
        next_direction = 6 - getTetra(next).vertexIndex(edge->EV(0)) - getTetra(next).vertexIndex(edge->EV(1)) - tmp_direction;

        if(next == first){
            break;
        }
    }

    if(next == noAdjacentTetraIndex){
        next = first;
        next_direction = directions[1];

        while(next != noAdjacentTetraIndex){
            faces->push_back(getTetra(next).TF(next_direction));

            if(getTetra(next).TT(next_direction) != noAdjacentTetraIndex){
                Tetra t=getTetra(getTetra(next).TT(next_direction));
                tmp_direction = getTetra(next).inverseTT(&t);
            }

            next = getTetra(next).TT(next_direction);
            if(next == noAdjacentTetraIndex) continue;
            next_direction = 6 - getTetra(next).vertexIndex(edge->EV(0)) - getTetra(next).vertexIndex(edge->EV(1)) - tmp_direction;

        }
    }

    return faces;
}


std::vector<TetraIndex>* Mesh :: ET(Edge* edge)
{
    vector<TetraIndex>* tetra = new vector<TetraIndex>();
    VertexIndex v0 = edge->EV(0);
    vector<TetraIndex>* vt = this->VT(v0);
    for(unsigned int w=0; w<vt->size(); w++)
    {
        bool isFound = false;
        for(PositionIndex i=0;i<=3;i++)
        {
            for(PositionIndex j=i+1;j<=3;j++)
            {
                Edge* e = new Edge(getTetra((*vt)[w]).TV(i),getTetra((*vt)[w]).TV(j));
                if(*edge == *e)
                {
                    tetra->push_back((*vt)[w]);
                    isFound = true;
                    delete e;
                    break;
                }
                delete e;
            }
            if(isFound)
                break;
        }
    }

    delete vt;

    return tetra;
}


std::vector<VertexIndex>* Mesh :: VV(VertexIndex center)
{

    set<int>* vert = new set<int>();
    set<int>* tetras = new set<int>();
    TetraIndex initial = getVertex(center).VTstar();

    queue<int> coda;
    coda.push(initial);
    tetras->insert(initial);

    PositionIndex k;
    TetraIndex attuale;

    while(!coda.empty()){
        attuale = coda.front();
        coda.pop();

        k = getTetra(attuale).vertexIndex(center);
        for(PositionIndex pos = 1; pos <= 3; pos++)
        {
            if(vert->find(this->getTetra(attuale).TV((PositionIndex)((k+pos)%4))) == vert->end())
                vert->insert(this->getTetra(attuale).TV((PositionIndex)((k+pos)%4)));
            TetraIndex adj = this->getTetra(attuale).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && tetras->find(adj) == tetras->end())
            {
                coda.push(adj);
                tetras->insert(adj);
            }
        }
    }

    vector<VertexIndex>* ret = new vector<VertexIndex>(vert->begin(), vert->end());

    delete vert;
    delete tetras;

    return ret;
}


std::vector<Edge>* Mesh :: VE(VertexIndex center)
{
    vector<Edge>* edges = new vector<Edge>();
    vector<TetraIndex> tetra;
    vector<bool> visitedTetra(tetraNumber(),false);
    TetraIndex tet = this->getVertex(center).VTstar();
    queue<TetraIndex> queue;
    queue.push(tet);
    tetra.push_back(tet);
    visitedTetra[tet]=true;

    PositionIndex k=(PositionIndex)-1;

    while(!queue.empty())
    {
        k=(PositionIndex)-1;
        TetraIndex current = queue.front();

        for(PositionIndex pos = 0; pos <= 3; pos++)
        {
            if(this->getTetra(current).TV(pos) == center)
            {
                k = pos;
                break;
            }
        }

        for(PositionIndex pos = 1; pos <= 3; pos++)
        {
            Edge e = Edge(this->getTetra(current).TV(k),this->getTetra(current).TV(((PositionIndex)(k+pos)%4)));
            if(!isIntoVector(e,*edges))
                edges->push_back(e);

            TetraIndex adj = this->getTetra(current).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && !visitedTetra[adj])
            {
                visitedTetra[adj]=true;
                queue.push(adj);
                tetra.push_back(adj);
            }
        }
        queue.pop();
    }


    return edges;
}


std::vector<Triangle> Mesh :: VF(VertexIndex center)
{
    vector<Triangle> faces;
    vector<TetraIndex> tetra;
    vector<bool> visitedTetra(tetraNumber(),false);
    TetraIndex tet = this->getVertex(center).VTstar();
    queue<TetraIndex> queue;
    queue.push(tet);
    tetra.push_back(tet);
    visitedTetra[tet]=true;

    PositionIndex k=(PositionIndex)-1;

    while(!queue.empty())
    {
        k=(PositionIndex)-1;

        TetraIndex current = queue.front();

        for(PositionIndex pos = 0; pos <= 3; pos++)
        {
            if(this->getTetra(current).TV(pos) == center)
            {
                k = pos;
                break;
            }
        }

        for(PositionIndex pos = 0; pos <= 3; pos++)
        {
            Triangle faceIn = *(this->getTetra(current).TF((PositionIndex)((k+pos)%4)));
            if(!this->isIntoVector(faceIn,faces))
                faces.push_back(faceIn);

            TetraIndex adj = this->getTetra(current).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && !visitedTetra[adj])
            {
                visitedTetra[adj]=true;
                queue.push(adj);
                tetra.push_back(adj);
            }
        }
        queue.pop();
    }

    return faces;
}


std::vector<TetraIndex>* Mesh :: VT(VertexIndex center){
    set<TetraIndex> tetras;
    TetraIndex initial = getVertex(center).VTstar();

    queue<TetraIndex> coda;
    coda.push(initial);
    tetras.insert(initial);

    PositionIndex k;
    TetraIndex attuale;

    while(!coda.empty()){
        attuale = coda.front();

        k = getTetra(attuale).vertexIndex(center);

        for(PositionIndex pos = 1; pos <= 3; pos++)
        {
            TetraIndex adj = this->getTetra(attuale).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && tetras.find(adj) == tetras.end())
            {
                coda.push(adj);
                tetras.insert(adj);
            }
        }
        coda.pop();
    }

    return new vector<TetraIndex>(tetras.begin(), tetras.end());
}


std::vector<TetraIndex> Mesh :: VT(VertexIndex center,bool* foundBorder)
{
    *foundBorder = false;
    vector<TetraIndex> tetras;
    vector<bool> visitedTetra(tetraNumber(),false);
    TetraIndex tet = this->getVertex(center).VTstar();
    queue<TetraIndex> coda;
    coda.push(tet);
    tetras.push_back(tet);
    visitedTetra[tet]=true;

    PositionIndex k=(PositionIndex)-1;

    while(!coda.empty()){
        k=(PositionIndex)-1;
        TetraIndex attuale = coda.front();

        for(PositionIndex pos = 0; pos <= 3; pos++)
        {
            if(this->getTetra(attuale).TV(pos) == center)
            {
                k = pos;
                break;
            }
        }

        for(PositionIndex pos = 1; pos <= 3; pos++)
        {
            TetraIndex adj = this->getTetra(attuale).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && !visitedTetra[adj])
            {
                visitedTetra[adj]=true;
                coda.push(adj);
                tetras.push_back(adj);
            }
            else if(adj == noAdjacentTetraIndex && this->getTetra(tet).TV((PositionIndex)((k+pos)%4)) != center)
                *foundBorder = true;
        }
        coda.pop();
    }


    return tetras;
}


bool Mesh :: isBoundary(VertexIndex center)
{

    vector<TetraIndex> tetras;
    vector<bool> visitedTetra(tetraNumber(),false);
    TetraIndex tet = this->getVertex(center).VTstar();
    queue<TetraIndex> coda;
    coda.push(tet);
    tetras.push_back(tet);
    visitedTetra[tet]=true;

    PositionIndex k=(PositionIndex)-1;

    while(!coda.empty()){
        k=(PositionIndex)-1;
        TetraIndex attuale = coda.front();

        for(PositionIndex pos = 0; pos <= 3; pos++)
        {
            if(this->getTetra(attuale).TV(pos) == center)
            {
                k = pos;
                break;
            }
        }

        for(PositionIndex pos = 1; pos <= 3; pos++)
        {
            TetraIndex adj = this->getTetra(attuale).TT((PositionIndex)((k+pos)%4));
            if( adj != noAdjacentTetraIndex && !visitedTetra[adj])
            {
                visitedTetra[adj]=true;
                coda.push(adj);
                tetras.push_back(adj);
            }
            else if(adj == noAdjacentTetraIndex && this->getTetra(tet).TV((PositionIndex)((k+pos)%4)) != center){
                return true;
            }
        }
        coda.pop();
    }

    return false;
}


void Mesh :: merge(vector<pair<Triangle,TetraIndex>* >* faces, int left, int center, int right){

    int size=right-left;

    std::vector<pair<Triangle,TetraIndex>* > aux(size);
    int i,j;

    for(i=left,j=center; i<center&&j<right;){
        if(faces->at(i)->first>faces->at(j)->first){
            aux[i+j-center-left]=faces->at(j);
            j++;
        }
        else{
            aux[i+j-center-left]=faces->at(i);
            i++;
        }
    }

    for(;i<center;i++){
        aux[i+j-center-left]=faces->at(i);
    }
    for(;j<right;j++){
        aux[i+j-center-left]=faces->at(j);
    }

    for(int k=0; k<size; k++){
        faces->at(k+left)=aux[k];
    }
}

template<class C> bool Mesh :: isIntoVector(C c, vector<C> &c_vect)
{
    for(unsigned int i=0; i<c_vect.size();i++)
        if(c == c_vect.at(i))
            return true;
    return false;
}


