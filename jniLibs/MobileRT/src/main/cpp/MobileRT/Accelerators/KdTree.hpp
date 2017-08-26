//
// Created by Tiago on 22/08/17.
//

#ifndef MOBILERT_ACCELERATORS_KDTREE_HPP
#define MOBILERT_ACCELERATORS_KDTREE_HPP

#include "../Scene.hpp"

namespace MobileRT {
  class KdTreeNode;

  struct kdstack {
    KdTreeNode *node;
    float t;
    Point3D pb;
    int prev;
  };

  struct SplitList final {
    float splitpos;
    int n1count, n2count;
    SplitList *next;
  };

  class ObjectList {
    public:
    ObjectList () : m_Primitive (nullptr), m_Next (nullptr) {}

    ~ObjectList () { delete m_Next; }

    void SetPrimitive (Primitive<Triangle> *a_Prim) { m_Primitive = a_Prim; }

    Primitive<Triangle> *GetPrimitive () { return m_Primitive; }

    void SetNext (ObjectList *a_Next) { m_Next = a_Next; }

    ObjectList *GetNext () { return m_Next; }

    private:
    Primitive<Triangle> *m_Primitive;
    ObjectList *m_Next;
  };

  class KdTreeNode final {
    public:
    KdTreeNode () : m_Data {nullptr} {}

    void SetAxis (int a_Axis) { m_Axis = a_Axis; }

    int GetAxis () { return m_Axis; }

    void SetSplitPos (float a_Pos) { m_Split = a_Pos; }

    float GetSplitPos () { return m_Split; }

    void SetLeft (KdTreeNode *a_Left) { m_left = a_Left; }

    KdTreeNode *GetLeft () { return m_left; }

    KdTreeNode *GetRight () { return m_right; }

    void Add (Primitive<Triangle> *a_Prim);

    bool IsLeaf () { return m_isLeaf; }

    void SetLeaf (bool a_Leaf) { m_isLeaf = a_Leaf; }

    ObjectList *GetList () { return m_Data; }

    void SetList (ObjectList *a_List) { m_Data = a_List; }

    private:
    int m_Axis;
    float m_Split;
    bool m_isLeaf;
    KdTreeNode *m_left;
    KdTreeNode *m_right;
    ObjectList *m_Data;
  };

  class MManager {
    public:
    MManager ();
    void FreeObjectList (ObjectList *a_List);
    KdTreeNode *NewKdTreeNodePair ();
    public:
    ObjectList *m_OList;
  };

  class KdTree final {
    public:
    void Build (Scene *a_Scene, AABB sceneBounds);

    KdTreeNode *GetRoot () { return m_Root; }

    // tree generation
    void InsertSplitPos (float a_SplitPos);
    void Subdivide (KdTreeNode *a_Node, AABB &a_Box, int a_Depth, int a_Prims);

    // memory manager
    void SetMemoryManager (MManager *a_MM) { m_MManager = a_MM; }

    private:
    KdTreeNode *m_Root;
    SplitList *m_SList, *m_SPool;
    AABB sceneBounds_;
    kdstack *m_Stack;
    int *m_Mod;
    int m_Intersections;
    private:
    MManager *m_MManager {nullptr};

    public:
    explicit KdTree () noexcept = default;
    explicit KdTree (AABB sceneBounds, Scene *scene) noexcept;
    explicit KdTree (const KdTree &kDTree) noexcept = delete;
    explicit KdTree (KdTree &&kDTree) noexcept = delete;
    ~KdTree () noexcept = default;
    KdTree &operator= (const KdTree &kDTree) noexcept = delete;
    KdTree &operator= (KdTree &&kDTree) noexcept = default;

    public:
    bool trace (Intersection *intersection, const Ray &ray) noexcept;
    bool shadowTrace (Intersection *intersection, Ray &&ray) const noexcept;
  };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_KDTREE_HPP
