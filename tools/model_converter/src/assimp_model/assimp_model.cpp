#include "assimp_model.hpp"
#include "../assimp_animation/from_assimp.hpp"
#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/cimport.h>
#include <assimp/color4.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>
#include <set>

namespace sinen {
Animation createAnimation(const aiAnimation *anim);
std::shared_ptr<Node> createNode(const aiNode *const n, aiMesh **mesh);
Mesh createMesh(const aiMesh *const m);
Bone createBone(const aiBone *b);
void createNodeInfo(std::shared_ptr<Node> node,
                    std::map<std::string, std::shared_ptr<Node>> &node_index,
                    std::vector<std::shared_ptr<Node>> &node_list);
void UpdateNodeMatrix(assimp_model *model, const float time,
                      const Animation &animation);
void UpdateNodeDerivedMatrix(const std::shared_ptr<Node> &node,
                             const matrix4 &parent_matrix);
void UpdateMesh(assimp_model *model);
void normalizeMeshWeight(assimp_model &model) {
  for (const auto &node : model.node_list) {
    for (auto &mesh : node->mesh) {
      if (!mesh.has_bone)
        continue;
      std::set<uint32_t> weight_index;
      std::multimap<uint32_t, Weight *> weight_values;
      for (auto &bone : mesh.bones) {
        for (auto &weight : bone.weights) {
          weight_index.insert(weight.vertex_id);
          weight_values.emplace(weight.vertex_id, &weight);
        }
      }
      // Normalize weight
      for (const auto i : weight_index) {
        const auto p = weight_values.equal_range(i);
        float weight = 0.0f;
        for (auto it = p.first; it != p.second; ++it) {
          weight += it->second->value;
        }

        {
          float n = 1.0f / weight;
          for (auto it = p.first; it != p.second; ++it) {
            it->second->value *= n;
          }
        }
      }
    }
  }
}
void resetMesh(assimp_model &model) {
  for (const auto &node : model.node_list) {
    for (auto &mesh : node->mesh) {
      if (!mesh.has_bone)
        continue;

      mesh.body = mesh.original;
    }
  }
}
void resetModelNodes(assimp_model &model) {
  for (const auto &node : model.node_list) {
    node->matrix = node->matrix_orig;
  }

  resetMesh(model);
}

bool assimp_model::load_from_file(std::string_view filePath) {
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      std::string(filePath),
      aiProcess_Triangulate | aiProcess_FlipUVs |
          aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes |
          aiProcess_LimitBoneWeights | aiProcess_RemoveRedundantMaterials);
  if (!scene) {
    std::cerr << importer.GetErrorString() << std::endl;
    return false;
  }

  this->node = createNode(scene->mRootNode, scene->mMeshes);
  // Create node index
  createNodeInfo(this->node, this->node_index, this->node_list);
  has_anim = scene->HasAnimations();
  if (has_anim) {
    aiAnimation **anim = scene->mAnimations;
    for (uint32_t i = 0; i < scene->mNumAnimations; ++i) {
      animation.push_back(createAnimation(anim[i]));
    }
  }
  return true;
}
NodeAnim createNodeAnim(const aiNodeAnim *anim) {
  NodeAnim animation;
  animation.node_name = anim->mNodeName.C_Str();
  // Translation
  for (uint32_t i = 0; i < anim->mNumPositionKeys; ++i) {
    animation.translate.push_back(fromAssimp(anim->mPositionKeys[i]));
  }
  // Scaling
  for (uint32_t i = 0; i < anim->mNumScalingKeys; ++i) {
    animation.scaling.push_back(fromAssimp(anim->mScalingKeys[i]));
  }
  // Rotation
  for (uint32_t i = 0; i < anim->mNumRotationKeys; ++i) {
    animation.rotation.push_back(fromAssimp(anim->mRotationKeys[i]));
  }
  return animation;
}
Animation createAnimation(const aiAnimation *anim) {
  Animation animation;

  animation.mDuration = anim->mDuration;

  // Node animation
  aiNodeAnim **node_anim = anim->mChannels;
  for (uint32_t i = 0; i < anim->mNumChannels; ++i) {
    animation.body.push_back(createNodeAnim(node_anim[i]));
  }

  return animation;
}
std::shared_ptr<Node> createNode(const aiNode *const n, aiMesh **mesh) {
  auto node = std::make_shared<Node>();

  node->name = n->mName.C_Str();

  for (uint32_t i = 0; i < n->mNumMeshes; i++) {
    node->mesh.push_back(createMesh(mesh[n->mMeshes[i]]));
  }
  memcpy(&node->matrix, &n->mTransformation, sizeof(float) * 16);
  node->matrix = matrix4::transpose(node->matrix);
  // Save original matrix
  node->matrix_orig = node->matrix;

  for (uint32_t i = 0; i < n->mNumChildren; ++i) {
    node->children.push_back(createNode(n->mChildren[i], mesh));
  }

  return node;
}
Bone createBone(const aiBone *b) {
  Bone bone;

  bone.name = b->mName.C_Str();
  memcpy(&bone.offset, &b->mOffsetMatrix, sizeof(float) * 16);
  bone.offset = matrix4::transpose(bone.offset);

  const aiVertexWeight *w = b->mWeights;
  for (uint32_t i = 0; i < b->mNumWeights; i++) {
    Weight weight;
    weight.vertex_id = w[i].mVertexId;
    weight.value = w[i].mWeight;
    bone.weights.push_back(weight);
  }

  return bone;
}

Mesh createMesh(const aiMesh *const m) {
  Mesh mesh;
  // Vertex
  uint32_t num_vtx = m->mNumVertices;
  const aiVector3D *vtx = m->mVertices;
  std::vector<vertex> v;
  v.resize(num_vtx);

  if (m->HasPositions()) {
    for (uint32_t h = 0; h < num_vtx; ++h) {
      v[h].position = fromAssimp(vtx[h]);
    }
  }
  if (m->HasNormals()) {
    const aiVector3D *normal = m->mNormals;
    for (uint32_t h = 0; h < num_vtx; ++h) {
      v[h].normal = fromAssimp(normal[h]);
    }
  }
  if (m->HasTextureCoords(0)) {
    const aiVector3D *uv = m->mTextureCoords[0];
    for (uint32_t h = 0; h < num_vtx; ++h) {
      v[h].uv = vector2(uv[h].x, uv[h].y);
    }
  }
  if (m->HasVertexColors(0)) {
    const aiColor4D *color = m->mColors[0];
    for (uint32_t h = 0; h < num_vtx; ++h) {
      v[h].rgba = sinen::color{color[h].r, color[h].g, color[h].b, color[h].a};
    }
  }
  // Setting vertex
  vertex_array va;
  va.vertices = v;
  for (uint32_t i = 0; i < m->mNumFaces; i++) {
    for (uint32_t j = 0; j < m->mFaces[i].mNumIndices; j++) {
      va.indices.push_back(m->mFaces[i].mIndices[j]);
    }
  }
  va.indexCount = va.indices.size();
  mesh.body = va;
  va.materialName = m->mMaterialIndex;
  // Bone info
  mesh.has_bone = m->HasBones();
  if (mesh.has_bone) {
    aiBone **b = m->mBones;
    for (uint32_t i = 0; i < m->mNumBones; ++i) {
      mesh.bones.push_back(createBone(b[i]));
    }
    // Save original
    mesh.original = mesh.body;
  }
  return mesh;
}
void createNodeInfo(std::shared_ptr<Node> node,
                    std::map<std::string, std::shared_ptr<Node>> &node_index,
                    std::vector<std::shared_ptr<Node>> &node_list) {

  node_index.insert(std::make_pair(node->name, node));
  node_list.push_back(node);

  for (auto child : node->children) {
    createNodeInfo(child, node_index, node_list);
  }
}

void assimp_model::UpdateAnimation(const double time, const size_t index) {
  if (!this->has_anim)
    return;
  float current_time = std::fmod(time, this->animation[index].mDuration);
  UpdateNodeMatrix(this, current_time, this->animation[index]);
  UpdateNodeDerivedMatrix(this->node, matrix4());
  UpdateMesh(this);
}
template <typename T> struct Comp {
  bool operator()(const double lhs, const T &rhs) { return lhs < rhs.time; }
};
quaternion getLerpValue(const float time, const std::vector<QuatKey> &values) {
  auto result = std::upper_bound(values.begin(), values.end(), (double)time,
                                 Comp<QuatKey>());

  quaternion value;
  if (result == values.begin()) {
    value = result->value;
  } else if (result == values.end()) {
    value = (result - 1)->value;
  } else {
    double dt = result->time - (result - 1)->time;
    double t = time - (result - 1)->time;
    value =
        quaternion::slerp((result - 1)->value, result->value, (float)(t / dt));
  }
  return value;
}
vector3 getLerpValue(const float time, const std::vector<VectorKey> &values) {
  auto result =
      std::upper_bound(values.begin(), values.end(), time, Comp<VectorKey>());
  vector3 value;
  if (result == values.begin()) {
    value = result->value;
  } else if (result == values.end()) {
    value = (result - 1)->value;
  } else {
    double dt = result->time - (result - 1)->time;
    double t = time - (result - 1)->time;
    value = vector3::lerp((result - 1)->value, result->value, float(t / dt));
  }

  return value;
}
void UpdateNodeMatrix(assimp_model *model, const float time,
                      const Animation &anim) {
  for (const auto &body : anim.body) {
    vector3 translate = getLerpValue(time, body.translate);
    matrix4 t = matrix4::identity;
    t[3][0] = translate.x;
    t[3][1] = translate.y;
    t[3][2] = translate.z;
    t[3][3] = 1.f;
    matrix4 r =
        matrix4::create_from_quaternion(getLerpValue(time, body.rotation));

    vector3 scaling = getLerpValue(time, body.scaling);
    matrix4 s;
    s[0][0] = scaling.x;
    s[1][1] = scaling.y;
    s[2][2] = scaling.z;
    s[3][3] = 1.f;
    matrix4 matrix = t * r * s;
    auto node = model->node_index.at(body.node_name);
    node->matrix = matrix;
  }
}
void UpdateNodeDerivedMatrix(const std::shared_ptr<Node> &n,
                             const matrix4 &parent_matrix) {
  n->global_matrix = parent_matrix * n->matrix;
  n->invert_matrix = n->global_matrix;
  n->invert_matrix.invert();

  for (auto child : n->children) {
    UpdateNodeDerivedMatrix(child, n->global_matrix);
  }
}
void UpdateMesh(assimp_model *model) {
  for (const auto &node : model->node_list) {
    for (auto &mesh : node->mesh) {
      if (!mesh.has_bone)
        continue;
      std::vector<matrix4> bone_matrix;
      bone_matrix.reserve(mesh.bones.size());
      for (auto &bone : mesh.bones) {
        auto local_node = model->node_index.at(bone.name);
        bone_matrix.push_back(local_node->global_matrix * bone.offset);
      }
      auto &body_vtx = mesh.body.vertices;
      for (auto &i : body_vtx) {
        i.position = vector3::zero;
      }
      const auto &orig_vtx = mesh.original.vertices;
      for (uint32_t i = 0; i < mesh.bones.size(); ++i) {
        const auto &bone = mesh.bones[i];
        const auto &m = bone_matrix[i];
        for (const auto &weight : bone.weights) {
          body_vtx[weight.vertex_id].position +=
              weight.value * (m * orig_vtx[weight.vertex_id].position);
        }
      }
    }
  }
}
} // namespace sinen
