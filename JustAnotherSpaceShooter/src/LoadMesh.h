#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include "assimp/Scene.h"

struct SubmeshData
{
   unsigned int mNumIndices;
   unsigned int mBaseIndex;
   unsigned int mBaseVertex;

   SubmeshData() : mNumIndices(0), mBaseIndex(0), mBaseVertex(0) {}
   void DrawSubmesh();
};

struct MeshData
{
   unsigned int mVao;
   unsigned int mVboVerts;
   unsigned int mVboNormals;
   unsigned int mVboTexCoords;
   unsigned int mIndexBuffer;
   float mScaleFactor; //TODO replace with bounding box

   const aiScene* mScene;
   aiVector3D mBbMin, mBbMax;

   std::vector<SubmeshData> mSubmesh;
   std::string mFilename;

   MeshData() : mVao(-1), mVboVerts(-1), mVboNormals(-1), mVboTexCoords(-1), mIndexBuffer(-1), mScaleFactor(0.0f), mScene(NULL) {}

   void DrawMesh();

};

MeshData LoadMesh(const std::string& pFile);