#include <iostream>
#include <vector>

using namespace std;
 
#define INF 0x3f3f3f3f
 
template< typename K, typename V >
class Treap {
private:
  struct Node {
    K key;
    V data;
    int prior, sz;
    Node *l, *r;
    //augmentation
    pair<V, K> mn, mx;
    Node() { }
    Node (K _key, V _data) {
      key = _key;
      data = _data;
      mn = mx = make_pair(data, key);
      prior = (rand() << 16) ^ (rand());
      sz = 1;
      l = r = NULL;
    }
  };
  typedef Node * pNode;
  int getSize(pNode p) {
    return p ? p->sz : 0;
  }
  pair<V, K> getMinimumVK(pNode p) {
    return p ? p->mn : make_pair(numeric_limits<V>::max(),numeric_limits<K>::max());
  }
  pair<V, K> getMaximumVK(pNode p) {
    return p ? p->mx : make_pair(numeric_limits<V>::min(),numeric_limits<K>::min());
  }
  void update(pNode p) {
    if(p) {
      p->sz = 1 + getSize(p->l) + getSize(p->r);
      p->mn = min(make_pair(p->data, p->key), min(getMinimumVK(p->l), getMinimumVK(p->r)));
      p->mx = max(make_pair(p->data, p->key), max(getMaximumVK(p->l), getMaximumVK(p->r)));
    }
  }
  void split (pNode t, K key, pNode & l, pNode & r) {
    if (!t) l = r = NULL;
    else if (key < t->key) split (t->l, key, l, t->l),  r = t;
    else split (t->r, key, t->r, r),  l = t;
    update(t);
  }
  void insert (pNode & t, pNode it) {
    if (!t) t = it;
    else if (it->prior > t->prior) split (t, it->key, it->l, it->r),  t = it;
    else insert (it->key < t->key ? t->l : t->r, it);
    update(t);
  }
  void merge (pNode & t, pNode l, pNode r) {
    if (!l || !r) t = l ? l : r;
    else if (l->prior > r->prior) merge (l->r, l->r, r),  t = l;
    else merge (r->l, l, r->l),  t = r;
    update(t);
  }
  void erase (pNode & t, K key) {
    if (t->key == key) merge (t, t->l, t->r);
    else erase (key < t->key ? t->l : t->r, key);
    update(t);
  }
  pNode unite (pNode l, pNode r) {
    if (!l || !r)  return l ? l : r;
    if (l->prior < r->prior)  swap (l, r);
    pNode lt, rt;
    split (r, l->key, lt, rt);
    l->l = unite (l->l, lt);
    l->r = unite (l->r, rt);
    return l;
  }
  pNode find_by_order(pNode t, int k) {
    if(t == NULL) return NULL;
    if(getSize(t->l) + 1 < k) return find_by_order(t->r, k - (getSize(t->l) + 1));
    else if(getSize(t->l) + 1 == k) return t;
    else return find_by_order(t->l, k);
  }
  int order_of_key(pNode t, K k) {
    if(t == NULL) return 0;
    if(k >= t->key) return order_of_key(t->r, k) + getSize(t->l) + 1;
    else return order_of_key(t->l, k);
  }
  bool find(pNode t, K k) {
    if(t == NULL) return false;
    if(t->key == k) return 1;
    if(t->key > k) return find(t->l, k);
    else return find(t->r, k);
    update(t);
  }
  K getMinimumKey(pNode t) {
    while(t->l) t = t->l;
    return t->key;
  }
  K getMaximumKey(pNode t) {
    while(t->r) t = t->r;
    return t->key;
  }
  pair<V, K> getMinimumValueAfter(pNode t, int fkey) {
    if(!t) return make_pair(numeric_limits<V>::max(),numeric_limits<K>::max());
    pair<V, K> foo = fkey <= t->key ? make_pair(t->data, t->key) : make_pair(numeric_limits<V>::max(),numeric_limits<K>::max());
    if(t->key >= fkey) return min(foo, min(getMinimumVK(t->r), getMinimumValueAfter(t->l, fkey)));
    else return getMinimumValueAfter(t->r, fkey);
  }
  pair<V, K> getMaximumValueAfter(pNode t, int fkey) {
    if(!t) return make_pair(numeric_limits<V>::min(),numeric_limits<K>::min());
    pair<V, K> foo = fkey <= t->key ? make_pair(t->data, t->key) : make_pair(numeric_limits<V>::min(),numeric_limits<K>::min());
    if(t->key >= fkey) return max(foo, max(getMaximumVK(t->r), getMaximumValueAfter(t->l, fkey)));
    else return getMaximumValueAfter(t->r, fkey);
  }
  pair<V, K> getMinimumValueBefore(pNode t, int fkey) {
    if(!t) return make_pair(numeric_limits<V>::max(),numeric_limits<K>::max());
    pair<V, K> foo = fkey >= t->key ? make_pair(t->data, t->key) : make_pair(numeric_limits<V>::max(),numeric_limits<K>::max());
    if(t->key <= fkey) return min(foo, min(getMinimumVK(t->l), getMinimumValueBefore(t->r, fkey)));
    else return getMinimumValueBefore(t->l, fkey);
  }
  pair<V, K> getMaximumValueBefore(pNode t, int fkey) {
    if(!t) return make_pair(numeric_limits<V>::min(),numeric_limits<K>::min());
    pair<V, K> foo = fkey >= t->key ? make_pair(t->data, t->key) : make_pair(numeric_limits<V>::min(),numeric_limits<K>::min());
    if(t->key <= fkey) return max(foo, max(getMaximumVK(t->l), getMaximumValueBefore(t->r, fkey)));
    else return getMaximumValueBefore(t->l, fkey);
  }
  void showTree(pNode t) {
    if(!t) return;
    showTree(t->l);
    printf("%d %d\n", t->key, t->data);
    showTree(t->r);
  }
  void allTree(pNode t, vector< V >&foo) {
    if(!t) return;
    allTree(t->l, foo);
    foo.push_back(t->data);
    allTree(t->r, foo);
  }
public:
  pNode root;
  Treap() {
    root = NULL;
  }
  void insert(K key, V data) {
    pNode it = new Node(key, data);
    insert(root, it);
  }
  void erase(K key) {
    if(find(key)) {
      erase(root, key);
    }
  }
  V find_by_order(int k) {
    pNode p = find_by_order(root, k);
    if(p == NULL) return V();
    return p->data;
  }
  int order_of_key(K k) {
    return order_of_key(root, k);
  } 
  bool find(K k) {
    return find(root, k);
  }
  K getMinimumKey() {
    return getMinimumKey(root);  
  }
  K getMaximumKey() {
    return getMaximumKey(root);
  }
  pair<V, K> getMinimumValueAfter(int w) {
    return getMinimumValueAfter(root, w);  
  }
  pair<V, K> getMaximumValueAfter(int w) {
    return getMaximumValueAfter(root, w);  
  }
  pair<V, K> getMinimumValueBefore(int w) {
    return getMinimumValueBefore(root, w);  
  }
  pair<V, K> getMaximumValueBefore(int w) {
    return getMaximumValueBefore(root, w);  
  }
  pair<V, K> getMinimumValue() {
    return getMinimumVK(root);
  }
  pair<V, K> getMaximumValue() {
    return getMaximumVK(root);
  }
  void showTree() {
    showTree(root);
  }
  vector< V > getTree() {
    vector< V > foo;
    allTree(root, foo);
    return foo;
  }
};
 
template <typename T>
class IntervalTreeSum {
  struct Node {
    T ps, mx, mn;
    int maxIndexMinimum = 1;
    int minIndexMinimum = 500000;
    Node(){
      ps = mx = mn = 0;
    };
    Node(int _ps, int _mn, int _mx) {
      ps = _ps;
      mn = _mn;
      mx = _mx;
    }
    Node(int _ps, int _mn, int _mx, int _minIndexMinimum, int _maxIndexMinimum) {
      ps = _ps;
      mn = _mn;
      mx = _mx;
      minIndexMinimum = _minIndexMinimum;
      maxIndexMinimum = _maxIndexMinimum;
    }
     Node operator + (Node other) const {
      int fooMin = (mn <= other.mn) ? minIndexMinimum : other.minIndexMinimum;
      int fooMax = (other.mn <= mn) ? other.maxIndexMinimum : maxIndexMinimum;
      return Node(ps + other.ps, 
        min(mn, other.mn),
        max(mx, other.mx), 
        fooMin,
        fooMax);
    }
    void showInfo() {
      printf("{%2d, %2d, %2d}\n", ps, mn, mx);
    }
  };
public:
  vector< Node > tr;
  vector< int > lz;
  int n;
  void build(int no, int l, int r) {
    if (l == r) {
      tr[no].minIndexMinimum = tr[no].maxIndexMinimum = l;
      return;
    }
    int nxt = (no << 1);
    int mid = (l + r) >> 1;
    build(nxt, l, mid);
    build(nxt + 1, mid + 1, r);
    tr[no] = tr[nxt] + tr[nxt + 1];
  }
  IntervalTreeSum(){
    n = (int) 500000;
    tr.resize(n * 4);
    lz.resize(n * 4);
    build(1, 1, n);
  };
  void propagate(int no, int l, int r) {
    if(!lz[no]) return;
    tr[no].ps += (r - l + 1) * lz[no];
    tr[no].mn += lz[no];
    tr[no].mx += lz[no];
    if(l != r) {
      int nxt = (no << 1), mid = (l + r) >> 1;
      lz[nxt] += lz[no];
      lz[nxt + 1] += lz[no];
    }
    lz[no] = 0;
  }
  void update(int no, int l, int r, int t, int inc) {
    propagate(no, l, r);
    if(l == r) {
      lz[no] += inc;
      propagate(no, l, r);
      return;
    }
    int nxt = (no << 1), mid = (l + r) >> 1;
    if(t <= mid) {
      update(nxt, l, mid, t, inc); 
      lz[nxt + 1] += inc;
    }
    else update(nxt + 1, mid + 1, r, t, inc);
    propagate(nxt, l, mid);
    propagate(nxt + 1, mid + 1, r);
    tr[no] = tr[nxt] + tr[nxt + 1];
  }
  Node query(int no, int l, int r, int i, int j) {
    propagate(no, l, r);
    if(r < i || l > j) return Node(0, numeric_limits< T >::max(), numeric_limits< T >::min());
    if(l >= i && r <= j) return tr[no];
    int nxt = (no << 1), mid = (l + r) >> 1;
    return query(nxt, l, mid, i, j) + query(nxt + 1, mid + 1, r, i, j);
  }
  void showTree(int no, int l, int r) {
    propagate(no, l, r);
    if(l == r) {
      printf("%d, ", tr[no].ps);
      return;
    }
    int nxt = (no << 1), mid = (l + r) >> 1;
    showTree(nxt, l, mid); showTree(nxt + 1, mid + 1, r);
  }
  void update(int t, T inc) {
    update(1, 1, n, t, inc);
  }
  Node query(int l, int r) {
    return query(1, 1, n, l, r);
  }
  void showTree() {
    showTree(1, 1, n);
    printf("\n");
  }
  int getLastBridge(int t) {
    Node sol = query(1, 1, n, 1, t);
    return sol.mn ? 1 : sol.maxIndexMinimum;
  }
  int getNextBridge(int t) {
    Node sol = query(1, 1, n, t, n);
    return sol.mn ? n : sol.minIndexMinimum;
  }
};
 
namespace Retroactivity {
  template <typename T>
  class PartialPriorityQueue {
  public:
    Treap< int, T > qnow;
    Treap< int, T > nqnow;
    IntervalTreeSum< int > bridges;
    void insertPush(int t, T data) {
      int tl = bridges.getLastBridge(t);
      nqnow.insert(t, data);
      bridges.update(t, 1);
      pair<T, int> add = nqnow.getMaximumValueAfter(tl);
      nqnow.erase(add.second);
      qnow.insert(add.second, add.first);
      bridges.update(add.second, -1);
    }
    void insertPop(int t) {
      int tl = bridges.getNextBridge(t);
      bridges.update(t, -1);
      pair<T, int> rem = qnow.getMinimumValueBefore(tl);
      qnow.erase(rem.second);
      nqnow.insert(rem.second, rem.first);
      bridges.update(rem.second, 1);
    }
    T getPeak() {
      return qnow.getMinimumValue().first;
    }
  };
}
 
int main() {
  srand (time (NULL));
  Retroactivity::PartialPriorityQueue< int > q;
  
  int n;
  cin >> n;
 
  for (int i = 0; i < n; ++i) {
    int x;
    scanf ("%d", &x);
 
    q.insertPush (i + 1, n - x);
  }
 
  for (int i = 0; i < n; ++i) {
    printf ("%d%c", n - q.getPeak (), " \n"[i + 1 == n]);
    
    int t;
    scanf ("%d", &t);
    --t;
    q.insertPop (t + 1);
  }
 
  return 0;
}