#include "Mesh.h"


void Mesh::writeTS(char* file_name){

    FILE* file;
    file = fopen(file_name, "w");

    fprintf(file,"%d %d\n", vertexNumber(), tetraNumber());

    for(int i=0; i<vertexNumber(); i++)
        fprintf(file, "%f %f %f %f\n", getVertex(i).x(), getVertex(i).y(), getVertex(i).z(), getVertex(i).fieldValue());


    for(int i=0; i<tetraNumber(); i++)
        fprintf(file, "%d %d %d %d\n", getTetra(i).TV(0), getTetra(i).TV(1), getTetra(i).TV(2), getTetra(i).TV(3));

    fclose(file);
}


void Mesh::writeTS(char* file_name, vector<float> fieldvalues){


    FILE* file;
    file = fopen(file_name, "w");

    fprintf(file,"%d %d\n", vertexNumber(), tetraNumber());

    for(int i=0; i<vertexNumber(); i++)
        fprintf(file, "%f %f %f %f\n", getVertex(i).x(), getVertex(i).y(), getVertex(i).z(), fieldvalues[i]);


    for(int i=0; i<tetraNumber(); i++)
        fprintf(file, "%d %d %d %d\n", getTetra(i).TV(0), getTetra(i).TV(1), getTetra(i).TV(2), getTetra(i).TV(3));

    fclose(file);
}



void Mesh::writeField(char* file_name, vector<float> fieldvalues){

    FILE* file;
    file = fopen(file_name, "w");

    fprintf(file, "%d\n", (int)fieldvalues.size());

    for(int i=0; i<fieldvalues.size(); i++)
        fprintf(file, "%f\n", fieldvalues[i]);

    fclose(file);
}



void Mesh::writeVTK(char* file_name, vector<VertexIndex>* segmentation_desc){

    FILE* file;
    file = fopen(file_name, "w");

    fprintf(file, "# vtk DataFile Version 2.0\n\n");
    fprintf(file, "ASCII \n");
    fprintf(file, "DATASET UNSTRUCTURED_GRID\n\n");
    fprintf(file, "POINTS %d float\n", vertexNumber());

    for(int i=0; i<vertexNumber(); i++)
        fprintf(file, "%f %f %f\n", getVertex(i).x(), getVertex(i).y(), getVertex(i).z());
    fprintf(file, "\n\n");

    fprintf(file, "CELLS %d %d\n", tetraNumber(), tetraNumber()*5);

    for(int i=0; i<tetraNumber(); i++)
        fprintf(file, "4 %d %d %d %d\n", getTetra(i).TV(0), getTetra(i).TV(1), getTetra(i).TV(2), getTetra(i).TV(3));
    fprintf(file, "\n");

    fprintf(file, "CELL_TYPES %d\n", tetraNumber());

    for(int i=0; i<tetraNumber(); i++)
        fprintf(file, "%d ", 10);
    fprintf(file, "\n\n");


    fprintf(file, "POINT_DATA %d \n", vertexNumber());
    fprintf(file, "FIELD FieldData 1\n");
    fprintf(file, "original_field 1 %d float\n", vertexNumber());

    for(int i=0; i<vertexNumber(); i++)
        fprintf(file, "%f ", getVertex(i).fieldValue());

    fprintf(file, "\n\n");


    if(segmentation_desc != NULL){
        fprintf(file, "CELL_DATA %d \n", tetraNumber());
        fprintf(file, "FIELD FieldData 1\n");
        fprintf(file, "descending_3_cells 1 %d float\n", tetraNumber());

        for(int i=0; i<tetraNumber(); i++){
            fprintf(file, "%d ", (*segmentation_desc)[i]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}




vector<float> Mesh::readField(char* file_name){

    vector<float> fieldvalues;
    FILE* file;
    file = fopen(file_name, "r");

    int num_values;
    fscanf(file, "%d", &num_values);
    float val;
    for(int i=0; i<num_values; i++){
        fscanf(file, "%f", &val);
        fieldvalues.push_back(val);
    }

    fclose(file);

    return fieldvalues;
}


