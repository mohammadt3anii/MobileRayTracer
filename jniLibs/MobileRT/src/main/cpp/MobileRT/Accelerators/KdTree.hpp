//
// Created by Tiago on 22/08/17.
//

#ifndef MOBILERT_ACCELERATORS_KDTREE_HPP
#define MOBILERT_ACCELERATORS_KDTREE_HPP

#include "../Scene.hpp"

namespace MobileRT {
  struct SplitList final {
    float splitpos;
    int n1count, n2count;
    SplitList *next;
  };

  class ObjectList {
    public:
    ObjectList () : m_Primitive (0), m_Next (0) {}

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
    KdTreeNode () : m_Data (6) {}

    void SetAxis (int a_Axis) {
      m_Data = (m_Data & 0xfffffffc) + static_cast<unsigned long> (a_Axis);
    }

    int GetAxis () { return static_cast<int> (m_Data) & 3; }

    void SetSplitPos (float a_Pos) { m_Split = a_Pos; }

    float GetSplitPos () { return m_Split; }

    void SetLeft (KdTreeNode *a_Left) { m_Data = (unsigned long) a_Left + (m_Data & 7); }

    KdTreeNode *GetLeft () { return (KdTreeNode *) (m_Data & 0xfffffff8); }

    KdTreeNode *GetRight () { return ((KdTreeNode *) (m_Data & 0xfffffff8)) + 1; }

    void Add (Primitive<Triangle> *a_Prim);

    bool IsLeaf () { return ((m_Data & 4) > 0); }

    void SetLeaf (bool a_Leaf) { m_Data = (a_Leaf) ? (m_Data | 4) : (m_Data & 0xfffffffb); }

    ObjectList *GetList () { return (ObjectList *) (m_Data & 0xfffffff8); }

    void SetList (ObjectList *a_List) { m_Data = (unsigned long) a_List + (m_Data & 7); }

    private:
    // int m_Flags;
    float m_Split;
    unsigned long m_Data;
  };

  class MManager {
    public:
    MManager ();
    ObjectList *NewObjectList ();
    void FreeObjectList (ObjectList *a_List);
    KdTreeNode *NewKdTreeNodePair ();
    private:
    ObjectList *m_OList;
    char *m_KdArray, *m_ObjArray;
    KdTreeNode *m_KdPtr;
    ObjectList *m_ObjPtr;
  };

  class KdTree final {
    public:
    void Build (Scene *a_Scene, AABB sceneBounds);

    KdTreeNode *GetRoot () { return m_Root; }

    void SetRoot (KdTreeNode *a_Root) { m_Root = a_Root; }

    // tree generation
    void InsertSplitPos (float a_SplitPos);
    void Subdivide (KdTreeNode *a_Node, AABB &a_Box, int a_Depth, int a_Prims);

    // memory manager
    static void SetMemoryManager (MManager *a_MM) { s_MManager = a_MM; }

    private:
    KdTreeNode *m_Root;
    SplitList *m_SList, *m_SPool;
    public:
    static MManager *s_MManager;
    public:
    explicit KdTree () noexcept = default;
    explicit KdTree (AABB sceneBounds, Scene *scene) noexcept;
    explicit KdTree (const KdTree &kDTree) noexcept = delete;
    explicit KdTree (KdTree &&kDTree) noexcept = delete;
    ~KdTree () noexcept = default;
    KdTree &operator= (const KdTree &kDTree) noexcept = delete;
    KdTree &operator= (KdTree &&kDTree) noexcept = default;
    public:
    bool trace (Intersection *intersection, const Ray &ray) const noexcept;
    bool shadowTrace (Intersection *intersection, Ray &&ray) const noexcept;
  };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_KDTREE_HPP
