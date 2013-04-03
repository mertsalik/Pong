//
//  Model.cpp
//  Ders 6
//
//  Created by Gurel Erceis on 3/17/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//

#include "Model.h"

bool Model::loadGeometry(){
    Assimp::Importer Importer;
    const aiScene *pScene = NULL;
    const aiMesh *pMesh = NULL;
    // Assimp e dosyayi yuklemesini soyluyoruz.
    pScene = Importer.ReadFile(filename, aiProcess_Triangulate| aiProcess_ValidateDataStructure | aiProcess_FindInvalidData);
    
    if (!pScene)
    {
        printf("Error parsing '%s': '%s'\n", filename, Importer.GetErrorString());
        return false;
    }
    
    pMesh = pScene->mMeshes[0];
    if (!pMesh)
    {
        printf("Error Finding Model In file.  Did you export an empty scene?");
        return false;
    }
    
    readScene(pScene);
    return true;
}

GLuint Model::LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ifstream::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Compile Vertex Shader
    printf("Compiling Vertex Shaders\n");
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    if(!Result)
    {
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "VertexShaderErrorMessage: %s\n", &VertexShaderErrorMessage[0]);
    }
    
    // Compile Fragment Shader
    printf("Compiling Fragment Shader\n");
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    if(!Result)
    {
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "FragmentShaderErrorMessage: %s\n", &FragmentShaderErrorMessage[0]);
    }
    
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    if(!Result)
    {
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "ProgramErrorMessage: %s\n", &ProgramErrorMessage[0]);
    }
    //glDeleteShader(VertexShaderID);
    //glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

void Model::readScene(const struct aiScene *sc) {
    faceArray.clear();
    vertices.clear();
    normals.clear();
    
    // For each mesh
    for (unsigned int n = 0; n < /*sc->mNumMeshes*/1; ++n)
    {
        const struct aiMesh* mesh = sc->mMeshes[n];
        
        // create array with faces
        // have to convert from Assimp format to array
        
        unsigned int faceIndex = 0;
        
        for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
            const struct aiFace* face = &mesh->mFaces[t];
            
            faceArray.push_back(face->mIndices[0]);
            faceArray.push_back(face->mIndices[1]);
            faceArray.push_back(face->mIndices[2]);
            faceIndex += 3;
        }
        numFaces = mesh->mNumFaces;
        
        // buffer for vertex positions
        if (mesh->HasPositions()) {
            
            unsigned int vertexIndex = 0;
            
            for( unsigned int t = 0; t < mesh->mNumVertices; t++)
            {
                const aiVector3D* vertex = &mesh->mVertices[t];
                vertices.push_back(glm::vec3(vertex->x,vertex->y,vertex->z));
                vertexIndex += 3;
            }
        } 
        
        // buffer for normals
        if (mesh->HasNormals()) {
            unsigned int vertexIndex = 0;
            
            for( unsigned int t = 0; t < mesh->mNumVertices; t++)
            {
                const aiVector3D* vertex = &mesh->mNormals[t];
                normals.push_back(glm::vec3(vertex->x,vertex->y,vertex->z));
                vertexIndex += 3;
            }
        }else{
            normals.resize(mesh->mNumVertices);
            // calculate normals if no normal is found for mesh
            for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
                const struct aiFace* face = &mesh->mFaces[t];
                
                aiVector3D p1 = mesh->mVertices[face->mIndices[0]];
                aiVector3D p2 = mesh->mVertices[face->mIndices[1]];
                aiVector3D p3 = mesh->mVertices[face->mIndices[2]];
                
                glm::vec3 a = glm::vec3(p2.x-p1.x,p2.y-p1.y,p2.z-p1.z);
                glm::vec3 b = glm::vec3(p3.x-p1.x,p3.y-p1.y,p3.z-p1.z);
                
                glm::vec3 n = glm::cross(a,b);
                glm::normalize(n);
 
                normals[face->mIndices[0]] = n;
                normals[face->mIndices[1]] = n;
                normals[face->mIndices[2]] = n;
            }
        }
    }
}