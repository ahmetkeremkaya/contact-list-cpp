#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <cmath>
#include <functional>

#include "Node.h"
#include "NoSuchItemException.h"

template<class T>
class DefaultBalanceCondition {
public:
    bool operator()(int current, int ideal) const {
        // Default BST: NEVER triggers rebalancing
        return true;
    }
};


template<class T,
         typename BalanceCondition = DefaultBalanceCondition<T>,
         typename Comparator = std::less<T> >
class BinarySearchTree {
public:
    BinarySearchTree();
    BinarySearchTree(const std::list<T> &sortedList);
    BinarySearchTree(const BinarySearchTree &obj);
    ~BinarySearchTree();
    
    Node<T>* RecursiveSortedBST(const std::vector<T>& sortedVector,
                                int start, int end);
                                
                                
    Node<T>* buildTreeFromNodes(std::vector<Node<T>*>& nodes, int start, int end);
    
    void removeNode(Node<T> * root);
    
    Node<T>* copyTree(const Node<T>* root);

    BinarySearchTree &operator=(const BinarySearchTree &rhs);
    
    void bstPrintHelper(const Node<T>* node, std::ostream &out)const;
    
    void inorderVector(std::vector<Node<T>*> & nodes, Node<T> * node );
    
    int getLeftSubtreeSize(int n);
    
    bool contains(const T &element) const;
    
    void toCompleteBST(Node<T>* &subRoot);
    
    bool insertHelper(Node<T>* &root, const T &element);
    
    bool removeHelper(Node<T>* &root, const T &element);
    
    void printInorderHelper(Node<T>* node, bool &isFirst) const;
    
    void printPreorderHelper(Node<T>* node, bool &isFirst) const;
    
    void printPostorderHelper(Node<T>* node, bool &isFirst) const;
    
    void printPrettyHelper(Node<T> *r, std::string indent, bool isLeft, bool isRoot) const;
    
    void removeAllNodesHelper(Node<T>* root);
    
    const T &getHelper(Node<T>*root,const T &element) const;
    
    const T &getMinHelper(Node<T>* root) const;
    
    const T &getMaxHelper(Node<T>* root) const;
    
    void findHelper(Node<T>* root,std::list<Node<T> *> &listem,const T &low, const T &high) const;

    bool insert(const T &element);
    bool remove(const T &element);
    void removeAllNodes();

    bool isEmpty() const;
    int getHeight() const;
    int getSize() const;

    const T &get(const T &element) const;
    const T &getMin() const;
    const T &getMax() const;
    const T &getNext(const T &element) const;
    const T &getCeiling(const T &element) const;
    const T &getFloor(const T &element) const;

    std::list<Node<T> *> find(const T &low, const T &high) const;

    void toCompleteBST();

    void printPreorder() const;
    void printInorder() const;
    void printPostorder() const;
    void printPretty() const;


    void print(std::ostream &out) const;

private:
    Node<T> *root;
    int numNodes;
    Comparator isLessThan;
    BalanceCondition isBalancedFunctor;

private:


};

/////////////////////////////////////////////////////////////
///////////  CONSTRUCTORS / DESTRUCTOR  /////////////////////
/////////////////////////////////////////////////////////////

template<class T, typename B, typename C>
BinarySearchTree<T,B,C>::BinarySearchTree() : 
      root(NULL),
      numNodes(0),
      isLessThan(C()),
      isBalancedFunctor(B())
  {  
}

template<class T, typename B, typename C>
BinarySearchTree<T,B,C>::BinarySearchTree(const std::list<T> &sortedList)
:     root(NULL),
      numNodes(0),
      isLessThan(C()),
      isBalancedFunctor(B())
{
    
   
  std::vector<T> sortedVector;
    for (typename std::list<T>::const_iterator it = sortedList.begin(); 
         it != sortedList.end(); 
         ++it) 
    {
        sortedVector.push_back(*it); 
    }
   numNodes = sortedVector.size();
   
   if ( numNodes > 0)
   root = RecursiveSortedBST (sortedVector,0,numNodes - 1);
    
}



template<class T, typename B, typename C>
int BinarySearchTree<T,B,C>::getLeftSubtreeSize(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 0;

    // Ağacın yüksekliği: h = floor(log2(n))
    int h = (int)log2(n);

    // Bu yükseklikteki tam bir ağacın son seviyesindeki maksimum düğüm sayısı (2^h)
    int maxNodesLastLevel = (1 << h); 

    // Son seviyeye gelmeden önceki toplam düğüm sayısı (2^h - 1)
    int nodesPriorLevels = (1 << h) - 1;

    // Son seviyeye sarkan düğüm sayısı
    int extraNodes = n - nodesPriorLevels;

    // Sol tarafın son seviyedeki kapasitesi (Son seviyenin yarısı)
    int capacityLeftLast = (maxNodesLastLevel / 2);

    // Sol tarafın son seviyeden alacağı gerçek pay
    int actualLeftLast = (extraNodes < capacityLeftLast) ? extraNodes : capacityLeftLast;

    // Sol alt ağacın toplam boyutu: (Solun tam dolu üst kısımları) + (Son seviyedeki payı)
    int leftSize = ((1 << (h - 1)) - 1) + actualLeftLast;

    return leftSize;
}

template<class T, typename B, typename C>
Node <T> * BinarySearchTree<T,B,C>::RecursiveSortedBST(const std::vector<T> &sortedVector,int start,int end)
{
    if(start>end) return NULL;
    
    // 1. Bu aralıktaki toplam eleman sayısını bul
    int n = end - start + 1;

    // 2. Sol tarafa kaç eleman düşeceğini yardımcı fonksiyondan al
    int leftSize = getLeftSubtreeSize(n);

    // 3. Kökün indeksini belirle (Başlangıç + Solun Boyutu)
    int mid = start + leftSize;
    Node<T>* node = new Node<T>();
    node->element = sortedVector[mid];

    node ->left = RecursiveSortedBST(sortedVector,start,mid -1);
    node ->right = RecursiveSortedBST(sortedVector,mid+1,end);
    int LeftHeight = (node->left) ? node->left->height : -1 ; 
    int RightHeight = (node->right) ? node->right->height : -1 ; 
    int LeftSize = (node->left) ? node->left->subsize : 0 ; 
    int RightSize = (node->right) ? node->right->subsize : 0 ; 
    
    node ->height = std::max(LeftHeight , RightHeight) + 1;
    node -> subsize = LeftSize + RightSize + 1;
    
    return node;
    
    
}

template<class T, typename B, typename C>
Node<T>* BinarySearchTree<T,B,C>::copyTree(const Node<T>* root){
    if( root==NULL)
    return NULL;
    
    
    Node<T> * newRoot = new Node<T>;
    newRoot -> element = root ->element;
    newRoot -> height = root ->height;
    newRoot -> subsize = root ->subsize;
    newRoot -> left = copyTree(root ->left);
    newRoot -> right = copyTree(root ->right);
    return newRoot;
}

template<class T, typename B, typename C>
BinarySearchTree<T,B,C>::BinarySearchTree(const BinarySearchTree &obj)

{
    numNodes = obj.numNodes;
    isLessThan = obj.isLessThan;
    isBalancedFunctor = obj.isBalancedFunctor;
    root = copyTree(obj.root);
    
    
}

template<class T, typename B, typename C>

void BinarySearchTree<T,B,C>::removeNode(Node<T> * root){
    if (root -> left)
    removeNode(root->left);
    
    if (root -> right)
    removeNode(root->right);
    
    delete root ;
}


template<class T, typename B, typename C>
BinarySearchTree<T,B,C>::~BinarySearchTree() {
   if(root!=NULL)
   removeNode(root);
    
    root = NULL ;
    numNodes = 0;
}





template<class T, typename B, typename C>
BinarySearchTree<T,B,C> &
BinarySearchTree<T,B,C>::operator=(const BinarySearchTree &rhs)
{
    if(this != &rhs){
        removeAllNodes();
        numNodes = rhs.numNodes;
        isLessThan = rhs.isLessThan;
        isBalancedFunctor = rhs.isBalancedFunctor;
        root = copyTree(rhs.root);
    
        
        
        
    }
    return *this;
}


template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::removeAllNodesHelper(Node<T>* root){
    if(root==NULL) return;
    
    removeAllNodesHelper(root->left);
    removeAllNodesHelper(root->right);
    delete root;
}

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::removeAllNodes()
{
    removeAllNodesHelper(root);
    root=NULL;
    numNodes=0;
    
}

/////////////////////////////////////////////////////////////
//////////////////// GETTERS ////////////////////////////////
/////////////////////////////////////////////////////////////

template<class T, typename B, typename C>
bool BinarySearchTree<T,B,C>::contains(const T &element) const{
    if(root==NULL) return true;
    Node<T>* current = root;
    while(current!=NULL){
        
        if(current->element == element)
        return true;
        
        else if(isLessThan(current->element,element))
        current = current->right;
        
        else if(isLessThan(element,current->element))
        current = current->left;
        
        
        
    }
    return false;
} 

template<class T, typename B, typename C>
bool BinarySearchTree<T,B,C>::isEmpty() const {
    return root==NULL;
 }

template<class T, typename B, typename C>
int BinarySearchTree<T,B,C>::getHeight() const {
    if (root==NULL) return -1;
    
    return root->height;
}

template<class T, typename B, typename C>
int BinarySearchTree<T,B,C>::getSize() const {
    return numNodes;
 }


/////////////////////////////////////////////////////////////
////////////////////// FIND NODE ////////////////////////////
/////////////////////////////////////////////////////////////
template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getHelper(Node<T>*root,const T &element) const{
    if (root==NULL) throw NoSuchItemException();
    
    if(root->element==element)
    return root->element;
    
    if(isLessThan(root->element,element))
    return getHelper(root->right,element);
    
    if(isLessThan(element,root->element))
    return getHelper(root->left,element);
    
    
    
    
}


template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::get(const T &element) const
{
    return getHelper(root,element);
}

/////////////////////////////////////////////////////////////
//////////////////// INSERT / REMOVE ////////////////////////
/////////////////////////////////////////////////////////////
template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::toCompleteBST(Node<T>* &subRoot){
    std::vector<Node<T>*> nodes;
    
    inorderVector(nodes,subRoot);
    
    // HATA BURADAYDI: numNodes değil, nodes.size() kullanılmalı.
    int count = nodes.size();
    
    if(count > 0) {
        // Vektörün başından sonuna kadar (0 ile count-1 arası)
        subRoot = buildTreeFromNodes(nodes, 0, count - 1);
    
}

}



template<class T, typename B, typename C>
bool BinarySearchTree<T,B,C>::insertHelper(Node<T>* &root, const T &element) {
    
    // 1. BASE CASE: Boş düğüme geldik, yeni node oluştur. [cite: 111]
    if (root == NULL) {
        // Yeni node: height=0, subsize=1 (yaprak düğüm)
        root = new Node<T>(element, NULL, NULL, 0, 1); 
        numNodes++; // Global sayacı artır
        return true;
    }

    bool inserted = false;

    // 2. RECURSIVE ADIMLAR [cite: 110]
    if (isLessThan(element, root->element)) {
        inserted = insertHelper(root->left, element);
    }
    else if (isLessThan(root->element, element)) {
        inserted = insertHelper(root->right, element);
    }
    else {
        // Eleman zaten var, güncelleme yapılması istenmiş olabilir ama
        // insert fonksiyonu 'false' dönmeli diyor. [cite: 107]
        return false; 
    }

    // Eğer yeni ekleme yapılmadıysa (duplicate), metadata güncellemeye gerek yok
    if (!inserted) return false;

    // 3. METADATA GÜNCELLEME (Backtracking sırasında) [cite: 113]
    
    // Alt düğümlerin verilerini güvenli şekilde al (Seg Fault önlemi)
    int leftHeight = (root->left != NULL) ? root->left->height : -1; // Boşsa -1 veya 0 (implementasyona göre)
    int rightHeight = (root->right != NULL) ? root->right->height : -1;
    
    int leftSubsize = (root->left != NULL) ? root->left->subsize : 0;
    int rightSubsize = (root->right != NULL) ? root->right->subsize : 0;

    // Height güncelle: max(sol, sağ) + 1
    root->height = 1 + std::max(leftHeight, rightHeight);
    
    // Subsize güncelle: sol + sağ + kendisi 
    root->subsize = leftSubsize + rightSubsize + 1;

    // 4. BALANCE CHECKING [cite: 116, 117, 118]
    // İdeal yükseklik = floor(log2(N))
    int idealHeight =(int)log2(root->subsize);
    
    // PDF: Eğer functor FALSE dönerse dengesizdir.
    if (!isBalancedFunctor(root->height, idealHeight)) {
        // 5. REBALANCING 
        // Sadece bu alt ağacı (root) yeniden kur.
        toCompleteBST(root); 
    }

    return true;
}

template<class T, typename B, typename C>
bool BinarySearchTree<T,B,C>::insert(const T &element)
{
   return insertHelper(root,element);
}

template<class T, typename B, typename C>
bool BinarySearchTree<T,B,C>::removeHelper(Node<T> * &root,const T &element){
    if(root==NULL) return false;
    
   
    
    bool deleted = false;
    
    if(isLessThan(root->element,element)){
        deleted=removeHelper(root->right,element);
    }
    else if (isLessThan(element,root->element)){
        deleted = removeHelper(root->left,element);
    }
    else{
        if(root->left!=NULL && root->right!=NULL){
            Node<T>* rightMax = root->right ;
            Node<T>* rightMaxParent = root ;
            
            while(rightMax->left!=NULL){
                rightMaxParent = rightMax;
                rightMax=rightMax->left;
                
            }
            if(rightMaxParent!=root){
                rightMaxParent->left = rightMax->right;
                rightMax -> right = root -> right;
            }
            rightMax -> left = root -> left;
            
            Node<T>* temp = root ;
            root = rightMax;
        
            delete temp;
            numNodes--;
            deleted = true;
            
        }
        else{
            Node<T>* temp = root;
            root = (root->left) ? root->left : root->right;
            delete temp;
            numNodes--;
            deleted = true;
            
        }
    }
    if(!deleted) return false;
    
    // --- KRİTİK DÜZELTME 2: NULL KONTROLÜ ---
    // Eğer sildiğimiz düğüm bir yaprak idiyse, root şu an NULL olabilir.
    // NULL olan bir şeye ->left veya ->height diyemezsin.
    if(root == NULL) return true;
    
    int leftHeight = (root->left != NULL) ? root->left->height : -1; // Boşsa -1 veya 0 (implementasyona göre)
    int rightHeight = (root->right != NULL) ? root->right->height : -1;
    
    int leftSubsize = (root->left != NULL) ? root->left->subsize : 0;
    int rightSubsize = (root->right != NULL) ? root->right->subsize : 0;
    
    root->height = 1 + std::max(leftHeight, rightHeight);
    root->subsize = leftSubsize + rightSubsize + 1;
    
    int idealHeight =(int)log2(root->subsize);
    
     if (!isBalancedFunctor(root->height, idealHeight)) {
        // 5. REBALANCING 
        // Sadece bu alt ağacı (root) yeniden kur.
        toCompleteBST(root); 
    }
    return true;
    
}


template<class T, typename B, typename C>
bool BinarySearchTree<T,B,C>::remove(const T &element) {
    return removeHelper(root,element);
}

/////////////////////////////////////////////////////////////
//////////////////// MIN/MAX/NEXT ///////////////////////////
/////////////////////////////////////////////////////////////


template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getMinHelper(Node<T>* root) const{
    if(root->left==NULL) return root->element;
    
    else{
        return getMinHelper(root->left);
    }
    
}

template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getMin() const {
    if(root==NULL) throw NoSuchItemException();
    return getMinHelper(root);
    
    
}

template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getMaxHelper(Node<T>* root) const{
    if(root->right==NULL) return root->element;
    
    else{
        return getMaxHelper(root->right);
    }
    
}

template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getMax() const {
     if(root==NULL) throw NoSuchItemException();
    return getMaxHelper(root);
}





template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getNext(const T &key) const {
   if (root == NULL) throw NoSuchItemException();

    Node<T> *current = root;
    Node<T> *successor = NULL;

    while (current != NULL) {
        // Eğer current, aranan key'den BÜYÜKSE:
        // Bu bir potansiyel cevaptır (successor). Bunu hafızaya at (successor = current).
        // Ama belki daha küçüğü vardır diye SOLA git.
        if (isLessThan(key, current->element)) {
            successor = current;
            current = current->left;
        }
        // Eğer current, aranan key'den KÜÇÜK veya EŞİTSE:
        // Bu sayı işimize yaramaz (bize daha büyüğü lazım), SAĞA git.
        else {
            current = current->right;
        }
    }

    // Döngü bitti. Eğer successor hiç bulunamadıysa (örneğin en büyük sayının next'i istendi), hata fırlat.
    if (successor == NULL) {
        throw NoSuchItemException();
    }

    return successor->element;
}
    
    


/////////////////////////////////////////////////////////////
//////////////// CEILING & FLOOR ////////////////////////////
/////////////////////////////////////////////////////////////


template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getFloor(const T &key) const {
    if (root == NULL) throw NoSuchItemException();

    Node<T> *current = root;
    Node<T> *floor = NULL;

    while (current != NULL) {
        // 1. EŞİTLİK DURUMU
        if (current->element == key) {
            return current->element;
        }

        // 2. KÜÇÜK OLMA DURUMU: Current < Key
        // Bu bir adaydır. Hafızaya al ve daha büyüğü (key'e daha yakını) var mı diye SAĞA git.
        if (isLessThan(current->element, key)) {
            floor = current;
            current = current->right;
        }
        // 3. BÜYÜK OLMA DURUMU: Current > Key
        // Bu sayı key'den büyük, taban olamaz. Küçültmek için SOLA git.
        else {
            current = current->left;
        }
    }

    // Aday bulunamadıysa (örneğin ağaçtaki tüm sayılar key'den büyükse)
    if (floor == NULL) throw NoSuchItemException();

    return floor->element;
}

template<class T, typename B, typename C>
const T &BinarySearchTree<T,B,C>::getCeiling(const T &key) const {
    if (root == NULL) throw NoSuchItemException();

    Node<T> *current = root;
    Node<T> *ceiling = NULL;

    while (current != NULL) {
        // 1. EŞİTLİK DURUMU: Tam aradığımız sayıysa, ceiling kendisidir.
        if (current->element == key) {
            return current->element;
        }

        // 2. BÜYÜK OLMA DURUMU: Current > Key
        // Bu bir adaydır. Hafızaya al ve daha küçüğü (daha yakını) var mı diye SOLA git.
        if (isLessThan(key, current->element)) {
            ceiling = current;
            current = current->left;
        }
        // 3. KÜÇÜK OLMA DURUMU: Current < Key
        // Bu sayı key'den küçük, tavan olamaz. Büyütmek için SAĞA git.
        else {
            current = current->right;
        }
    }

    // Aday bulunamadıysa (örneğin ağaçtaki tüm sayılar key'den küçükse)
    if (ceiling == NULL) throw NoSuchItemException();

    return ceiling->element;
}
/////////////////////////////////////////////////////////////
/////////////////////// RANGE FIND //////////////////////////
/////////////////////////////////////////////////////////////

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::findHelper(Node<T>* root, std::list<Node<T> *> &listem, const T &low, const T &high) const {
    
    if (root == NULL) return;
    
    // Durum 1: Düğüm aranan aralığın içinde mi? (low <= root <= high)
    if (root->element >= low && root->element <= high) {
        // Küçükleri bulmak için önce SOLA git
        findHelper(root->left, listem, low, high);
        
        // Ortadaki (şimdiki) değeri sona ekle (push_back)
        listem.push_back(root); 
        
        // Büyükleri bulmak için SAĞA git
        findHelper(root->right, listem, low, high);
    }
    // Durum 2: Düğüm aralıktan küçük mü? (root < low)
    else if (isLessThan(root->element, low)) {
        // Değer çok küçük, daha büyüğünü bulmak için SAĞA git
        findHelper(root->right, listem, low, high);
    }
    // Durum 3: Düğüm aralıktan büyük mü? (root > high)
    else {
        // Değer çok büyük, daha küçüğünü bulmak için SOLA git
        findHelper(root->left, listem, low, high);
    }
}

template<class T, typename B, typename C>
std::list<Node<T> *> BinarySearchTree<T,B,C>::find(const T &low, const T &high) const
{
    std::list<Node<T>*> listem;
    findHelper(root,listem,low,high);
    return listem;
    
    
    
}

/////////////////////////////////////////////////////////////
//////////////////// COMPLETE BST ///////////////////////////
/////////////////////////////////////////////////////////////

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::inorderVector(std::vector<Node<T>*> &nodes, Node<T> * node ){
    if(node==NULL) return;
    
    inorderVector(nodes,node->left);
    
    nodes.push_back(node);
    
    inorderVector(nodes,node->right);
    
   
}
template<class T, typename B, typename C>
Node<T>* BinarySearchTree<T,B,C>::buildTreeFromNodes(std::vector<Node<T>*>& nodes, int start, int end) {
    if (start > end) {
        return NULL;
    }
    
    int n = end - start + 1;
    int leftSize = getLeftSubtreeSize(n); // Mevcut fonksiyonunuzu kullanın
    int mid = start + leftSize;

    Node<T>* node = nodes[mid];

    // Pointerları yeniden bağla (Rewiring)
    node->left = buildTreeFromNodes(nodes, start, mid - 1);
    node->right = buildTreeFromNodes(nodes, mid + 1, end);

    // Metadata güncelle (height ve subsize)
    // (Burası constructor helper ile aynı)
    int lh = (node->left) ? node->left->height : -1;
    int rh = (node->right) ? node->right->height : -1;
    node->height = 1 + (lh > rh ? lh : rh); // std::max yerine manuel kontrol
    
    size_t ls = (node->left) ? node->left->subsize : 0;
    size_t rs = (node->right) ? node->right->subsize : 0;
    node->subsize = 1 + ls + rs;

    return node;
}



template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::toCompleteBST()
{
    std::vector<Node<T>*> nodes;
    
    inorderVector(nodes,root);
    
    if(numNodes>0)
    root = buildTreeFromNodes(nodes,0,numNodes-1);
    
}

/////////////////////////////////////////////////////////////
//////////////////////// PRINTS /////////////////////////////
/////////////////////////////////////////////////////////////

// === PREORDER ===

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printPreorderHelper(Node<T>* node, bool &isFirst) const {
    
    if (node == NULL) return; // NULL kontrolü
     // 2. YAZDIR (Virgül Mantığı Burada)
    if (!isFirst) {
        std::cout << ","<<std::endl; // Eğer ilk eleman değilse, önce virgülü bas
    }
    std::cout <<"\t" <<node->element; // Sonra sayıyı bas
    
    isFirst = false; // Artık ilk elemanı geçtik, bayrağı indir.

    // 1. SOLA GİT
    printPreorderHelper(node->left, isFirst);

   

    // 3. SAĞA GİT
    printPreorderHelper(node->right, isFirst);
}

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printPreorder() const {
      std::cout << "BST_preorder{"<<std::endl;
      bool isFirst = true; // Başlangıçta 'true' olarak ayarla
      printPreorderHelper(root, isFirst); // Referans olarak gönder
      std::cout << std::endl<<"}"<<std::endl;
}

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printInorderHelper(Node<T>* node, bool &isFirst) const {
    
    if (node == NULL) return; // NULL kontrolü

    // 1. SOLA GİT
    printInorderHelper(node->left, isFirst);

    // 2. YAZDIR (Virgül Mantığı Burada)
    if (!isFirst) {
        std::cout << ","<<std::endl; // Eğer ilk eleman değilse, önce virgülü bas
    }
    std::cout <<"\t" <<node->element; // Sonra sayıyı bas
    
    isFirst = false; // Artık ilk elemanı geçtik, bayrağı indir.

    // 3. SAĞA GİT
    printInorderHelper(node->right, isFirst);
}

// === INORDER ===
template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printInorder() const {
     std::cout << "BST_inorder{"<<std::endl;
     bool isFirst = true; // Başlangıçta 'true' olarak ayarla
    printInorderHelper(root, isFirst); // Referans olarak gönder
      std::cout << std::endl<<"}"<<std::endl;
}

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printPostorderHelper(Node<T>* node, bool &isFirst) const {
    
    if (node == NULL) return; // NULL kontrolü

    // 1. SOLA GİT
    printPostorderHelper(node->left, isFirst);

   

    // 3. SAĞA GİT
    printPostorderHelper(node->right, isFirst);
    
        // 2. YAZDIR (Virgül Mantığı Burada)
    if (!isFirst) {
        std::cout << ","<<std::endl; // Eğer ilk eleman değilse, önce virgülü bas
    }
    std::cout <<"\t" <<node->element; // Sonra sayıyı bas
    
    isFirst = false; // Artık ilk elemanı geçtik, bayrağı indir.
}


// === POSTORDER ===
template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printPostorder() const {
      std::cout << "BST_postorder{"<<std::endl;
      bool isFirst = true; // Başlangıçta 'true' olarak ayarla
      printPostorderHelper(root, isFirst); // Referans olarak gönder
      std::cout << std::endl<<"}"<<std::endl;
}

/////////////////////////////////////////////////////////////
/////////////////////// PRETTY PRINT ////////////////////////
/////////////////////////////////////////////////////////////
template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printPrettyHelper(Node<T> *r, std::string indent, bool isLeft, bool isRoot) const {
    
    // 1. Baz Durum: Eğer düğüm boşsa hiçbir şey yapma
    if (r == NULL) {
        return;
    }

    // 2. Ekrana Yazdırma İşlemi
    if (isRoot) {
        // Eğer kök düğümse, başında tire olmaz, sadece "root:" yazar
        std::cout << "\t"<<"root:" << r->element << std::endl;
    } 
    else {
        // Kök değilse, girinti (indent) ve yön bilgisi (l/r) yazılır
        std::cout <<"\t"<< indent;
        
        if (isLeft) {
            std::cout << "l:";
        } else {
            std::cout << "r:";
        }
        
        std::cout << r->element << std::endl;
    }

    // 3. Bir sonraki seviye için girintiyi ayarla
    // Eğer şu an kökteysek, çocukların girintisi "---" ile başlar.
    // Değilse, mevcut girintiye "---" eklenir.
    std::string nextIndent = (isRoot) ? "---" : indent + "---";

    // 4. Önce SOL çocuğu ziyaret et (Pre-order)
    printPrettyHelper(r->left, nextIndent, true, false);

    // 5. Sonra SAĞ çocuğu ziyaret et
    printPrettyHelper(r->right, nextIndent, false, false);
}
template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::printPretty() const
{
    // Kök düğümü (root) göndererek başlatıyoruz.
    // Başlangıçta indent boş (""), sol/sağ yok (false), ve bu bir kök (true).
    if (root==NULL)
    std::cout << "BST_pretty{";
    else{
    std::cout << "BST_pretty{"<<std::endl;
    printPrettyHelper(root, "", false, true);}
    
    std::cout <<"}"<<std::endl;
}

template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::bstPrintHelper(const Node<T>* node, std::ostream &out)const
{
    if (node == NULL) return;

    // Yaprak düğüm ise: sadece (key) yaz
    if (node->left == NULL && node->right == NULL) {
        out << "(" << node->element << ")";
        return;
    }

    // Yaprak değilse: [ left {key,Hx,Sy} right ]
    out << "[";

    if (node->left != NULL) {
        bstPrintHelper(node->left, out);
    }

    out << "{" << node->element
        << ",H" << node->height
        << ",S" << node->subsize
        << "}";

    if (node->right != NULL) {
        bstPrintHelper(node->right, out);
    }

    out << "]";
}


template<class T, typename B, typename C>
void BinarySearchTree<T,B,C>::print(std::ostream &out) const {
    if (root==NULL){
    
    return;}
    
    bstPrintHelper(root,out);
    out << "\n";
}
// Print pair<int,float> or any std::pair<K,V>
template<typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A,B>& p) {
    os << p.first;   // ONLY print the key
    return os;
}




#endif // BINARY_SEARCH_TREE_H
