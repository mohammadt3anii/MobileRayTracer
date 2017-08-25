//
// Created by Tiago on 22/08/17.
//

#include "KdTree.hpp"

using MobileRT::MManager;
using MobileRT::KdTree;
using MobileRT::KdTreeNode;
using MobileRT::ObjectList;
using MobileRT::AABB;
using MobileRT::Primitive;
#define MAXTREEDEPTH  20
MManager *KdTree::s_MManager = 0;

MManager::MManager () : m_OList (0) {
  // build a 32-byte aligned array of KdTreeNodes
  m_KdArray = (char *) (new KdTreeNode[1000000]);
  m_ObjArray = (char *) (new ObjectList[100000]);
  unsigned long addr = (unsigned long) m_KdArray;
  m_KdPtr = (KdTreeNode *) ((addr + 32) & (0xffffffff - 31));
  addr = (unsigned long) m_ObjArray;
  m_ObjPtr = (ObjectList *) ((addr + 32) & (0xffffffff - 31));
  ObjectList *ptr = m_ObjPtr;
  for (int i = 0; i < 99995; i++) {
    ptr->SetNext (ptr + 1);
    ptr++;
  }
  ptr->SetNext (0);
  m_OList = m_ObjPtr;
}

void KdTreeNode::Add (Primitive<Triangle> *a_Prim) {
  ObjectList *lnode = KdTree::s_MManager->NewObjectList ();
  lnode->SetPrimitive (a_Prim);
  lnode->SetNext (GetList ());
  SetList (lnode);
}

ObjectList *MManager::NewObjectList () {
  ObjectList *retval;
  retval = m_OList;
  m_OList = m_OList->GetNext ();
  retval->SetNext (0);
  retval->SetPrimitive (0);
  return retval;
}

void MManager::FreeObjectList (ObjectList *a_List) {
  ObjectList *list = a_List;
  while (list->GetNext ()) list = list->GetNext ();
  list->SetNext (m_OList);
  m_OList = a_List;
}

KdTreeNode *MManager::NewKdTreeNodePair () {
  unsigned long *tmp = (unsigned long *) m_KdPtr;
  tmp[1] = tmp[3] = 6;
  KdTreeNode *node = m_KdPtr;
  m_KdPtr += 2;
  return node;
}

KdTree::KdTree (AABB sceneBounds, Scene *scene) noexcept :
  m_Root {new KdTreeNode ()} {
  KdTree::SetMemoryManager (new MManager ());
  Build (scene, sceneBounds);
}

void KdTree::Build (Scene *a_Scene, AABB sceneBounds) {
  for (int p = 0; p < a_Scene->triangles_.size (); p++)
    m_Root->Add (&a_Scene->triangles_[static_cast<size_t>(p)]);
  int prims = static_cast <int> (a_Scene->triangles_.size ());
  AABB sbox = sceneBounds;
  m_SPool = new SplitList[prims * 2 + 8];
  int i;
  for (i = 0; i < (prims * 2 + 6); i++) m_SPool[i].next = &m_SPool[i + 1];
  m_SPool[i].next = 0;
  m_SList = 0;
  Subdivide (m_Root, sbox, 0, prims);
}

void KdTree::InsertSplitPos (float a_SplitPos) {
  // insert a split position candidate in the list if unique
  SplitList *entry = m_SPool;
  m_SPool = m_SPool->next;
  entry->next = 0;
  entry->splitpos = a_SplitPos;
  entry->n1count = 0;
  entry->n2count = 0;
  if (!m_SList) m_SList = entry;
  else {
    if (a_SplitPos < m_SList->splitpos) {
      entry->next = m_SList;
      m_SList = entry;
    } else if (a_SplitPos == m_SList->splitpos) {
      entry->next = m_SPool; // redundant; recycle
      m_SPool = entry;
    } else {
      SplitList *list = m_SList;
      while ((list->next) && (a_SplitPos >= list->next->splitpos)) {
        if (a_SplitPos == list->next->splitpos) {
          entry->next = m_SPool; // redundant; recycle
          m_SPool = entry;
          return;
        }
        list = list->next;
      }
      entry->next = list->next;
      list->next = entry;
    }
  }
}

void KdTree::Subdivide (KdTreeNode *a_Node, AABB &a_Box, int a_Depth, int a_Prims) {
  // recycle used split list nodes
  if (m_SList) {
    SplitList *list = m_SList;
    while (list->next) list = list->next;
    list->next = m_SPool;
    m_SPool = m_SList;
    m_SList = 0;
  }
  // determine split axis
  Vector3D s = a_Box.pointMax_ - a_Box.pointMin_;
  if ((s.x_ >= s.y_) && (s.x_ >= s.z_)) a_Node->SetAxis (0);
  else if ((s.y_ >= s.x_) && (s.y_ >= s.z_)) a_Node->SetAxis (1);
  int axis = a_Node->GetAxis ();
  // make a list of the split position candidates
  ObjectList *l = a_Node->GetList ();
  float p1;
  float pos1 = a_Box.pointMin_.cell[axis];
  float pos2 = a_Box.pointMin_.cell[axis] + a_Box.pointMax_.cell[axis];
  bool *pright = new bool[a_Prims];
  float *eleft = new float[a_Prims], *eright = new float[a_Prims];
  Primitive<Triangle> **parray = new Primitive<Triangle> *[a_Prims];
  int aidx = 0;
  while (l) {
    Primitive<Triangle> *p = parray[aidx] = l->GetPrimitive ();
    pright[aidx] = true;
    p->shape_.CalculateRange (eleft[aidx], eright[aidx], axis);
    aidx++;
    /*float p2;
    if (p->GetType() == Primitive::SPHERE)
    {
      p1 = p->GetCentre().cell[axis] - p->GetRadius();
      p2 = p->GetCentre().cell[axis] + p->GetRadius();
      if ((p1 >= pos1) && (p1 <= pos2)) InsertSplitPos( p1 );
      if ((p2 >= pos1) && (p2 <= pos2)) InsertSplitPos( p2 );
    }
    else*/
    {
      p1 = p->shape_.pointA_.cell[axis];
      if ((p1 >= pos1) && (p1 <= pos2)) InsertSplitPos (p1);
      p1 = p->shape_.pointB_.cell[axis];
      if ((p1 >= pos1) && (p1 <= pos2)) InsertSplitPos (p1);
      p1 = p->shape_.pointC_.cell[axis];
      if ((p1 >= pos1) && (p1 <= pos2)) InsertSplitPos (p1);
    }
    l = l->GetNext ();
  }
  // determine n1count / n2count for each split position
  AABB b1, b2, b3 = a_Box, b4 = a_Box;
  SplitList *splist = m_SList;
  float b3p1 = b3.pointMin_.cell[axis];
  float b4p2 = b4.pointMin_.cell[axis] + b4.pointMax_.cell[axis];
  while (splist) {
    b4.pointMin_.cell[axis] = splist->splitpos;
    b4.pointMax_.cell[axis] = pos2 - splist->splitpos;
    b3.pointMax_.cell[axis] = splist->splitpos - pos1;
    float b3p2 = b3.pointMin_.cell[axis] + b3.pointMax_.cell[axis];
    float b4p1 = b4.pointMin_.cell[axis];
    for (int i = 0; i < a_Prims; i++)
      if (pright[i]) {
        Primitive<Triangle> *p = parray[i];
        if ((eleft[i] <= b3p2) && (eright[i] >= b3p1)) {
          if (p->intersect (b3)) splist->n1count++;
        }
        if ((eleft[i] <= b4p2) && (eright[i] >= b4p1)) {
          if (p->intersect (b4)) {
            splist->n2count++;
          } else {
            pright[i] = false;
          }
        }
      } else {
        splist->n1count++;
      }
    splist = splist->next;
  }
  delete[] pright;
  // calculate surface area for current node
  float SAV =
    0.5f / (a_Box.pointMax_ - a_Box.pointMin_).x_ * (a_Box.pointMax_ - a_Box.pointMin_).z_ +
    (a_Box.pointMax_ - a_Box.pointMin_).x_ * (a_Box.pointMax_ - a_Box.pointMin_).y_ +
    (a_Box.pointMax_ - a_Box.pointMin_).z_ * (a_Box.pointMax_ - a_Box.pointMin_).y_;
  // calculate cost for not splitting
  float Cleaf = a_Prims * 1.0f;
  // determine optimal split plane position
  splist = m_SList;
  float lowcost = 10000;
  float bestpos = 0;
  while (splist) {
    // calculate child node extends
    b4.pointMin_.cell[axis] = splist->splitpos;
    b4.pointMax_.cell[axis] = pos2 - splist->splitpos;
    b3.pointMax_.cell[axis] = splist->splitpos - pos1;
    // calculate child node cost
    float SA1 = 2 * ((b3.pointMax_ - b3.pointMin_).x_ * (b3.pointMax_ - b3.pointMin_).z_ +
                     (b3.pointMax_ - b3.pointMin_).x_ * (b3.pointMax_ - b3.pointMin_).y_ +
                     (b3.pointMax_ - b3.pointMin_).z_ * (b3.pointMax_ - b3.pointMin_).y_);
    float SA2 = 2 * (b4.pointMax_ - b4.pointMin_).x_ * (b4.pointMax_ - b4.pointMin_).z_ +
                (b4.pointMax_ - b4.pointMin_).x_ * (b4.pointMax_ - b4.pointMin_).y_ +
                (b4.pointMax_ - b4.pointMin_).z_ * (b4.pointMax_ - b4.pointMin_).y_;
    float splitcost = 0.3f + 1.0f * (SA1 * SAV * splist->n1count + SA2 * SAV * splist->n2count);
    // update best cost tracking variables
    if (splitcost < lowcost) {
      lowcost = splitcost;
      bestpos = splist->splitpos;
      b1 = b3;
      b2 = b4;
    }
    splist = splist->next;
  }
  if (lowcost > Cleaf) return;
  a_Node->SetSplitPos (bestpos);
  // construct child nodes
  KdTreeNode *left = s_MManager->NewKdTreeNodePair ();
  int n1count = 0, n2count = 0, total = 0;
  // assign primitives to both sides
  float b1p1 = b1.pointMin_.cell[axis];
  float b2p2 = b2.pointMin_.cell[axis] + b2.pointMax_.cell[axis];
  float b1p2 = b1.pointMin_.cell[axis] + b1.pointMax_.cell[axis];
  float b2p1 = b2.pointMin_.cell[axis];
  for (int i = 0; i < a_Prims; i++) {
    Primitive<Triangle> *p = parray[i];
    total++;
    if ((eleft[i] <= b1p2) && (eright[i] >= b1p1))
      if (p->intersect (b1)) {
        left->Add (p);
        n1count++;
      }
    if ((eleft[i] <= b2p2) && (eright[i] >= b2p1))
      if (p->intersect (b2)) {
        (left + 1)->Add (p);
        n2count++;
      }
  }
  delete[] eleft;
  delete[] eright;
  delete[] parray;
  s_MManager->FreeObjectList (a_Node->GetList ());
  a_Node->SetLeft (left);
  a_Node->SetLeaf (false);
  if (a_Depth < MAXTREEDEPTH) {
    if (n1count > 2) Subdivide (left, b1, a_Depth + 1, n1count);
    if (n2count > 2) Subdivide (left + 1, b2, a_Depth + 1, n2count);
  }
}

bool KdTree::trace (Intersection * /*intersection*/, const Ray & /*ray*/) const noexcept {
  return false;
}

bool KdTree::shadowTrace (Intersection * /*intersection*/, Ray && /*ray*/) const noexcept {
  return false;
}
